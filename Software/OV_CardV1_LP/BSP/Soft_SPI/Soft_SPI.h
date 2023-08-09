#ifndef SOFT_SPI_H
#define SOFT_SPI_H

#include "stm32l0xx_hal.h"

/***************************************************
Mode0：CPOL=0，CPHA=0 常用  空闲SCK=0 数据采样是在上升沿
Mode1：CPOL=0，CPHA=1 空闲SCK=0 数据采样是在下降沿
Mode2：CPOL=1，CPHA=0 空闲SCK=1 数据采集是在下降沿
Mode3：CPOL=1，CPHA=1 常用 空闲SCK=1 数据采样是在上升沿
***************************************************/
 
#define SPI_MODE  	0
#define SPI_DELAY 	1 

typedef struct
{
	GPIO_TypeDef * SPI_MOSI_PORT;
	GPIO_TypeDef * SPI_MISO_PORT;
	GPIO_TypeDef * SPI_SCLK_PORT;
	GPIO_TypeDef * SPI_CS_PORT;
	
	uint16_t SPI_MOSI_PIN;
	uint16_t SPI_MISO_PIN;
	uint16_t SPI_SCLK_PIN;
	uint16_t SPI_CS_PIN;
	
}spi_bus_t;

void SPI_WriteByte(spi_bus_t * spi_bus, uint8_t addr, uint8_t dat);
uint8_t SPI_ReadByte(spi_bus_t * spi_bus, uint8_t addr);
void SPI_GpioInit(spi_bus_t * spi_bus);



#endif
