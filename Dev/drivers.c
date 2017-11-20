/*
 * drivers.c
 *
 *  Created on: 18 nov. 2017
 *      Author: Quentin
 */

#include "drivers.h"
#include "dev.h"


void I2C_Write(I2C_TypeDef* I2Cx, uint8_t slave_add, uint8_t* pBuffer, uint8_t size)
{
	TimeOut = TIME_OUT_INIT;

	LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_ACK);
	LL_I2C_GenerateStartCondition(I2Cx);

	while(!LL_I2C_IsActiveFlag_SB(I2Cx))
	{
		TimeOutChecker(pTimeOut);
	}
	TimeOut = TIME_OUT_INIT;

	LL_I2C_TransmitData8(I2Cx, slave_add);

	while(!LL_I2C_IsActiveFlag_ADDR(I2Cx))
	{
		TimeOutChecker(pTimeOut);
	}
	TimeOut = TIME_OUT_INIT;

	LL_I2C_ClearFlag_ADDR(I2Cx);

	while(size > 0)
	{
		while(!LL_I2C_IsActiveFlag_TXE(I2Cx))
		{
			TimeOutChecker(pTimeOut);
		}
		TimeOut = TIME_OUT_INIT;

		LL_I2C_TransmitData8(I2C1, (*pBuffer++));
		size--;
/*
		if(LL_I2C_IsActiveFlag_BTF(I2Cx) && (size != 0))
		{
			LL_I2C_TransmitData8(I2C1, (*pBuffer));
			pBuffer++;
			size--;
		}
		*/
	}
	while(!LL_I2C_IsActiveFlag_TXE(I2Cx))
	{
		TimeOutChecker(pTimeOut);
	}
	TimeOut = TIME_OUT_INIT;

	while(LL_I2C_IsActiveFlag_BTF(I2Cx))
	{
		TimeOutChecker(pTimeOut);
	}
	TimeOut = TIME_OUT_INIT;

	LL_I2C_GenerateStopCondition(I2Cx);
	//DEBUG_LED_Status_OK();
}

void I2C_Read(I2C_TypeDef* I2Cx, uint8_t slave_add, uint8_t reg, uint8_t* pBuffer, uint8_t size)
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

void SPI_FullDuplex(SPI_TypeDef* SPIx, GPIO_TypeDef* NSS_port, uint16_t NSS_GPIO, uint8_t* pTxBuffer, uint8_t txSize, uint8_t* pRxBuffer, uint8_t rxSize)
{
	LL_SPI_SetMode(SPIx, LL_SPI_MODE_MASTER);
	LL_GPIO_ResetOutputPin(NSS_port, NSS_GPIO);
	while(rxSize > 0 || txSize > 0)
	  {
	    /* Check TXE flag to transmit data */
	    if(( LL_SPI_IsActiveFlag_TXE(SPIx)) && (txSize > 0))
	    {
	      /* Transmit 16bit Data */
	      LL_SPI_TransmitData8(SPIx, *pTxBuffer++);
	      txSize--;
	    }
	    /* Check RXE flag */
	    if(LL_SPI_IsActiveFlag_RXNE(SPIx))
	    {
	      /* Receive 16bit Data */
	      *pRxBuffer++ = LL_SPI_ReceiveData8(SPIx);
	      rxSize--;
	    }
	  }
	  LL_GPIO_SetOutputPin(NSS_port, NSS_GPIO);
}

