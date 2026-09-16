/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC16F1936
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.36 and above
        MPLAB 	          :  MPLAB X 6.00	
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set TURN_RIGHT aliases
#define TURN_RIGHT_TRIS                 TRISBbits.TRISB0
#define TURN_RIGHT_LAT                  LATBbits.LATB0
#define TURN_RIGHT_PORT                 PORTBbits.RB0
#define TURN_RIGHT_WPU                  WPUBbits.WPUB0
#define TURN_RIGHT_ANS                  ANSELBbits.ANSB0
#define TURN_RIGHT_SetHigh()            do { LATBbits.LATB0 = 1; } while(0)
#define TURN_RIGHT_SetLow()             do { LATBbits.LATB0 = 0; } while(0)
#define TURN_RIGHT_Toggle()             do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define TURN_RIGHT_GetValue()           PORTBbits.RB0
#define TURN_RIGHT_SetDigitalInput()    do { TRISBbits.TRISB0 = 1; } while(0)
#define TURN_RIGHT_SetDigitalOutput()   do { TRISBbits.TRISB0 = 0; } while(0)
#define TURN_RIGHT_SetPullup()          do { WPUBbits.WPUB0 = 1; } while(0)
#define TURN_RIGHT_ResetPullup()        do { WPUBbits.WPUB0 = 0; } while(0)
#define TURN_RIGHT_SetAnalogMode()      do { ANSELBbits.ANSB0 = 1; } while(0)
#define TURN_RIGHT_SetDigitalMode()     do { ANSELBbits.ANSB0 = 0; } while(0)

// get/set TURN_LEFT aliases
#define TURN_LEFT_TRIS                 TRISBbits.TRISB1
#define TURN_LEFT_LAT                  LATBbits.LATB1
#define TURN_LEFT_PORT                 PORTBbits.RB1
#define TURN_LEFT_WPU                  WPUBbits.WPUB1
#define TURN_LEFT_ANS                  ANSELBbits.ANSB1
#define TURN_LEFT_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define TURN_LEFT_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define TURN_LEFT_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define TURN_LEFT_GetValue()           PORTBbits.RB1
#define TURN_LEFT_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define TURN_LEFT_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define TURN_LEFT_SetPullup()          do { WPUBbits.WPUB1 = 1; } while(0)
#define TURN_LEFT_ResetPullup()        do { WPUBbits.WPUB1 = 0; } while(0)
#define TURN_LEFT_SetAnalogMode()      do { ANSELBbits.ANSB1 = 1; } while(0)
#define TURN_LEFT_SetDigitalMode()     do { ANSELBbits.ANSB1 = 0; } while(0)

// get/set HIBEAM aliases
#define HIBEAM_TRIS                 TRISBbits.TRISB2
#define HIBEAM_LAT                  LATBbits.LATB2
#define HIBEAM_PORT                 PORTBbits.RB2
#define HIBEAM_WPU                  WPUBbits.WPUB2
#define HIBEAM_ANS                  ANSELBbits.ANSB2
#define HIBEAM_SetHigh()            do { LATBbits.LATB2 = 1; } while(0)
#define HIBEAM_SetLow()             do { LATBbits.LATB2 = 0; } while(0)
#define HIBEAM_Toggle()             do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define HIBEAM_GetValue()           PORTBbits.RB2
#define HIBEAM_SetDigitalInput()    do { TRISBbits.TRISB2 = 1; } while(0)
#define HIBEAM_SetDigitalOutput()   do { TRISBbits.TRISB2 = 0; } while(0)
#define HIBEAM_SetPullup()          do { WPUBbits.WPUB2 = 1; } while(0)
#define HIBEAM_ResetPullup()        do { WPUBbits.WPUB2 = 0; } while(0)
#define HIBEAM_SetAnalogMode()      do { ANSELBbits.ANSB2 = 1; } while(0)
#define HIBEAM_SetDigitalMode()     do { ANSELBbits.ANSB2 = 0; } while(0)

// get/set DRL aliases
#define DRL_TRIS                 TRISBbits.TRISB3
#define DRL_LAT                  LATBbits.LATB3
#define DRL_PORT                 PORTBbits.RB3
#define DRL_WPU                  WPUBbits.WPUB3
#define DRL_ANS                  ANSELBbits.ANSB3
#define DRL_SetHigh()            do { LATBbits.LATB3 = 1; } while(0)
#define DRL_SetLow()             do { LATBbits.LATB3 = 0; } while(0)
#define DRL_Toggle()             do { LATBbits.LATB3 = ~LATBbits.LATB3; } while(0)
#define DRL_GetValue()           PORTBbits.RB3
#define DRL_SetDigitalInput()    do { TRISBbits.TRISB3 = 1; } while(0)
#define DRL_SetDigitalOutput()   do { TRISBbits.TRISB3 = 0; } while(0)
#define DRL_SetPullup()          do { WPUBbits.WPUB3 = 1; } while(0)
#define DRL_ResetPullup()        do { WPUBbits.WPUB3 = 0; } while(0)
#define DRL_SetAnalogMode()      do { ANSELBbits.ANSB3 = 1; } while(0)
#define DRL_SetDigitalMode()     do { ANSELBbits.ANSB3 = 0; } while(0)

// get/set LOBEAM aliases
#define LOBEAM_TRIS                 TRISBbits.TRISB4
#define LOBEAM_LAT                  LATBbits.LATB4
#define LOBEAM_PORT                 PORTBbits.RB4
#define LOBEAM_WPU                  WPUBbits.WPUB4
#define LOBEAM_ANS                  ANSELBbits.ANSB4
#define LOBEAM_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define LOBEAM_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define LOBEAM_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define LOBEAM_GetValue()           PORTBbits.RB4
#define LOBEAM_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define LOBEAM_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define LOBEAM_SetPullup()          do { WPUBbits.WPUB4 = 1; } while(0)
#define LOBEAM_ResetPullup()        do { WPUBbits.WPUB4 = 0; } while(0)
#define LOBEAM_SetAnalogMode()      do { ANSELBbits.ANSB4 = 1; } while(0)
#define LOBEAM_SetDigitalMode()     do { ANSELBbits.ANSB4 = 0; } while(0)

// get/set POS aliases
#define POS_TRIS                 TRISBbits.TRISB5
#define POS_LAT                  LATBbits.LATB5
#define POS_PORT                 PORTBbits.RB5
#define POS_WPU                  WPUBbits.WPUB5
#define POS_ANS                  ANSELBbits.ANSB5
#define POS_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define POS_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define POS_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define POS_GetValue()           PORTBbits.RB5
#define POS_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define POS_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define POS_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define POS_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)
#define POS_SetAnalogMode()      do { ANSELBbits.ANSB5 = 1; } while(0)
#define POS_SetDigitalMode()     do { ANSELBbits.ANSB5 = 0; } while(0)

// get/set RC6 procedures
#define RC6_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define RC6_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define RC6_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define RC6_GetValue()              PORTCbits.RC6
#define RC6_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define RC6_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)

// get/set RC7 procedures
#define RC7_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define RC7_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define RC7_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define RC7_GetValue()              PORTCbits.RC7
#define RC7_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define RC7_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/