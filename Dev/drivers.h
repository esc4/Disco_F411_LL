/*
 * drivers.h
 *
 *  Created on: 18 nov. 2017
 *      Author: Quentin
 */

#ifndef DRIVERS_H_
#define DRIVERS_H_

#include "dev.h"

void I2C_Write(I2C_TypeDef*,uint8_t, uint8_t*, uint8_t);
void I2C_Read(I2C_TypeDef*,uint8_t, uint8_t,uint8_t*, uint8_t);

void SPI_FullDuplex(SPI_TypeDef* SPIx, GPIO_TypeDef* NSS_port, uint16_t NSS_GPIO, uint8_t* pTxBuffer, uint8_t txSize, uint8_t* pRxBuffer, uint8_t rxSize);



#endif /* DRIVERS_H_ */


