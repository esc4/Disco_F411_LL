/*
 * Dev.c
 *
 *  Created on: 6 nov. 2017
 *      Author: Quentin
 */

#include "Dev.h"

void flashLed(void)
{

	LL_GPIO_TogglePin(GPIOD, LL_GPIO_PIN_15);
	LL_mDelay(100);
}
