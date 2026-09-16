/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC16F1936
        Driver Version    :  2.00
*/

// 20260505 F150_RX_V01 CS:8C60
// LED1 POS
// LED2 DRL
// LED3 lo beam
// LED3 hi beam

// 20260519 F150_XLT_RX_V02  CS:D96E
// 找出更詳細的控制條件

// 20260520 F150_XLT_RX_V03  CS:33DA
// 程式重構，修正成 switch case 的形式

// 20260522 F150_XLT_RX_V04  CS:A008
// 取消接收指示燈，增加左右方向燈

// 20260831 FR685_V05 CS:AA8B
// 1.更名FR685_V05
// 2.更新功能只收Hibeam和超車信號
// 3.消除編譯的警告
// 4.2秒沒收到正確信號關閉Hibeam和超車
// 5.31 08 LED OFF.

// 20260916 FR685V2_V06 CS:3912
// 1.新增POS燈控制，DRL/POS互斥
// 2.新增LOBEAM燈控制，LOBEAM/POS互斥
// 3.新增gpio.c gpio.h

#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/LINDrivers/lin_slave.h"

// 消除未呼叫警告 假裝有使用這些函數
void Clear_No_Call_Warning(void)
{
  uint8_t i = 0;
  if (i == 1)
  {
    TMR0_Reload();
    TMR0_ReadTimer();
  }
  else if (i == 2)
  {
    PIN_MANAGER_IOC();
  }
  else if (i == 3)
  {
    EUSART_get_last_status();
    EUSART_is_tx_done();
    EUSART_is_tx_ready();
  }
  else if (i == 4)
  {
    //    LIN_stopPeriod();
  }
  else if (i == 5)
  {
    PIN_MANAGER_IOC();
  }
}
/*
                         Main application
 */
int main(void)
{
  // initialize the device
  SYSTEM_Initialize();

  // Enable the Global Interrupts
  INTERRUPT_GlobalInterruptEnable();
  // Enable the Peripheral Interrupts
  INTERRUPT_PeripheralInterruptEnable();

  Clear_No_Call_Warning();
  while (1)
  {
    // Add your application code
    LIN_handler();
    if (LIN_signalTimeoutOccurred())
    {
      HIBEAM_LAT = 0; // hibeam or overtake
      // P2亮,但P2是電源直接給電.
    }
  }
  return 0;
}
/**
 End of File
*/