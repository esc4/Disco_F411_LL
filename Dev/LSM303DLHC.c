#include "LSM303DLHC.h"

/**** Accelerometer ****/
void ACC_Conf()
{
	uint8_t TxBuffer1[8] = {
			CTRL_REG1_A | 0x80,
			0x57,				/*Reg : CTRL_REG1_A (ODR 100Hz + EN all axes) */
			0x00,				/*Reg : CTRL_REG2_A (no filter) */
			0x40,				/*Reg : CTRL_REG3_A (I1_AOI) */
			0x28,				/*Reg : CTRL_REG4_A (continuous update, Little Endian, FS : +-8g (4mg/LSB), HR) */
			0x00,				/*Reg : CTRL_REG5_A (0x04 = D4D)*/
			0x40,				/*Reg : CTRL_REG6_A (INT1 on PAD2)*/
			0x00				/*Reg : REFERENCE_A (Reference value for interrupt generation) */
	};

	uint8_t TxBuffer2[2] = {
			INT1_CFG_A,
			0x0A				/*Reg : INT1_CFG_A (OR detection, X|Y > TH) */
	};

	uint8_t TxBuffer3[3] = {
			INT1_THS_A |0x80,
			0x02,				/*Reg : INT1_THS_A (2LSB*62mg/LSB = 124mg) */
			0x0A				/*Reg : INT1_DURATION_A ((1/100Hz)*10 = 100ms*/
	};

	I2C_Write(I2C1, ACC_ADD, TxBuffer1, 8);
	I2C_Write(I2C1, ACC_ADD, TxBuffer2, 2);
	I2C_Write(I2C1, ACC_ADD, TxBuffer3, 3);
}

void ACC_GetXYZ(int16_t* pX, int16_t* pY, int16_t* pZ)
{
	uint8_t RxBufferACC[6] = {0,0,0,0,0,0};
	I2C_Read(I2C1, ACC_ADD, OUT_X_L_A |0x80,RxBufferACC, 6);
	//12 bits data outputs
	*pX = (RxBufferACC[1]<<8) | RxBufferACC[0] ;
	*pX /= 16;
	*pY = (RxBufferACC[3]<<8) | RxBufferACC[2] ;
	*pY /= 16;
	*pZ = (RxBufferACC[5]<<8) | RxBufferACC[4] ;
	*pZ /= 16;
}

void ACC_LedMode()
{
	int16_t x = 0;
	int16_t y = 0;
	int16_t z = 0;
	ACC_GetXYZ(&x,&y,&z);
	if(x < -20)
	{
	LL_GPIO_SetOutputPin(GPIOD,LD6_Pin);
	LL_GPIO_ResetOutputPin(GPIOD,LD3_Pin);
	}
	else if(x > 20)
	{
	LL_GPIO_SetOutputPin(GPIOD,LD3_Pin);
	LL_GPIO_ResetOutputPin(GPIOD,LD6_Pin);
	}
	else
	{
	LL_GPIO_ResetOutputPin(GPIOD,LD6_Pin);
	LL_GPIO_ResetOutputPin(GPIOD,LD3_Pin);
	}
//Y axe
	if(y < -20)
	{
	LL_GPIO_SetOutputPin(GPIOD,LD5_Pin);
	LL_GPIO_ResetOutputPin(GPIOD,LD4_Pin);
	}
	else if(y > 20)
	{
	LL_GPIO_SetOutputPin(GPIOD,LD4_Pin);
	LL_GPIO_ResetOutputPin(GPIOD,LD5_Pin);
	}
	else
	{
	LL_GPIO_ResetOutputPin(GPIOD,LD4_Pin);
	LL_GPIO_ResetOutputPin(GPIOD,LD5_Pin);
	}
}

