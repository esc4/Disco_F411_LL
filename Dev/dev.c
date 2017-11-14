/*
 * Dev.c
 *
 *  Created on: 6 nov. 2017
 *      Author: Quentin
 */

#include "dev.h"
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

		while(tick < 200)
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
	}
	INT1_LSM303_Flag = 0;
}

void I2C_Write(I2C_TypeDef* I2Cx, uint8_t slave_add, uint8_t* pBuffer, uint8_t size)
{
	TimeOut = TIME_OUT_INIT;

	LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_ACK);
	LL_I2C_GenerateStartCondition(I2Cx);

	/* Loop until SB flag is raised  */
	while(!LL_I2C_IsActiveFlag_SB(I2Cx))
	{
		TimeOutChecker(pTimeOut);
	}
	TimeOut = TIME_OUT_INIT;

	LL_I2C_TransmitData8(I2Cx, slave_add);

	/* Loop until ADDR flag is raised  */
	while(!LL_I2C_IsActiveFlag_ADDR(I2Cx))
	{
		TimeOutChecker(pTimeOut);
	}
	TimeOut = TIME_OUT_INIT;

	/* Clear ADDR flag value in ISR register */
	LL_I2C_ClearFlag_ADDR(I2Cx);

	while(size > 0)
	{
		while(!LL_I2C_IsActiveFlag_TXE(I2C1))
		{
			TimeOutChecker(pTimeOut);
		}
		TimeOut = TIME_OUT_INIT;

		/* TXE flag is cleared by writing data in TXDR register */
		LL_I2C_TransmitData8(I2C1, (*pBuffer++));
		size--;

		if(LL_I2C_IsActiveFlag_BTF(I2Cx) && (size != 0))
		{
			LL_I2C_TransmitData8(I2C1, (*pBuffer++));
			size--;
		}
	}

	while(LL_I2C_IsActiveFlag_BTF(I2Cx))
	{
		TimeOutChecker(pTimeOut);
	}
	TimeOut = TIME_OUT_INIT;

	LL_I2C_GenerateStopCondition(I2C1);
	//DEBUG_LED_Status_OK();
}

void I2C_Read(I2C_TypeDef * I2Cx, uint8_t slave_add, uint8_t reg, uint8_t * pBuffer, uint8_t size)
{
	TimeOut = TIME_OUT_INIT;
	uint8_t RemainingByte = size;
	uint8_t BufferIndex = 0;

	/***ACK enable & Start Generate***/
	LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_ACK);
	LL_I2C_GenerateStartCondition(I2Cx);

	while(!LL_I2C_IsActiveFlag_SB(I2Cx))
	{
		TimeOutChecker(pTimeOut);
	}
	TimeOut = TIME_OUT_INIT;

	/*** Send slave address in WRITE mode ***/
	LL_I2C_TransmitData8(I2Cx, slave_add | 0x00);

	while(!LL_I2C_IsActiveFlag_ADDR(I2Cx))
	{
		TimeOutChecker(pTimeOut);
	}
	TimeOut = TIME_OUT_INIT;

	LL_I2C_ClearFlag_ADDR(I2Cx);

	/*** Send reg to read ***/
	while(!LL_I2C_IsActiveFlag_TXE(I2Cx))
	{
		TimeOutChecker(pTimeOut);
	}
	TimeOut = TIME_OUT_INIT;

	LL_I2C_TransmitData8(I2Cx, reg);

	while(!LL_I2C_IsActiveFlag_TXE(I2Cx))
	{
		TimeOutChecker(pTimeOut);
	}
	TimeOut = TIME_OUT_INIT;

	/*** ReStart generate ***/
	LL_I2C_GenerateStartCondition(I2Cx);

	while(!LL_I2C_IsActiveFlag_SB(I2Cx))
	{
		TimeOutChecker(pTimeOut);
	}
	TimeOut = TIME_OUT_INIT;

	/*** Send slave address in READ mode ***/
	LL_I2C_TransmitData8(I2Cx, slave_add | 0x01);

	while(!LL_I2C_IsActiveFlag_ADDR(I2Cx))
	{
		TimeOutChecker(pTimeOut);
	}
	TimeOut = TIME_OUT_INIT;

	/*** Receive data ***/
	if(RemainingByte == 0)
	{
		LL_I2C_ClearFlag_ADDR(I2Cx);
		LL_I2C_GenerateStopCondition(I2Cx);
	}
	else if(RemainingByte == 1)
	{
		LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_NACK);
		LL_I2C_ClearFlag_ADDR(I2Cx);
		LL_I2C_GenerateStopCondition(I2Cx);
	}
	else if(RemainingByte == 2)
	{
		LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_NACK);
		LL_I2C_EnableBitPOS(I2Cx);
		LL_I2C_ClearFlag_ADDR(I2Cx);
	}
	else
	{
		LL_I2C_ClearFlag_ADDR(I2Cx);
	}

	while(RemainingByte > 0)
	{
		if(RemainingByte <=3)
		{
			if(RemainingByte == 1)
			{
				while(!LL_I2C_IsActiveFlag_RXNE(I2Cx))
				{
					TimeOutChecker(pTimeOut);
				}
				TimeOut = TIME_OUT_INIT;

				/* RXNE flag is cleared by reading data in DR register */
				pBuffer[BufferIndex++] = LL_I2C_ReceiveData8(I2Cx);
				RemainingByte --;
			}
			else if(RemainingByte == 2)
			{
				while(!LL_I2C_IsActiveFlag_BTF(I2Cx))
				{
					TimeOutChecker(pTimeOut);
				}
				TimeOut = TIME_OUT_INIT;

				LL_I2C_GenerateStopCondition(I2Cx);
				pBuffer[BufferIndex++] = LL_I2C_ReceiveData8(I2Cx);
				RemainingByte --;
				pBuffer[BufferIndex++] = LL_I2C_ReceiveData8(I2Cx);
				RemainingByte --;
			}
			else 	/* RemainingByte == 3 */
			{
				while(!LL_I2C_IsActiveFlag_BTF(I2Cx))
				{
					TimeOutChecker(pTimeOut);
				}
				TimeOut = TIME_OUT_INIT;

				LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_NACK);
				pBuffer[BufferIndex++] = LL_I2C_ReceiveData8(I2Cx);
				RemainingByte --;

				while(!LL_I2C_IsActiveFlag_BTF(I2Cx))
				{
					TimeOutChecker(pTimeOut);
				}
				TimeOut = TIME_OUT_INIT;

				LL_I2C_GenerateStopCondition(I2Cx);
				pBuffer[BufferIndex++] = LL_I2C_ReceiveData8(I2Cx);
				RemainingByte --;
				pBuffer[BufferIndex++] = LL_I2C_ReceiveData8(I2Cx);
				RemainingByte --;
			}
		}
		else 	/* RemainingByte > 3 */
		{
			while(!LL_I2C_IsActiveFlag_RXNE(I2Cx))
			{
				TimeOutChecker(pTimeOut);
			}
			TimeOut = TIME_OUT_INIT;

			pBuffer[BufferIndex++] = LL_I2C_ReceiveData8(I2Cx);
			RemainingByte --;

			if(LL_I2C_IsActiveFlag_BTF(I2Cx))
			{
				pBuffer[BufferIndex++] = LL_I2C_ReceiveData8(I2Cx);
				RemainingByte --;
			}
		}
	}
	//LED_Status_OK();
}
