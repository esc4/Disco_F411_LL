/*
 * Dev.c
 *
 *  Created on: 6 nov. 2017
 *      Author: Quentin
 */

#include "dev.h"
#include "drivers.h"
#include "LSM303DLHC.h"

/* Typedef type defn */
typedef enum userMode {
	Acc,
	Compass
} userMode;

/* Global variable defn */
userMode enumCurrentMode = Acc;
uint8_t INT1_LSM303_Flag = 0;
uint8_t INT_PB_Flag = 0;

void SwitchMode()
{
	enumCurrentMode = !enumCurrentMode;
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

void ModeExecution(void)
{
	if(INT1_LSM303_Flag)
	{
		uint32_t tick = 0;

		while(tick < 15)
		{
			switch (enumCurrentMode)
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