/**** Magnetometer ****/
void MAG_Conf()
{
	uint8_t TxBufferMAGconf[4] = {
			CRA_REG_M,			/*Auto increment*/
			0x90,				/* [Temp enable, ODR : 15Hz] [Range +/- 1.3 Gauss] [Continuous-conversion mode] */
			0x20,
			0x00
	};

	I2C_Write(I2C1, MAG_ADD, TxBufferMAGconf, 4);
}

void MAG_GetXYZ(int16_t* pX, int16_t* pY, int16_t* pZ)
{
	uint8_t RxBufferMAG[6] = {0,0,0,0,0,0};
	I2C_Read(I2C1, MAG_ADD, OUT_X_H_M, RxBufferMAG, 6);
	// 16 bits data outputs ?? Seems to.
	*pX = (RxBufferMAG[0]<<8) | RxBufferMAG[1] ;	// Divide by 1100 to convert in Gauss
	*pZ = (RxBufferMAG[2]<<8) | RxBufferMAG[3] ;	// Divide by 980 to convert in Gauss
	*pY = (RxBufferMAG[4]<<8) | RxBufferMAG[5] ;	// Divide by 1100 to convert in Gauss
	LL_mDelay(1);
}

void MAG_GetTemp(int16_t* pT)
{
	uint8_t RxBufferTemp[2] = {0,0};
	I2C_Read(I2C1, MAG_ADD, TEMP_OUT_H_M, RxBufferTemp, 2);
	*pT = (RxBufferTemp[0]<<8) | RxBufferTemp[1] ;
	*pT /= 16;	 //12 Bits outputs
	*pT /= 8;	//8 LSB/°C (not calibrated)
}

void MAG_CompassMode()
{
	int16_t x,y,z = 0;
	MAG_GetXYZ(&x,&y,&z);

	float magDeclination = 1.483;	//Magnetic declination east/west depends on actual position
	float angle = atan2f((float)y, (float)x)*180/3.1415;
	angle += magDeclination;
	uint8_t accu = 10;	//Window detection +/- accu (°)

	if(angle < accu && angle > -accu)
	{
	LL_GPIO_SetOutputPin(GPIOD,LD3_Pin);
	LL_GPIO_ResetOutputPin(GPIOD,LD4_Pin);
	LL_GPIO_ResetOutputPin(GPIOD,LD5_Pin);
	LL_GPIO_ResetOutputPin(GPIOD,LD6_Pin);
	}
	else if (angle < -(90-accu) && angle > -(90+accu) )
	{
	LL_GPIO_ResetOutputPin(GPIOD,LD3_Pin);
	LL_GPIO_ResetOutputPin(GPIOD,LD4_Pin);
	LL_GPIO_SetOutputPin(GPIOD,LD5_Pin);
	LL_GPIO_ResetOutputPin(GPIOD,LD6_Pin);
	}
	else if(angle < -(180-accu) && angle > -(180+accu) )
	{
	LL_GPIO_ResetOutputPin(GPIOD,LD3_Pin);
	LL_GPIO_ResetOutputPin(GPIOD,LD4_Pin);
	LL_GPIO_ResetOutputPin(GPIOD,LD5_Pin);
	LL_GPIO_SetOutputPin(GPIOD,LD6_Pin);
	}
	else if (angle > (90-accu) && angle < (90+accu) )
	{
	LL_GPIO_ResetOutputPin(GPIOD,LD3_Pin);
	LL_GPIO_SetOutputPin(GPIOD,LD4_Pin);
	LL_GPIO_ResetOutputPin(GPIOD,LD5_Pin);
	LL_GPIO_ResetOutputPin(GPIOD,LD6_Pin);
	}
	else
	{
	LL_GPIO_ResetOutputPin(GPIOD,LD3_Pin);
	LL_GPIO_ResetOutputPin(GPIOD,LD4_Pin);
	LL_GPIO_ResetOutputPin(GPIOD,LD5_Pin);
	LL_GPIO_ResetOutputPin(GPIOD,LD6_Pin);
	}
}
