/*
 * utils.c
 *
 *  Created on: 26 feb 2020
 *      Author: uburti
 */

#include "utils.h"

#include "usart.h"
#include "constants.h"
#include "string.h"
#include "stdio.h"
#include "reset-cause.h"

  void print_reset_cause() {
  	reset_cause_t reset_cause = reset_cause_get();

  	char *RESET_CAUSE = reset_cause_get_name(reset_cause);
  	HAL_UART_Transmit(&huart3, (uint8_t*) MSG_THE_SYSTEM_RESET_CAUSE_IS, strlen(MSG_THE_SYSTEM_RESET_CAUSE_IS),
  	HAL_MAX_DELAY);
  	HAL_UART_Transmit(&huart3, (uint8_t*) RESET_CAUSE, strlen(RESET_CAUSE),
  	HAL_MAX_DELAY);
  }

  void print_cpu_freq() {
  	HAL_UART_Transmit(&huart3, (uint8_t*) MSG_RUNNING_AT, strlen(MSG_RUNNING_AT), HAL_MAX_DELAY);
  	uint32_t currentClock = HAL_RCC_GetSysClockFreq();
  	char clockValue[50];
  	currentClock = currentClock / 1000000;
  	sprintf(clockValue, "%lu", currentClock);
  	HAL_UART_Transmit(&huart3, (uint8_t*) clockValue, strlen(clockValue), HAL_MAX_DELAY);
  }
