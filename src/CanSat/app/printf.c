/*
 * printf.c
 *
 *  Created on: Feb 21, 2026
 *      Author: KT
 */
#include "stm32f1xx.h"

extern UART_HandleTypeDef huart2;

int _write(int file, char *ptr, int len)
{
	HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, 1000);


  return len;
}

