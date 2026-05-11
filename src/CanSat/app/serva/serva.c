/*
 * serva.c
 *
 *  Created on: Apr 22, 2026
 *      Author: 1
 */

#include "stm32f1xx.h"

extern TIM_HandleTypeDef htim2;

void serva_rotate_ch2(double value_)
{
	if (value_> 180)
	{
		value_ = 180;
	}
	else if (value_ < 0)
	{
		value_ = 0;
	}
	const uint16_t serva_min = 300;
	const uint16_t serva_max = 2800;
	int value = (serva_max - serva_min) * value_ / 0xFF + serva_min;

	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, value);
}

void serva_rotate_ch4(double value_)
{
	if (value_> 180)
	{
		value_ = 180;
	}
	else if (value_ < 0)
	{
		value_ = 0;
	}
	const uint16_t serva_min = 300;
	const uint16_t serva_max = 2800;
	int value = (serva_max - serva_min) * value_ / 0xFF + serva_min;


	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, value);
}


