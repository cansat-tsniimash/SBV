/*
 * photores.c
 *
 *  Created on: Apr 11, 2026
 *      Author: Светлана
 */

#include "stm32f1xx.h"
extern ADC_HandleTypeDef hadc1;

float photores_read_data(void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, RESET);
	HAL_ADC_Init(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 100);
	uint32_t data = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);

	return (3.3 / 4095) * data;
}

