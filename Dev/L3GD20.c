/*
 * L3GD20.c
 *
 *  Created on: 19 nov. 2017
 *      Author: Quentin
 */

#include "L3GD20.h"
#include "dev.h"


void L3GD20_Conf()
{
	uint8_t TxBuffer[2] = {
			CTRL_REG1,
			0x0F
	};

	SPI_Write(SPI1, CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, TxBuffer, 2);
}

void L3GD20_GetXYZ(int16_t* pX, int16_t* pY, int16_t* pZ)
{
	uint8_t reg = OUT_X_L | 0xC0;
	uint8_t RxBuffer[6] = {0,0,0,0,0,0};

	SPI_ReadReg(SPI1, CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, reg, RxBuffer, 3);

	*pX = (RxBuffer[1]<<8) | RxBuffer[0] ;
	//*pX /= 16;
	*pY = (RxBuffer[3]<<8) | RxBuffer[2] ;
	//*pY /= 16;
	*pZ = (RxBuffer[5]<<8) | RxBuffer[4] ;
	//*pZ /= 16;
}

void L3GD20_LedMode()
{

}
