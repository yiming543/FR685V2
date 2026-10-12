/**
  LIN Slave Driver

  Company:
    Microchip Technology Inc.

  File Name:
    lin_slave.c

  Summary:
    LIN Slave Driver

  Description:
    This source file provides the driver for LIN slave nodes

 */

#include "lin_slave.h"
#include "../eusart.h"
#include "../tmr0.h"

#define READ_TIMEOUT 20         // ms：一個 LIN frame 從開始接收至完成的最長等待時間。
#define SIGNAL_TIMEOUT_MS 2000U // 2 seconds：超過此時間沒有有效 LIN frame，視為 LIN 訊號中斷。

/* 收到完整且通過驗證的 frame 後，由應用程式自行處理資料。 */
static void (*LIN_processData)(void);

/* LIN frame 的共用緩衝區。rawPacket 可視需要用來一次清除整個封包內容。 */
lin_packet_t LIN_packet;
/* true 表示目前已看到 frame 起始資料，接收狀態機正在等待後續欄位。 */
bool LIN_rxInProgress = false;
/* 應用程式提供的 LIN command/schedule table。 */
const lin_rx_cmd_t *LIN_rxCommand;

/* command table 中的項目數量。 */
uint8_t LIN_rxCommandLength;

static uint8_t LIN_timeout = 10;                       // 預設 10 ms；期間沒有收到下一個 byte 即視為 frame 接收失敗。
static bool LIN_timerRunning = false;                  // frame 接收逾時計時器是否仍在運作。
static volatile uint8_t CountCallBack = 0;             // 由 TMR0 每 1 ms 增加，用來計算 frame 接收逾時。
static volatile uint16_t LIN_signalElapsedMs = 0;      // 距離上次有效 LIN frame 的經過時間（毫秒）。
static volatile bool LIN_validSignalReceived = false;  // 主迴圈已取出有效 frame，通知 timer 重新計算訊號逾時。
static volatile bool LIN_signalTimedOut = false;       // 已經產生訊號逾時；保持 true 避免重複產生事件。
static volatile bool LIN_signalTimeoutPending = false; // 尚未被主迴圈讀取的訊號逾時事件。

/*
 * 讀取並清除一次 LIN 訊號逾時事件。
 * 事件由 TMR0 interrupt 設定，主迴圈透過此函式取出，因此可避免
 * interrupt 與主迴圈直接共用一個「只會出現一瞬間」的條件。
 */
bool LIN_signalTimeoutOccurred(void) {
  if (LIN_signalTimeoutPending) { // LIN 訊號超時待處理
    LIN_signalTimeoutPending = false;
    return true;
  }

  return false;
}

void LIN_init(uint8_t tableLength, const lin_rx_cmd_t *command, void (*processData)(void))
{
    /* 保存應用程式的 schedule table 與完成 frame 後的 callback。 */
    LIN_rxCommand = command; //scheduleTable
    LIN_rxCommandLength = tableLength;
    LIN_processData = processData;
    /* 初始化時先停止上一個可能殘留的 frame timeout。 */
    LIN_stopTimer();

    /* 預設開啟 UART 接收，並將 TMR0 設為 LIN timer 的 callback。 */
    LIN_enableRx();
    LIN_setTimerHandler();
}

void LIN_queuePacket(uint8_t cmd)
{
    /* 依 command 找到回應資料；局部指標只在 table 中向前搜尋。 */
    const lin_rx_cmd_t *tempSchedule = LIN_rxCommand; // copy table pointer so we can modify it

    cmd &= 0x3F; // command 只使用 PID 的 ID0~ID5，清除可能存在的 parity bits。
    for (uint8_t i = 0; i < LIN_rxCommandLength; i++)
    {
        if (cmd == tempSchedule->cmd)
        {
            break;
        }
        tempSchedule++; // go to next entry
    }

    LIN_packet.type = tempSchedule->type;
    LIN_packet.length = tempSchedule->length;

    /*
     * 建立從站回應 frame。資料內容、長度由 schedule table 定義，
     * checksum 則依 LIN checksum 規則由 PID 與所有資料 byte 計算。
     */
    memcpy(LIN_packet.data, tempSchedule->data, LIN_packet.length);

    // 先保存計算結果，再透過 UART 傳送資料與 checksum。
    LIN_packet.checksum = LIN_getChecksum(LIN_packet.length, LIN_packet.PID, LIN_packet.data);
    LIN_sendPacket(LIN_packet.length, LIN_packet.PID, LIN_packet.data);
}

