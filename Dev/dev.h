/*
 * dev.h
 *
 *  Created on: 6 nov. 2017
 *      Author: Quentin
 */

#ifndef DEV_H_
#define DEV_H_

#include "main.h"
#include "i2c.h"

#include "LSM303DLHC.h"


#define TIME_OUT_INIT 10U

typedef enum userMode {
	Acc,
	Compass
} userMode;

/*Globals*/
extern uint8_t INT1_LSM303_Flag;
extern uint8_t INT_PB_Flag;
extern userMode CurrentMode;
extern uint8_t* pTimeOut;

/*App functions*/
void ModeExecution (void);
void DEV_LedInterlude1();
void DEV_LedInterlude2();
void DEV_LedClear();
void DEV_LedModeAcc();
void DEV_LedModeMag();

/*Driver functions*/
void I2C_Write(I2C_TypeDef*,uint8_t, uint8_t*, uint8_t);
void I2C_Read(I2C_TypeDef*,uint8_t, uint8_t,uint8_t*, uint8_t);

/* Helper functions*/
void ErrorHandler(void);
void DEBUG_LED_Status_OK(void);
int16_t ConvertTwoComplement(uint16_t);
uint8_t CheckBit(uint8_t, uint8_t);

void DEV_I2CReadRegister(uint16_t, uint16_t, uint8_t *);

#endif /* DEV_H_ */
