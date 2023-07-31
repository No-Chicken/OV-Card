#include "Soft_SPI.h" 
 
void SPI_GpioInit(spi_bus_t * spi_bus)
{
  /*GPIO初始化*/
  /*建议上拉模式*/
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	//clk_enable
	//spi_bus->CLK_ENABLE();
	
	//CS	
  GPIO_InitStructure.Pin = spi_bus->SPI_CS_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(spi_bus->SPI_CS_PORT, &GPIO_InitStructure);
		
  HAL_GPIO_WritePin(spi_bus->SPI_CS_PORT,spi_bus->SPI_CS_PIN,GPIO_PIN_SET);
	//CLK
	GPIO_InitStructure.Pin = spi_bus->SPI_SCLK_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(spi_bus->SPI_SCLK_PORT, &GPIO_InitStructure);
		
	HAL_GPIO_WritePin(spi_bus->SPI_SCLK_PORT,spi_bus->SPI_SCLK_PIN,GPIO_PIN_RESET);
	//MOSI
	GPIO_InitStructure.Pin = spi_bus->SPI_MOSI_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(spi_bus->SPI_MOSI_PORT, &GPIO_InitStructure);
		
	HAL_GPIO_WritePin(spi_bus->SPI_MOSI_PORT,spi_bus->SPI_MOSI_PIN,GPIO_PIN_SET);
	//MISO
	GPIO_InitStructure.Pin = spi_bus->SPI_MISO_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(spi_bus->SPI_MISO_PORT, &GPIO_InitStructure);
	
}

void SPI_CS_SET(spi_bus_t * spi_bus, uint8_t state)
{
	if (state)
    {
        spi_bus->SPI_CS_PORT->BSRR |= spi_bus->SPI_CS_PIN;
    }
    else
    {
        spi_bus->SPI_CS_PORT->BSRR = (uint32_t)spi_bus->SPI_CS_PIN << 16U;
    }
}

void SPI_SCLK_SET(spi_bus_t * spi_bus, uint8_t state)
{
	if (state)
    {
        spi_bus->SPI_SCLK_PORT->BSRR |= spi_bus->SPI_SCLK_PIN;
    }
    else
    {
        spi_bus->SPI_SCLK_PORT->BSRR = (uint32_t)spi_bus->SPI_SCLK_PIN << 16U;
    }
}

uint8_t SPI_MISO_Read(spi_bus_t * spi_bus)
{
	if(HAL_GPIO_ReadPin(spi_bus->SPI_MISO_PORT, spi_bus->SPI_MISO_PIN) == GPIO_PIN_SET)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void SPI_MOSI_SET(spi_bus_t * spi_bus, uint8_t state)
{
	if (state)
    {
        spi_bus->SPI_MOSI_PORT->BSRR |= spi_bus->SPI_MOSI_PIN;
    }
    else
    {
        spi_bus->SPI_MOSI_PORT->BSRR = (uint32_t)spi_bus->SPI_MOSI_PIN << 16U;
    }
}

void SPI_WriteByte(spi_bus_t * spi_bus, uint8_t addr, uint8_t dat)
{
	uint8_t i;
	uint8_t temp = addr;
	SPI_CS_SET(spi_bus,0);
#if SPI_MODE==0
	for(i=0;i<8;i++)
	{
		SPI_SCLK_SET(spi_bus,0);
		if(temp&0x80)
			SPI_MOSI_SET(spi_bus,1);
		else
			SPI_MOSI_SET(spi_bus,0);
		temp<<=1;
		SPI_SCLK_SET(spi_bus,1);
	}
	SPI_SCLK_SET(spi_bus,0);

	temp = dat;
	for(i=0;i<8;i++)
	{
		SPI_SCLK_SET(spi_bus,0);
		if(temp&0x80)
			SPI_MOSI_SET(spi_bus,1);
		else
			SPI_MOSI_SET(spi_bus,0);
		temp<<=1;
		SPI_SCLK_SET(spi_bus,1);
	}
	SPI_SCLK_SET(spi_bus,0);
	SPI_CS_SET(spi_bus,1);
#elif SPI_MODE==1

#elif SPI_MODE==2

#elif SPI_MODE==3

#endif
}

uint8_t SPI_ReadByte(spi_bus_t * spi_bus, uint8_t addr)
{
	uint8_t i,j;
	uint8_t temp = addr;
	SPI_CS_SET(spi_bus,0);
#if SPI_MODE==0
	for(i=0;i<8;i++)
	{
		SPI_SCLK_SET(spi_bus,0);
		if(temp&0x80)
			SPI_MOSI_SET(spi_bus,1);
		else
			SPI_MOSI_SET(spi_bus,0);
		temp<<=1;
		SPI_SCLK_SET(spi_bus,1);
	}
	SPI_SCLK_SET(spi_bus,0);
	
	temp = 0;
	for(j=0;j<8;j++)
	{
		SPI_SCLK_SET(spi_bus,0);
		temp<<=1;
		if(SPI_MISO_Read(spi_bus))
			temp|=0x01;
		else
			temp&=0xfe;
		SPI_SCLK_SET(spi_bus,1);
	}
	SPI_SCLK_SET(spi_bus,0);
	SPI_CS_SET(spi_bus,1);
	return temp;
#elif SPI_MODE==1

#elif SPI_MODE==2

#elif SPI_MODE==3

#endif
}