lin_rx_state_t LIN_handler(void)
{
    /*
     * 此函式由主迴圈反覆呼叫，每次只處理目前 UART buffer 中可取得的
     * 一個接收階段。static 變數讓狀態與資料索引跨呼叫保存。
     */
    static lin_rx_state_t LIN_rxState = LIN_RX_IDLE;
    static uint8_t rxDataIndex = 0;

    if (LIN_rxInProgress == true)
    {
        if (LIN_timerRunning == false)
        {
            /* TMR0 已將 timer 停止，表示 frame 中間等待 byte 超時。 */
            LIN_rxState = LIN_RX_ERROR;
        }
    }

    switch (LIN_rxState)
    {
    case LIN_RX_IDLE:
        if (EUSART_is_rx_ready() > 0)
        {
            /* UART 收到第一個 byte 後開始計時，該 byte 應為 LIN break。 */
            LIN_startTimer(READ_TIMEOUT);
            LIN_rxInProgress = true;
            LIN_rxState = LIN_RX_BREAK;
        }
        break;
    case LIN_RX_BREAK:
        if (EUSART_is_rx_ready() > 0)
        {
            if (LIN_breakCheck() == true)
            { // 讀取並驗證 break：UART 應回報 0x00 且 framing error。
                LIN_rxState = LIN_RX_SYNC;
            }
            else
            {
                LIN_rxState = LIN_RX_ERROR;
            }
        }
        break;
    case LIN_RX_SYNC:
        if (EUSART_is_rx_ready() > 0)
        {
            if (EUSART_Read() == 0x55)
            { // sync byte 固定為 0x55；確認後丟棄，不存入資料。
                LIN_rxState = LIN_RX_PID;
            }
            else
            {
                LIN_rxState = LIN_RX_ERROR;
            }
        }
        break;
    case LIN_RX_PID:
        if (EUSART_is_rx_ready() > 0)
        {
            LIN_packet.PID = EUSART_Read();

            /* PID 必須同時存在於 schedule table，且 parity bits 必須正確。 */
            if (LIN_checkPID(LIN_packet.PID) == false)
            {
                LIN_rxState = LIN_RX_ERROR;
                break;
            }
            LIN_packet.type = LIN_getFromTable(LIN_packet.PID, TYPE);
            if (LIN_packet.type == RECEIVE)
            {
                /* RECEIVE 表示 master 傳資料給 slave，接著等待資料與 checksum。 */
                LIN_packet.length = LIN_getFromTable(LIN_packet.PID, LENGTH);
                LIN_rxState = LIN_RX_DATA;
            }
            else
            {
                /* TRANSMIT 表示 slave 應回傳資料，關閉接收避免回傳內容被重新接收。 */
                LIN_disableRx();
                LIN_rxState = LIN_RX_TX_DATA;
            }
        }
        break;
    case LIN_RX_DATA:
        if (EUSART_is_rx_ready() > 0)
        {
            /* 依 schedule table 指定的長度逐 byte 收入封包緩衝區。 */
            LIN_packet.data[rxDataIndex] = EUSART_Read();
            if (++rxDataIndex >= LIN_packet.length)
            {
                // 所有資料 byte 已收到，下一個 byte 應為 checksum。
                rxDataIndex = 0;
                LIN_rxState = LIN_RX_CHECKSUM;
            }
        }
        break;
    case LIN_RX_CHECKSUM:
        if (EUSART_is_rx_ready() > 0)
        {
            LIN_packet.checksum = EUSART_Read();
            /* 計算值與接收值不同時，整個 frame 視為無效。 */
            if (LIN_packet.checksum != LIN_getChecksum(LIN_packet.length, LIN_packet.PID, LIN_packet.data))
            {
                LIN_rxState = LIN_RX_ERROR;
            }
            else
            {
                LIN_rxState = LIN_RX_RDY;
            }
        }
        break;
    case LIN_RX_TX_DATA:
        LIN_queuePacket(LIN_packet.PID); // 依 PID 自動準備並送出從站回應。
        LIN_rxState = LIN_RX_RDY;
    case LIN_RX_RDY:
        /* RDY 與 TX_DATA 刻意直接落入下一個 case，完成 callback 後統一收尾。 */
        LIN_processData();
    case LIN_RX_ERROR:
        /*
         * 成功或失敗都在此清理接收上下文。此處不立即重新開啟 RX；
         * 若正在傳送，必須等 TX shift register 完成後由 LIN_RX_WAIT 處理。
         */
        CountCallBack = 0;
        LIN_timerRunning = false;

        rxDataIndex = 0;
        LIN_rxInProgress = false;
        memset(LIN_packet.rawPacket, 0, sizeof(LIN_packet.rawPacket)); // clear receive data
    case LIN_RX_WAIT:
        if (TXSTAbits.TRMT)
        {
            /* 確認 UART 最後一個 bit 已送出，才可切回接收模式。 */
            LIN_enableRx();
            LIN_rxState = LIN_RX_IDLE;
        }
        else
        {
            LIN_rxState = LIN_RX_WAIT;
        }
        break;
    }
    return LIN_rxState;
}

