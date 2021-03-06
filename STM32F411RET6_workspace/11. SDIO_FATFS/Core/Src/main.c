/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "dma.h"
#include "fatfs.h"
#include "sdio.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define STR_SIZE 40
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
char str[STR_SIZE];
uint8_t flag1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void uart_buffer_clean(char *str, uint32_t cnt);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  MX_USART1_UART_Init();
  MX_DMA_Init();
  MX_FATFS_Init();
  MX_SDIO_SD_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart1, str, STR_SIZE);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  BYTE buf[32] = "Hello world";	// file copy buffer
  uint32_t bw, br;

  // 1. FS Initialization("0:/"drive allocation)
//  if((retSD = f_mount(&SDFatFS, "0:/", 1)) == FR_OK)
  if((retSD = f_mount(&SDFatFS, &SDPath[0], 1)) == FR_OK)
  {
	  sprintf(str, "f_mount OK %d\n", retSD);
	  HAL_UART_Transmit(&huart1, str, STR_SIZE, 10);
  }
  else
  {
	  sprintf(str, "f_mount failed %d\n", retSD);
	  HAL_UART_Transmit(&huart1, str, STR_SIZE, 10);
  }
  uart_buffer_clean(str, STR_SIZE);

  /*
  // 2. File create & write
  if((retSD = f_open(&SDFile, "0:/Hello_world.txt", FA_CREATE_NEW | FA_WRITE)) == FR_OK)
  {
	  sprintf(buf, "Hello world~!");
	  f_write(&SDFile, buf, sizeof(buf),  &bw);

	  sprintf(str, "%d bytes\n", bw);
	  HAL_UART_Transmit(&huart1, str, STR_SIZE, 10);

	  f_close(&SDFile);
  }
  else
  {
	  sprintf(str, "error %d\n", retSD);
	  HAL_UART_Transmit(&huart1, str, STR_SIZE, 10);
  }
  */

  // 4. Read file info in folder
  DIR dir;
  FILINFO filinfo;
  uint16_t filecnt = 0;
  if((retSD = f_opendir(&dir, "0:/")) == FR_OK)
  {
	  while(1)
	  {
		  retSD = f_readdir(&dir, &filinfo);
		  if(dir.sect == 0) break;

		  sprintf(str, "%s\t 0x%x\n", filinfo.fname, filinfo.fattrib);
		  HAL_UART_Transmit(&huart1, str, STR_SIZE, 10);
		  uart_buffer_clean(str, STR_SIZE);
		  filecnt++;
	  }
	  uart_buffer_clean(str, STR_SIZE);
	  sprintf(str, "\nTotal %d file(s)..\n\n", filecnt);
	  HAL_UART_Transmit(&huart1, str, STR_SIZE, 10);
  }
  else
  {
	  sprintf(str, "The file/directory object is invalid! %d\n", retSD);
	  HAL_UART_Transmit(&huart1, str, STR_SIZE, 10);
  }
  uart_buffer_clean(str, STR_SIZE);

  // 3. File read
//  if((retSD = f_open(&SDFile, "0:/sdp.txt", FA_OPEN_EXISTING | FA_READ)) == FR_OK)
  // if file not exist
  if((retSD = f_open(&SDFile, "0:/sdp1234.txt", FA_OPEN_EXISTING | FA_READ)) == FR_OK)
  {
	  f_read(&SDFile, buf, 32, &br);
	  sprintf(str, "%s\n", buf);
	  HAL_UART_Transmit(&huart1, str, STR_SIZE, 10);
	  uart_buffer_clean(str, STR_SIZE);

	  sprintf(str, "%d bytes\n", br);
	  HAL_UART_Transmit(&huart1, str, STR_SIZE, 10);
	  uart_buffer_clean(str, STR_SIZE);
  }
  else
  {
	  sprintf(str, "error %d\n", retSD);
	  HAL_UART_Transmit(&huart1, str, STR_SIZE, 10);
	  uart_buffer_clean(str, STR_SIZE);
  }

  while (1)
  {

	  if(flag1)
	  {
		  flag1 = 0;
		  HAL_UART_Transmit(&huart1, str, STR_SIZE, 10);
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
		HAL_UART_Receive_IT(&huart1, str, STR_SIZE);
		flag1 = 1;
	}
}

void uart_buffer_clean(char *str, uint32_t cnt)
{
	while(cnt--)
		*str++ = 0;
}
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
