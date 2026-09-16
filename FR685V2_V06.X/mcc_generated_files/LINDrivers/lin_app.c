/**
  LIN Slave Application

  Company:
    Microchip Technology Inc.

  File Name:
    lin_app.c

  Summary:
    LIN Slave Application

  Description:
    This source file provides the interface between the user and
    the LIN drivers.

 */

#include "../mcc.h"
#include "../../gpio.h"

#define ON 1
#define OFF 0

// D2
typedef enum
{
  OFF_NIGHT_P = 0x31,    // 0b0011 0001 47
  OFF_NIGHT_N = 0x71,    // 0b0111 0001 113
  OFF_DAY_P = 0x29,      // 0b0010 1001 41
  OFF_DAY_N = 0x69,      // 0b0110 1001 105
  DRIVER_NIGHT_P = 0x32, // 0b0011 0010 50
  DRIVER_NIGHT_N = 0x72, // 0b0111 0010 114
  DRIVER_DAY_P = 0x2A,   // 0b0010 1010 42
  DRIVER_DAY_N = 0x6A,   // 0b0110 1010 106
  AUTO_NIGHT_P = 0xB4,   // 0b1011 0100 180
  AUTO_NIGHT_N = 0xB4,   // 0b1011 0100 180
  AUTO_DAY_P = 0x2C,     // 0b0010 1100 44
  AUTO_DAY_N = 0x6C,     // 0b0110 1100 108
  LOBEAM_NIGHT_P = 0x33, // 0b0011 0011 51
  LOBEAM_NIGHT_N = 0x33, // 0b0011 0011 51
  LOBEAM_DAY_P = 0x2B,   // 0b0010 1011 43
  LOBEAM_DAY_N = 0x2B,   // 0b0010 1011 43
} ControlCode;

// D2 bit6: DRL
#define DRL_BIT 0x40

// 檔位	D2 bit2~0
typedef enum
{
  FUNC_OFF = 1,
  FUNC_DRIVER,
  FUNC_AUTO,
  FUNC_LOBEAM
} FUNC_MODE;

// D2 bit4/bit3 決定 Night/Day
#define MODE_DAY_NIGHT 0B00011000
#define MODE_NIGHT 0B00010000 // 10
#define MODE_DAY 0B00001000   // 01

uint16_t cnt = 0;
void LIN_Slave_Initialize(void)
{
  LIN_init(TABLE_SIZE, scheduleTable, processLIN);
}

void processLIN(void)
{
  uint8_t tempRxData[8];
  uint8_t cmd;
  ControlCode mode;

  cmd = LIN_getPacket(tempRxData);

  // ID NG return
  if (cmd != SIGNAL)
  {
    return;
  }

  if (tempRxData[0] == 0x24)
  {
    switch (tempRxData[2])
    {
    case OFF_NIGHT_P:
      LOBEAM_OFF();
      DRL_POS_OFF();
      break;

    case OFF_NIGHT_N:
      LOBEAM_OFF();
      DRL_ON();
      break;

    case OFF_DAY_P:
      LOBEAM_OFF();
      DRL_POS_OFF();
      break;

    case OFF_DAY_N:
      LOBEAM_OFF();
      DRL_ON();
      break;

    case DRIVER_NIGHT_P:
      LOBEAM_OFF();
      POS_ON();
      break;

    case DRIVER_NIGHT_N:
      LOBEAM_OFF();
      DRL_ON();
      break;

    case DRIVER_DAY_P:
      LOBEAM_OFF();
      POS_ON();
      break;

    case DRIVER_DAY_N:
      LOBEAM_OFF();
      DRL_ON();
      break;

    case AUTO_NIGHT_P:
      // case AUTO_NIGHT_N:
      LOBEAM_ON();
      POS_ON();
      break;

    case AUTO_DAY_P:
      LOBEAM_OFF();
      DRL_POS_OFF();
      break;

    case AUTO_DAY_N:
      LOBEAM_OFF();
      DRL_ON();
      break;

    case LOBEAM_NIGHT_P:
      // case LOBEAM_NIGHT_N:
      // case LOBEAM_DAY_P:
      // case LOBEAM_DAY_N:
      LOBEAM_ON();
      POS_ON();
      break;

    default:
      // 無法識別的資料，維持原本的狀態
      break;
    }

    if (tempRxData[1] & 0xC0)
    {
      HIBEAM_ON(); // hibeam or overtake
    }
    else
    {
      HIBEAM_OFF(); // hibeam or overtake
    }
  }
  else if ((tempRxData[0] == 0x31) && (tempRxData[1] == 0x08))
  {               // LED OFF
    HIBEAM_OFF(); // hibeam or overtake
    DRL_POS_OFF();
  }
  else if ((tempRxData[0] == 0x31) && (tempRxData[1] == 0x0A))
  { // Ignition OFF，維持原本的狀態
  }
  else
  {
    // 無法識別的資料，維持原本的狀態
  }
}