void LIN_sendPacket(uint8_t length, uint8_t pid, uint8_t *data)
{
    /* 先傳送資料欄位；LIN checksum 放在所有資料 byte 之後。 */
    for (uint8_t i = 0; i < length; i++)
    {
        EUSART_Write(*(data + i));
    }
    EUSART_Write(LIN_getChecksum(length, pid, data));
}

uint8_t LIN_getPacket(uint8_t *data)
{
    /*
     * 將最近一次驗證成功的資料複製給應用程式，並回傳不含 parity
     * 的 6-bit command。讀取後清空封包緩衝區，避免重複使用舊資料。
     */
    uint8_t cmd = LIN_packet.PID & 0x3F;

    memcpy(data, LIN_packet.data, sizeof(LIN_packet.data));
    memset(LIN_packet.rawPacket, 0, sizeof(LIN_packet.rawPacket));
    LIN_validSignalReceived = 1;
    return cmd;
}

uint8_t LIN_getFromTable(uint8_t cmd, lin_sch_param_t param)
{
    const lin_rx_cmd_t *rxCommand = LIN_rxCommand; // copy table pointer so we can modify it
    //scheduleTable
    
    cmd &= 0x3F; // table 以 6-bit command 比對，不包含 PID parity bits。
    // 在 schedule table 中搜尋 command。
    for (uint8_t length = 0; length < LIN_rxCommandLength; length++)
    {
        if (cmd == rxCommand->cmd)
        {
            break;
        }
        rxCommand++; // go to next entry

        if (length == (LIN_rxCommandLength - 1))
        {
            return ERROR; // command 不存在於 schedule table。
        }
    }

    switch (param)
    {
    case CMD:
        return (uint8_t)rxCommand->cmd;
    case TYPE:
        return (uint8_t)rxCommand->type;
    case LENGTH:
        return (uint8_t)rxCommand->length;
    default:
        break;
    }

    return ERROR;
}

bool LIN_checkPID(uint8_t pid)
{
    /* 先確認 command 被本節點支援，再驗證 PID 的兩個 parity bits。 */
    if (LIN_getFromTable(pid, TYPE) == ERROR)
        return false; // PID not in schedule table

    if (pid == LIN_calcParity(pid & 0x3F))
        return true;

    return false; // Parity Error
}

