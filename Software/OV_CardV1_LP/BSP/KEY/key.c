#include "main.h"
#include "key.h"

#define KEY_U_PORT GPIOA
#define KEY_U_PIN GPIO_PIN_4
#define KEY_D_PORT GPIOA
#define KEY_D_PIN GPIO_PIN_3

#define KEY_U HAL_GPIO_ReadPin(KEY_U_PORT,KEY_U_PIN)
#define KEY_D HAL_GPIO_ReadPin(KEY_D_PORT,KEY_D_PIN)

void key_port_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin : PB8 */
  GPIO_InitStruct.Pin = KEY_U_PIN | KEY_D_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY_U_PORT, &GPIO_InitStruct);
}

unsigned char KeyScan( unsigned char mode )
{
	static uint8_t key_up = 1;
	unsigned char keyvalue;
	if(mode) key_up = 1;
	if( key_up && ( (!KEY_U) || (!KEY_D) ) )
	{
		HAL_Delay(3);//ensure the key is down
		if(!KEY_U) keyvalue = 1;
		else if(!KEY_D) keyvalue = 2;
		if(keyvalue) key_up = 0;
	}
	else
	{
		HAL_Delay(3);//ensure the key is up
		if( KEY_U && KEY_D )
			key_up = 1;
	}
	return keyvalue;
}
