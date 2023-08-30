/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "RC522.h"
#include "key.h"
#include "EEPROM.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint8_t RC522_ERRO = 1;
uint8_t EEPROM_ERRO = 1;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
unsigned char snr, TagType[2], SelectedSnr[4], DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	
	uint8_t status;
	uint8_t read_buf[16];
	uint8_t data_addr;
	uint8_t cuid_flag = 1;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
	HAL_GPIO_WritePin(GPIOA,LED6_Pin|LED5_Pin|LED4_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,LED1_Pin|LED2_Pin|LED3_Pin,GPIO_PIN_RESET);
	
	key_port_init();	
	
	RC522_Port_Init();
	RC522_PowerOff();
	
	if(!EEPROM_Read_W_CHECK(0x00,read_buf,2))
	{
		if(read_buf[0] == 0x55 && read_buf[1] == 0xAA)
		{
			EEPROM_ERRO = 0;
			EEPROM_Read_W_CHECK(DATA_ADDR_ADDR,&data_addr,1);
		}
		else//first use
		{
			read_buf[0] = 0x55;
			read_buf[1] = 0xAA;
			read_buf[2] = 0x10;
			EEPROM_ERRO = EEPROM_Write_W_CHECK(0,read_buf,3);
		}
	}
	
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
		/*****************Key Scan*****************/
		uint8_t key;
		key = KeyScan(0);
		if(key == 1)
		{
			data_addr -= 0x10;//data_addr --, change to previous UID
			if(data_addr<0x10)
				data_addr = 0x10*ID_NUM;
			HAL_GPIO_WritePin(UID_Check_GPIO_Port,UID_Check_Pin,GPIO_PIN_SET);
			if(!EEPROM_ERRO)
			{
				EEPROM_Write_W_CHECK(DATA_ADDR_ADDR,&data_addr,1);
			}
			cuid_flag = 1;
			HAL_GPIO_WritePin(GPIOA,LED6_Pin|LED5_Pin|LED4_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB,LED1_Pin|LED2_Pin|LED3_Pin,GPIO_PIN_RESET);
		}
		else if(key == 2)
		{
			data_addr += 0x10;//data_addr add, change to next UID
			if(data_addr>(0x10*ID_NUM))
				data_addr = 0x10;
			HAL_GPIO_WritePin(UID_Check_GPIO_Port,UID_Check_Pin,GPIO_PIN_SET);
			if(!EEPROM_ERRO)
			{
				EEPROM_Write_W_CHECK(DATA_ADDR_ADDR,&data_addr,1);
			}
			cuid_flag = 1;
			HAL_GPIO_WritePin(GPIOA,LED6_Pin|LED5_Pin|LED4_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB,LED1_Pin|LED2_Pin|LED3_Pin,GPIO_PIN_RESET);
		}
		/******************************************/
		
		
		/*********check the ID Change and turn the LED**************/
		uint8_t temp = data_addr>>4 & 0x0f;
		//temp = temp>>4 & 0x0F;
		switch(temp)
		{
			case 1:
				HAL_GPIO_WritePin(GPIOB,LED1_Pin,GPIO_PIN_SET);
				break;
			case 2:
				HAL_GPIO_WritePin(GPIOB,LED2_Pin,GPIO_PIN_SET);
				break;
			case 3:
				HAL_GPIO_WritePin(GPIOB,LED3_Pin,GPIO_PIN_SET);
				break;
			case 4:
				HAL_GPIO_WritePin(GPIOA,LED4_Pin,GPIO_PIN_SET);
				break;
			case 5:
				HAL_GPIO_WritePin(GPIOA,LED5_Pin,GPIO_PIN_SET);
				break;
			case 6:
				HAL_GPIO_WritePin(GPIOA,LED6_Pin,GPIO_PIN_SET);
				break;
		}
		/***********************************************************/
		
		
		/*********check the CUID card is selected or not************/
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_8))//the UID card is selected
		{
			if(cuid_flag)
			{
				RC522_ERRO = RC522_Init();
				/*************write the cuid************/
				status= PcdRequest(REQ_ALL,TagType);
				if(!status)
				{
					status = PcdAnticoll(SelectedSnr);
					if(!status)
					{
						status=PcdSelect(SelectedSnr);
						if(!status)
						{
							snr = 0;  //sector 0
							status = PcdAuthState(KEYA, (snr*4+3), DefaultKey, SelectedSnr);
							{
								if(!status)
								{
									if(!EEPROM_ERRO)
									{
										EEPROM_Read_W_CHECK(data_addr,read_buf,16);
									}
									status = PcdWrite((snr*4+0), read_buf);  
									if(!status)
									{
										HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);
										cuid_flag = 0;
										//WaitCardOff();
										HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET);
										RC522_PowerOff();
									}
								}
							}
						}
					}
				}
				/***************************************/
			}
		}
		else	//the UID card is disabled
		{
			/***********search the card***************/
			if(cuid_flag)
			{
				RC522_ERRO = RC522_Init();
				status= PcdRequest(REQ_ALL,TagType);
				if(!status)
				{
					status = PcdAnticoll(SelectedSnr);
					if(!status)
					{
						status=PcdSelect(SelectedSnr);
						if(!status)
						{
							snr = 0;  //sector 0
							status = PcdAuthState(KEYA, (snr*4+3), DefaultKey, SelectedSnr);
							{
								if(!status)
								{
									status = PcdRead((snr*4+0), read_buf);  
									if(!status)
									{
										HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);
										//WaitCardOff();
										if(!EEPROM_ERRO)
										{
											EEPROM_Write_W_CHECK(data_addr,read_buf,16);
										}
										HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET);
										//return 0;
										RC522_PowerOff();
										cuid_flag = 0;
									}
								}
							}
						}
					}
				}
			}
			/****************************************/
		}
		/***********************************************************/

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
