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
#include "can.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
volatile uint8_t uart_rx_data;
volatile uint8_t SW1_flag = 0;
volatile uint8_t SW2_flag = 0;
volatile uint8_t SW3_flag = 0;
volatile uint8_t SW4_flag = 0;
volatile uint8_t can1_rx0_flag = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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
  MX_CAN1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  canFilter1.FilterMaskIdHigh = 0x7F3 << 5;
  canFilter1.FilterIdHigh = 0x106 << 5;
  canFilter1.FilterMaskIdLow = 0x7F3 << 5;
  canFilter1.FilterIdLow = 0x106 << 5;
  canFilter1.FilterMode = CAN_FILTERMODE_IDMASK;
  canFilter1.FilterScale = CAN_FILTERSCALE_16BIT;
  canFilter1.FilterFIFOAssignment = CAN_FILTER_FIFO0;
  canFilter1.FilterBank = 0;
  canFilter1.FilterActivation = ENABLE;

  HAL_CAN_ConfigFilter(&hcan1, &canFilter1);
  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);

  HAL_CAN_Start(&hcan1);

  char str[20] = {0};
  while (1)
  {
	  if(can1_rx0_flag)
	  {
		  can1_rx0_flag = 0;

		  sprintf(str, "Rx ID: 0x%X\n", canRxHeader.StdId);
		  HAL_UART_Transmit(&huart1, str, sizeof(str), 10);

		  for(int i = 0; i < 8; i++)
		  {
			  sprintf(str, "Rx Data[%d]: 0x%X\n", i, can1Rx0Data[i]);
			  HAL_UART_Transmit(&huart1, str, sizeof(str), 10);
			  HAL_Delay(10);
		  }
		  sprintf(str, "\n\n");
		  HAL_UART_Transmit(&huart1, str, sizeof(str), 10);
	  }

	  // flag1
	  if(SW1_flag)
	  {
		  SW1_flag = 0;

		  canTxHeader.StdId = 0x102;
		  canTxHeader.RTR = CAN_RTR_DATA;
		  canTxHeader.IDE = CAN_ID_STD;
		  canTxHeader.DLC = 8;

		  for(int i = 0; i < 8; i++) can1Tx0Data[i]++;

		  TxMailBox = HAL_CAN_GetTxMailboxesFreeLevel(&hcan1);
		  HAL_CAN_AddTxMessage(&hcan1, &canTxHeader, &can1Tx0Data[0], &TxMailBox);
	  }

	  // flag2
	  if(SW2_flag)
	  {
		  SW2_flag = 0;

		  canTxHeader.StdId = 0x106;
		  canTxHeader.RTR = CAN_RTR_DATA;
		  canTxHeader.IDE = CAN_ID_STD;
		  canTxHeader.DLC = 8;

		  for(int i = 0; i < 8; i++) can1Tx0Data[i]++;

		  TxMailBox = HAL_CAN_GetTxMailboxesFreeLevel(&hcan1);
		  HAL_CAN_AddTxMessage(&hcan1, &canTxHeader, &can1Tx0Data[0], &TxMailBox);
	  }

	  // flag3
	  if(SW3_flag)
	  {
		  SW3_flag = 0;

		  canTxHeader.StdId = 0x10A;
		  canTxHeader.RTR = CAN_RTR_DATA;
		  canTxHeader.IDE = CAN_ID_STD;
		  canTxHeader.DLC = 8;

		  for(int i = 0; i < 8; i++) can1Tx0Data[i]++;

		  TxMailBox = HAL_CAN_GetTxMailboxesFreeLevel(&hcan1);
		  HAL_CAN_AddTxMessage(&hcan1, &canTxHeader, &can1Tx0Data[0], &TxMailBox);
	  }

	  // flag4
	  if(SW4_flag)
	  {
		  SW4_flag = 0;

		  canTxHeader.StdId = 0x10E;
		  canTxHeader.RTR = CAN_RTR_DATA;
		  canTxHeader.IDE = CAN_ID_STD;
		  canTxHeader.DLC = 8;

		  for(int i = 0; i < 8; i++) can1Tx0Data[i]++;

		  TxMailBox = HAL_CAN_GetTxMailboxesFreeLevel(&hcan1);
		  HAL_CAN_AddTxMessage(&hcan1, &canTxHeader, &can1Tx0Data[0], &TxMailBox);
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
		HAL_UART_Receive_IT(&huart1, &uart_rx_data, 1);

		if(uart_rx_data == 1)
		{
			SW1_flag = 1;
		}
		else if(uart_rx_data  == 2)
		{
			SW2_flag = 1;
		}
		else if(uart_rx_data == 3)
		{
			SW3_flag = 1;
		}
		else if(uart_rx_data == 4)
		{
			SW4_flag = 1;
		}
	}
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	if(hcan->Instance == CAN1)
	{
		HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &canRxHeader, &can1Rx0Data[0]);
		can1_rx0_flag = 1;
	}
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
