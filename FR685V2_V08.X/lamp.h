/* 
 * File:lamp.h
 * Author: yiming
 * Comments:lamp active
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LAMP_H
#define	LAMP_H

#include <xc.h> // include processor files - each processor file is guarded.  

extern void lamp(uint8_t* tempRxData);
extern void feedbacksignal(uint8_t *tempRxData);




#endif	/* LAMP_H */

