/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "rtc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "stdio.h"
#include "reset-cause.h"
#include "utils.h"
#include "constants.h"
#include "ringbuffer.h"

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
RingBuffer txBuf, rxBuf;
uint8_t txData;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t len) {
	if (HAL_UART_Transmit_IT(huart, pData, len) != HAL_OK) { //vero tentativo di trasmissione
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);
		if (RingBuffer_Write(&txBuf, pData, len) != RING_BUFFER_OK) {
			for (int e = 0; e < 10; e++) {
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);
				HAL_Delay(200);
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_14);
				HAL_Delay(200);
			}
			return 0;
		}
	}
	return 1;
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	//callback chiamata quando ha finito invio di tutti i byte del parametro size (variabile "len" qui sopra)

	if (RingBuffer_GetDataLength(&txBuf) > 0) {
		RingBuffer_Read(&txBuf, &txData, 1);
		HAL_UART_Transmit_IT(huart, &txData, 1);
	} else {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);
	}
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	//declareConst();
	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_RTC_Init();
	MX_USART3_UART_Init();
	/* USER CODE BEGIN 2 */

//	char* readInputTillEnter(char readBuf[]) {
//
//		int enterFound = 0;
//		int bufPosition = 0;
//		while (!enterFound) {
//			if (bufPosition == MAXINPUTBUF - 1) {
//				return readBuf;
//			}
//			char currentChar[1];
//			HAL_UART_Receive(&huart3, (uint8_t*) currentChar, 1,
//			HAL_MAX_DELAY);
//
//			if (currentChar[0] == '\r') {
//				readBuf[bufPosition] = '\0'; //terminate the array
//				enterFound = 1;
//			} else {
//				HAL_UART_Transmit(&huart3, (uint8_t*) currentChar, 1,
//				HAL_MAX_DELAY); //echo char back to terminal
//				readBuf[bufPosition] = currentChar[0];
//				bufPosition++;
//			}
//		}
//		return readBuf;
//	}
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);

	print_reset_cause();
	HAL_Delay(1000);
	print_cpu_freq();
	HAL_Delay(100);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		int i = 1000;

//		HAL_UART_Transmit(&huart3, (uint8_t*) MSG_INSERT_TEXT_AND_PRESS_ENTER,
//				strlen(MSG_INSERT_TEXT_AND_PRESS_ENTER), HAL_MAX_DELAY);

//		char readBuf[MAXINPUTBUF];
//		char *answer = readInputTillEnter(readBuf);
//
//		HAL_UART_Transmit(&huart3, (uint8_t*) MSG_YOU_WROTE,
//				strlen(MSG_YOU_WROTE), HAL_MAX_DELAY);
//		HAL_UART_Transmit(&huart3, (uint8_t*) answer, strlen(readBuf),
//				HAL_MAX_DELAY);

//		HAL_UART_Transmit(&huart3, (uint8_t*) MSG_TEST,	strlen(MSG_TEST), HAL_MAX_DELAY);

		//HAL_UART_Transmit_IT(&huart3, (uint8_t*) MSG_TEST1, strlen(MSG_TEST1));
		//HAL_UART_Transmit_IT(&huart3, (uint8_t*) MSG_TEST2, strlen(MSG_TEST2));

		UART_Transmit(&huart3, (uint8_t*) MSG_TEST1, strlen(MSG_TEST1));
		UART_Transmit(&huart3, (uint8_t*) MSG_TEST2, strlen(MSG_TEST2));

		HAL_Delay(i);

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
