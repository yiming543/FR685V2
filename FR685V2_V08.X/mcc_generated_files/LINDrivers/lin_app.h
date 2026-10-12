/**
  LIN Slave Application
	
  Company:
    Microchip Technology Inc.

  File Name:
    lin_app.h

  Summary:
    LIN Slave Application

  Description:
    This header file provides the interface between the user and 
    the LIN drivers.

 */

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/


#ifndef LIN_APP_H
#define	LIN_APP_H

#include "lin_slave.h"

typedef enum {
    ID_00 = 0x0,
    ID_01 = 0x01,
    ID_10 = 0x10,
    ID_18 = 0x18
}lin_cmd_t;

uint8_t ID_00_Data[8];
uint8_t ID_01_Data[8];
uint8_t ID_10_Data[8]={0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF};
uint8_t ID_18_Data[8]={0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF};

const lin_rx_cmd_t scheduleTable[] = {
    //Command, Type, TX/RX Length, Data Address
    {ID_00, RECEIVE, 8, ID_00_Data },
    {ID_01, RECEIVE, 8, ID_01_Data },
    {ID_10, TRANSMIT, 8, ID_10_Data },
    {ID_18, TRANSMIT, 8, ID_18_Data }
};
#define TABLE_SIZE  (sizeof(scheduleTable)/sizeof(lin_rx_cmd_t))

void LIN_Slave_Initialize(void);

void processLIN(void);


#endif	/* LIN_APP_H */

