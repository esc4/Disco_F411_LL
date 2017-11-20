/*
 * Dev.c
 *
 *  Created on: 6 nov. 2017
 *      Author: Quentin
 */

#include "dev.h"
#include "drivers.h"
#include "LSM303DLHC.h"

userMode CurrentMode = Acc;
uint8_t INT1_LSM303_Flag = 0;
uint8_t INT_PB_Flag = 0;
uint8_t TimeOut = TIME_OUT_INIT;
uint8_t* pTimeOut = &TimeOut;

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

void DEV_LedInterlude1()
{
	for(int i=0; i<4; i++)
	{
	LL_GPIO_SetOutputPin(GPIOD,LD3_Pin);
	LL_mDelay(100);
	LL_GPIO_ResetOutputPin(GPIOD,LD3_Pin);
	LL_GPIO_SetOutputPin(GPIOD,LD5_Pin);
	LL_mDelay(100);
	LL_GPIO_ResetOutputPin(GPIOD,LD5_Pin);
	LL_GPIO_SetOutputPin(GPIOD,LD6_Pin);
	LL_mDelay(100);
	LL_GPIO_ResetOutputPin(GPIOD,LD6_Pin);
	LL_GPIO_SetOutputPin(GPIOD,LD4_Pin);
	LL_mDelay(100);
	LL_GPIO_ResetOutputPin(GPIOD,LD4_Pin);
	}
}
void DEV_LedInterlude2()
{
	for (int i=0; i<3; i++)
	{
	LL_GPIO_SetOutputPin(GPIOD,LD3_Pin);
	LL_GPIO_SetOutputPin(GPIOD,LD4_Pin);
	LL_GPIO_SetOutputPin(GPIOD,LD5_Pin);
	LL_GPIO_SetOutputPin(GPIOD,LD6_Pin);
	LL_mDelay(100);
	LL_GPIO_ResetOutputPin(GPIOD,LD3_Pin);
	LL_GPIO_ResetOutputPin(GPIOD,LD4_Pin);
	LL_GPIO_ResetOutputPin(GPIOD,LD5_Pin);
	LL_GPIO_ResetOutputPin(GPIOD,LD6_Pin);
	LL_mDelay(100);
	}
}
void DEV_LedClear(void)
{
	LL_GPIO_ResetOutputPin(GPIOD,LD3_Pin);
	LL_GPIO_ResetOutputPin(GPIOD,LD4_Pin);
	LL_GPIO_ResetOutputPin(GPIOD,LD5_Pin);
	LL_GPIO_ResetOutputPin(GPIOD,LD6_Pin);
}
void DEV_LedModeAcc(void)
{
	for (int i =0; i<3; i++)
		{
		LL_GPIO_SetOutputPin(GPIOD, LD4_Pin);
		LL_GPIO_SetOutputPin(GPIOD, LD5_Pin);
		LL_GPIO_SetOutputPin(GPIOD, LD6_Pin);
		LL_mDelay(100);
		LL_GPIO_ResetOutputPin(GPIOD,LD4_Pin);
		LL_GPIO_ResetOutputPin(GPIOD,LD5_Pin);
		LL_GPIO_ResetOutputPin(GPIOD,LD6_Pin);
		LL_mDelay(100);
		}
}
void DEV_LedModeMag(void)
{
	for (int i =0; i<3; i++)
		{
		LL_GPIO_SetOutputPin(GPIOD, LD4_Pin);
		LL_GPIO_SetOutputPin(GPIOD, LD5_Pin);
		LL_GPIO_SetOutputPin(GPIOD, LD3_Pin);
		LL_mDelay(100);
		LL_GPIO_ResetOutputPin(GPIOD,LD4_Pin);
		LL_GPIO_ResetOutputPin(GPIOD,LD5_Pin);
		LL_GPIO_ResetOutputPin(GPIOD,LD3_Pin);
		LL_mDelay(100);
		}
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

void ModeExecution(void)
{
	if(INT1_LSM303_Flag)
	{
		uint32_t tick = 0;

		while(tick < 15)
		{
			switch (CurrentMode)
			{
				case Acc :
				{
					ACC_LedMode();
					break;
				}
				case Compass :
				{
					MAG_CompassMode();
					break;
				}
				default :
					DEV_LedInterlude2();
			}
			if(LL_SYSTICK_IsActiveCounterFlag())
			{
				tick++;
			}
		}
		DEV_LedClear();
		DEV_LedInterlude2();
		INT1_LSM303_Flag = 0;
	}
}



