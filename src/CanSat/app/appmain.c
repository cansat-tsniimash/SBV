/*
 * appmain.c
 *
 *  Created on: Feb 7, 2026
 *      Author: KT
 */
#include "stm32f1xx.h"
#include "delay/dwt_delay.h"
#include "ds18b20/ds18b20.h"


void appMain()
{
	//volatile uint8_t byte = 0b01010110;
	HAL_Delay(750);

	ds18b20_init(DS18B20_12_BIT);

	uint32_t ds18b20_timer = HAL_GetTick();
	ds18b20_conv();

	while(1)
	{
		if((HAL_GetTick() - ds18b20_timer) > 750 )
		{
			volatile float res = ds18b20_read_temp();
			ds18b20_conv();
			ds18b20_timer = HAL_GetTick();

		}
	}
			/*HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);*/

	return;

}
