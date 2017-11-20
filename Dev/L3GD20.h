/*
 * L3GD20.h
 *
 *  Created on: 19 nov. 2017
 *      Author: Quentin
 */

#ifndef L3GD20_H_
#define L3GD20_H_

#include "dev.h"

#define CTRL_REG1 0x20
#define OUT_X_L 0x28

void L3GD20_Conf();
void L3GD20_GetXYZ(int16_t* pX, int16_t* pY, int16_t* pZ);

#endif /* L3GD20_H_ */
