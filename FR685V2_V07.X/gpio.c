/*
 * File:   gpio.c
 * Author: T00904
 * 車燈輸入腳位(輸入) 
 * 車燈LED腳位(輸出)  DRL/HIBEAM
 * Created on 2026年2月23日, 下午 3:18
 */

#include <xc.h>
#include "mcc_generated_files/pin_manager.h"
#include <stdbool.h>
// #include <stdint.h>
#include "gpio.h"

#define HiBeam_en1 HIBEAM_LAT
#define LoBeam_en1 LOBEAM_LAT
#define DRL_en DRL_LAT
#define POS_en POS_LAT

#define ON 1
#define OFF 0

#define LAMP_L_SIDE 1

void HIBEAM_ON(void) { HiBeam_en1 = ON; }
void HIBEAM_OFF(void) { HiBeam_en1 = OFF; }
void LOBEAM_ON(void) { LoBeam_en1 = ON; }
void LOBEAM_OFF(void) { LoBeam_en1 = OFF; }

void DRL_ON(void)
{
    DRL_en = ON;
    POS_en = OFF;
}
void POS_ON(void)
{
    POS_en = ON;
    DRL_en = OFF;
}
void DRL_POS_OFF(void)
{
    DRL_en = OFF;
    POS_en = OFF;
}
