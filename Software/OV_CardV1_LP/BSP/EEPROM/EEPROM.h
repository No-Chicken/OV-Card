#ifndef __EEPROM_H__
#define __EEPROM_H__

#include "main.h"

#define EEPROM_BASE_ADDR	0x08080000
#define EEPROM_BYTE_SIZE	0x03FF

#define DATA_ADDR_ADDR	0x02
#define ID_NUM 6						//6 cards

void EEPROM_WRITE(uint16_t BiasAddress, uint8_t *Data, uint16_t len);
void EEPROM_READ(uint16_t BiasAddress,uint8_t *Buffer,uint16_t Len);
HAL_StatusTypeDef EEPROM_Write_W_CHECK(uint16_t BiasAddress, uint8_t *Data, uint16_t len);
HAL_StatusTypeDef EEPROM_Read_W_CHECK(uint16_t BiasAddress, uint8_t *Data, uint16_t len);
HAL_StatusTypeDef EEPROM_Clear_W_CHECK(uint16_t BiasAddress,uint16_t len);

#endif
