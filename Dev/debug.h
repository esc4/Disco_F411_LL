/*
 * debug.h
 *
 *  Created on: 10 juin 2018
 *      Author: Quentin
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include "dev.h"
#define TIME_OUT_INIT 10U


/* Helper functions*/
void TimeOutChecker(uint8_t*);
void ErrorHandler(void);
void DEBUG_LED_Status_OK(void);
int16_t ConvertTwoComplement(uint16_t);
uint8_t CheckBit(uint8_t, uint8_t);


#endif /* DEBUG_H_ */
