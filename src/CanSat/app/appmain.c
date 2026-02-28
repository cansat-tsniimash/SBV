/*
 * appmain.c
 *
 *  Created on: Feb 7, 2026
 *      Author: KT
 */
#include "stm32f1xx.h"
#include "delay/dwt_delay.h"
#include "ds18b20/ds18b20.h"
#include "neo6mv2/neo6mv2.h"
#include "bmp280/bme280.h"
#include "bmp280/bmp280.h"

extern UART_HandleTypeDef huart1;
extern I2C_HandleTypeDef hi2c1;

void appMain()
{
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_ERR);
	neo6mv2_Init();

	struct bme280_dev bmp280;
	bmp280.intf = BME280_I2C_INTF;
	bmp280.read = bmp280_read_reg;
	bmp280.delay_us = bmp280_delay_us;
	bmp280.write = bmp280_write_reg;

	bmp280.settings.osr_p = BME280_OVERSAMPLING_16X;
	bmp280.settings.osr_t = BME280_OVERSAMPLING_16X;
	bmp280.settings.standby_time = BME280_STANDBY_TIME_20_MS;
	bmp280.settings.filter = BME280_FILTER_COEFF_16;

	bmp280_bus_t bmp_bus;
	bmp_bus.addr = 0x76 << 1;
	bmp_bus.hi2c1 = &hi2c1;
	bmp280.intf_ptr = &bmp_bus;


	ds18b20_init(DS18B20_12_BIT);
	uint32_t ds18b20_timer = HAL_GetTick();
	ds18b20_conv();


	bme280_init(&bmp280);
	bme280_set_sensor_settings(BME280_ALL_SETTINGS_SEL, &bmp280);
	bme280_set_sensor_mode(BME280_NORMAL_MODE, &bmp280);

	struct bme280_data bmp280_data;

	while(1)
	{

		bme280_get_sensor_data(BME280_PRESS | BME280_TEMP, &bmp280_data, &bmp280);



		for(int i = 0; i < 10; i++)
		{
			if (neo6mv2_work())
				break;
		}

		volatile GPS_Data getData = neo6mv2_GetData();
		printf("cookie = %d\n", getData.cookie);
		printf("fix = %d\n", getData.fixQuality);
		printf("%d %d %f %f\n", getData.cookie, getData.fixQuality, getData.latitude, getData.longitude);

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
