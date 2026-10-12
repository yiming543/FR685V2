# 1 "lamp.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 295 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include/language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "lamp.c" 2
# 20 "lamp.c"
# 1 "./gpio.h" 1
# 12 "./gpio.h"
extern void HIBEAM_ON(void) ;
extern void HIBEAM_OFF(void) ;
extern void LOBEAM_ON(void) ;
extern void LOBEAM_OFF(void) ;
extern void DRL_ON(void) ;
extern void POS_ON(void) ;
extern void DRL_POS_OFF(void) ;
# 21 "lamp.c" 2
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 1 3



# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/musl_xc8.h" 1 3
# 5 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 2 3
# 26 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 1 3
# 133 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef unsigned short uintptr_t;
# 148 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef short intptr_t;
# 164 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef signed char int8_t;




typedef short int16_t;




typedef __int24 int24_t;




typedef long int32_t;





typedef long long int64_t;
# 194 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef long long intmax_t;





typedef unsigned char uint8_t;




typedef unsigned short uint16_t;




typedef __uint24 uint24_t;




typedef unsigned long uint32_t;





typedef unsigned long long uint64_t;
# 235 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef unsigned long long uintmax_t;
# 27 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 2 3

typedef int8_t int_fast8_t;

typedef int64_t int_fast64_t;


typedef int8_t int_least8_t;
typedef int16_t int_least16_t;

typedef int24_t int_least24_t;
typedef int24_t int_fast24_t;

typedef int32_t int_least32_t;

typedef int64_t int_least64_t;


typedef uint8_t uint_fast8_t;

typedef uint64_t uint_fast64_t;


typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;

typedef uint24_t uint_least24_t;
typedef uint24_t uint_fast24_t;

typedef uint32_t uint_least32_t;

typedef uint64_t uint_least64_t;
# 148 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/stdint.h" 1 3
typedef int16_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef uint16_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
# 149 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 2 3
# 22 "lamp.c" 2


extern uint8_t ID_10_Data[8];
extern uint8_t ID_18_Data[8];
extern uint8_t fLamp_RL_Side;






typedef enum {
  OFF_NIGHT_P = 0x31,
  OFF_NIGHT_N = 0x71,
  OFF_DAY_P = 0x29,
  OFF_DAY_N = 0x69,
  DRIVER_NIGHT_P = 0x32,
  DRIVER_NIGHT_N = 0x72,
  DRIVER_DAY_P = 0x2A,
  DRIVER_DAY_N = 0x6A,
  AUTO_NIGHT_P = 0xB4,
  AUTO_NIGHT_N = 0xB4,
  AUTO_DAY_P = 0x2C,
  AUTO_DAY_N = 0x6C,
  LOBEAM_NIGHT_P = 0x33,
  LOBEAM_NIGHT_N = 0x33,
  LOBEAM_DAY_P = 0x2B,
  LOBEAM_DAY_N = 0x2B,
} ControlCode;
# 88 "lamp.c"
void feedbacksignal(uint8_t *data) {


  if (fLamp_RL_Side == 0)
    ID_18_Data[1] = data[1] & 0x07;
  else
    ID_10_Data[1] = data[1] & 0x07;

}

void lamp(uint8_t *tempRxData) {
  if (tempRxData[0] == 0x24) {
    switch (tempRxData[2]) {
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



      LOBEAM_ON();
      POS_ON();
      break;

    default:

      break;
    }

    if (tempRxData[1] & 0xC0) {
      HIBEAM_ON();
    }

  } else if ((tempRxData[0] == 0x01) && (tempRxData[1] == 0x08))
  {
    DRL_POS_OFF();
  } else if ((tempRxData[0] == 0x11) && (tempRxData[1] == 0x09))
  {
    POS_ON();
  }
  else if ((tempRxData[0] == 0x11) && (tempRxData[1] == 0x0A))
  {
    POS_ON();
  }
  else if ((tempRxData[0] == 0x11) && (tempRxData[1] == 0x0D))
  {
    POS_ON();
  }
  else if ((tempRxData[0] == 0x11) && (tempRxData[1] == 0x0B))
  {
    POS_ON();
  }
  else if ((tempRxData[0] == 0x11) && (tempRxData[1] == 0x0C))
  {
    POS_ON();
  }
  else if ((tempRxData[0] == 0x28) && (tempRxData[1] == 0x08))
  {
    DRL_POS_OFF();
  }
  else if ((tempRxData[0] == 0x31) && (tempRxData[1] == 0x0A))
  {
  }
  else if ((tempRxData[0] == 0x31) && (tempRxData[1] == 0x0B))
  {
  }
  else if ((tempRxData[0] == 0x31) && (tempRxData[1] == 0x0C))
  {
  }
  else if ((tempRxData[0] == 0x31) && (tempRxData[1] == 0x08))
  {
    DRL_POS_OFF();
  }
  else {

  }

  if ((tempRxData[1] & 0xC0) == 0) {
    HIBEAM_OFF();
  }
}
