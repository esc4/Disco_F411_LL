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
#include "drivers.h"
#include "LSM303DLHC.h"
#include "L3GD20.h"
#include "debug.h"

/* Global variables declaration */
extern uint8_t INT1_LSM303_Flag;
extern uint8_t INT_PB_Flag;

/*App functions*/
void SwitchMode(void);
void ModeExecution (void);
void DEV_LedInterlude1();
void DEV_LedInterlude2();
void DEV_LedClear();
void DEV_LedModeAcc();
void DEV_LedModeMag();



#endif /* DEV_H_ */
