#include <string.h>
#include "main.h"
#include "EEPROM.h"

#define EEPROM_CHECK_NUM 2

//Byte write
void EEPROM_WRITE(uint16_t BiasAddress, uint8_t *Data, uint16_t len)
{
	uint16_t i;
	HAL_StatusTypeDef status = HAL_OK;

	HAL_FLASHEx_DATAEEPROM_Unlock();
	for(i=0;i<len;i++)
	{
		status +=HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, EEPROM_BASE_ADDR+BiasAddress+i, *Data);
		Data++;
	}
	HAL_FLASHEx_DATAEEPROM_Lock();
}

//Byte read
void EEPROM_READ(uint16_t BiasAddress,uint8_t *Buffer,uint16_t Len)
{
	uint8_t *wAddr;
	wAddr=(uint8_t *)(EEPROM_BASE_ADDR+BiasAddress);
	while(Len--)
	{
		*Buffer++=*wAddr++;
	}
}

//Byte write with check
HAL_StatusTypeDef EEPROM_Write_W_CHECK(uint16_t BiasAddress, uint8_t *Data, uint16_t len)
{
	uint8_t buff[len];
	uint16_t i;
	for (i=0;i<EEPROM_CHECK_NUM;i++)
	{
		EEPROM_WRITE(BiasAddress, Data, len);
		EEPROM_READ(BiasAddress, buff, len);
		if (memcmp(Data, buff, len)==0)
		{
			return HAL_OK;
		}
	}

	return HAL_ERROR;
}

//Byte read with check
HAL_StatusTypeDef EEPROM_Read_W_CHECK(uint16_t BiasAddress, uint8_t *Data, uint16_t len)
{
	uint8_t buff0[len];
	uint8_t buff1[len];
	uint16_t i;
	for (i=0;i<EEPROM_CHECK_NUM;i++)
	{
		EEPROM_READ(BiasAddress, buff0, len);
		EEPROM_READ(BiasAddress, buff1, len);

		if (memcmp(buff0, buff1, len)==0)
		{
			memcpy(Data, buff0, len);
			return HAL_OK;
		}
	}

	return HAL_ERROR;
}

//Byte clear with check
HAL_StatusTypeDef EEPROM_Clear_W_CHECK(uint16_t BiasAddress,uint16_t len)
{
	
	uint8_t state = 1;
	uint8_t buff[len];
	memset(buff,0,len);
	state = EEPROM_Write_W_CHECK(BiasAddress,buff,len);
	
	return state;
	
}
	
	