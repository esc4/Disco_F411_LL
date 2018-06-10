/*
 * debug.c
 *
 *  Created on: 10 juin 2018
 *      Author: Quentin
 */

#include "debug.h"

void TimeOutChecker(uint8_t* timeout_checker)
{
	/* Check Systick counter flag to decrement the time-out value */
	if (LL_SYSTICK_IsActiveCounterFlag())
		{
			if((*timeout_checker)-- == 0)
			{
			  ErrorHandler();
			}
		}
}

void ErrorHandler()
{
	DEV_LedClear();
	while(1)
	{
		LL_GPIO_TogglePin(GPIOD, LD5_Pin);
		LL_mDelay(100);
	}
}

void DEBUG_LED_Status_OK()
{
	LL_GPIO_SetOutputPin(GPIOD, LD4_Pin);
	LL_mDelay(500);
	LL_GPIO_ResetOutputPin(GPIOD, LD4_Pin);
}

int16_t ConvertTwoComplement(uint16_t input)
{
	int16_t converted = 0;
	if (input > 2048)
	{
		converted = -((~input)+1);
	}
	else if(input == 2048)
	{
		converted = -2048;
	}
	else
	{
		converted = input;
	}

	return converted;
}

uint8_t CheckBit(uint8_t bitInt, uint8_t pos)
{
	return (bitInt & (1<<pos));
}