uint8_t LIN_calcParity(uint8_t CMD)
{
    lin_pid_t PID;
    PID.rawPID = CMD;

    /*
     * LIN 2.x PID parity：
     *   P0 = ID0 ^ ID1 ^ ID2 ^ ID4
     *   P1 = ~(ID1 ^ ID3 ^ ID4 ^ ID5)
     * 逐步計算是為了避開 Microchip 編譯器 CAE_MCU8-200 問題。
     */
    //    PID.P0 = PID.ID0 ^ PID.ID1 ^ PID.ID2 ^ PID.ID4;
    //    PID.P1 = ~(PID.ID1 ^ PID.ID3 ^ PID.ID4 ^ PID.ID5);
    PID.P0 = PID.ID0 ^ PID.ID1;
    PID.P0 = PID.P0 ^ PID.ID2;
    PID.P0 = PID.P0 ^ PID.ID4;
    PID.P1 = PID.ID1 ^ PID.ID3;
    PID.P1 = PID.P1 ^ PID.ID4;
    PID.P1 = PID.P1 ^ PID.ID5;
    PID.P1 = ~PID.P1;

    return PID.rawPID;
}

uint8_t LIN_getChecksum(uint8_t length, uint8_t pid, uint8_t *data)
{
    /*
     * 使用 enhanced checksum：將 PID 與各資料 byte 以 end-around carry
     * 累加，最後取反。checksum byte 本身不納入計算。
     */
    uint16_t checksum = pid;

    for (uint8_t i = 0; i < length; i++)
    {
        checksum = checksum + *data++;
        if (checksum > 0xFF)
            /* end-around carry：溢位的 1 加回低 8 bits。 */
            checksum -= 0xFF;
    }
    checksum = ~checksum;

    return (uint8_t)checksum;
}

void LIN_startTimer(uint8_t timeout)
{
    /* TMR0 已由 MCC 設定為 1 ms tick；從零開始計算 frame 接收逾時。 */
    LIN_timeout = timeout;
    TMR0_WriteTimer(0);
    NOP();
    LIN_timerRunning = true;
    CountCallBack = 0;
}

void LIN_timerHandler(void)
{
    /* 此函式在 TMR0 interrupt 中每 1 ms 執行一次。 */

    if (LIN_validSignalReceived)
    { // 收到有效 frame 時，重新開始計算 LIN 訊號逾時。
        LIN_validSignalReceived = false;
        LIN_signalElapsedMs = 0;
        LIN_signalTimedOut = false;
        LIN_signalTimeoutPending = false;
    }
    else if (!LIN_signalTimedOut)
    {
        if (++LIN_signalElapsedMs >= SIGNAL_TIMEOUT_MS)
        {
            /* 只產生一次 pending event，直到下一個有效 frame 將狀態清除。 */
            LIN_signalTimedOut = true;
            LIN_signalTimeoutPending = true;
        }
    }

    /* 另一個計數器負責單一 frame 的 byte 間隔/整體接收逾時。 */
    if (++CountCallBack >= LIN_timeout)
    {
        // LIN_stopTimer();
        // reset ticker counter
        CountCallBack = 0;
        LIN_timerRunning = false;
    }
}

void LIN_setTimerHandler(void)
{
    /* 將 LIN 的 1 ms handler 掛到 MCC 產生的 TMR0 interrupt。 */
    TMR0_SetInterruptHandler(LIN_timerHandler);
}

void LIN_stopTimer(void)
{
    /* 停止 frame timeout；訊號長時間逾時的計數仍由 handler 管理。 */
    CountCallBack = 0;
    LIN_timerRunning = false;
}

void LIN_enableRx(void)
{
    /* 開啟 UART continuous receive 與 receive interrupt。 */
    RCSTAbits.CREN = 1;
    PIE1bits.RCIE = 1;
}

void LIN_disableRx(void)
{
    /* 回傳 frame 時暫停接收，避免 TX/RX 同時處理造成狀態混亂。 */
    RCSTAbits.CREN = 0;
    PIE1bits.RCIE = 0;
}

bool LIN_breakCheck(void)
{
    /*
     * LIN break 的 UART 表現為資料 0x00 加 framing error。
     * 讀取該 byte 也會同時清除 UART receive buffer 中的 break。
     */
    if ((EUSART_Read() == 0x00) && (EUSART_get_last_status().ferr == 1))
    {
        return true;
    }

    return false;
}