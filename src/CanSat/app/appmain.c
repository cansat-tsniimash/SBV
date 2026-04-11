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
#include "lsm/lsm6ds3.h"
#include "lis2/lis2mdl.h"
#include "ff.h"
#include "lora/lora.h"
#include "photores/photores.h"


extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern I2C_HandleTypeDef hi2c1;



#pragma pack(push, 1)
typedef struct
{
	uint16_t start;
	uint16_t team_id;
	uint32_t time;
	uint16_t temperature;
	uint32_t pressure;
	uint16_t acc[3];
	uint16_t gyro[3];
	uint8_t sum;

	uint16_t packet_num;
	uint8_t state;
	float gps_latitude;
	float gps_longitude;
	float gps_height;
	uint8_t gps_fix;
	uint16_t photores[6];
	uint32_t ampermetr;
	uint16_t magn[3];
	uint16_t termometr;
	uint16_t acc2[3];
	uint16_t gyro2[3];
	uint8_t sum2;


}packet_t;

#pragma pack(pop)

uint8_t checksum(const void * data_, size_t size)
{
	if (0 == size)
		return 0;

	const uint8_t * data = (const uint8_t*)data_;
	uint8_t chk = *(data + 0);
	for (size_t i = 1; i < size; i++)
		chk = chk ^ data[i];

	return chk;
}


void appMain()
{
	packet_t packet = {0};
	packet.start = 0xAAAA;
	packet.team_id = 0xBBBB;

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

	lsm_data_t lsm_data;
	lsm_data.addr = 0x6A << 1;
	lsm_data.hi2c1 = &hi2c1;

	stmdev_ctx_t lsm6ds3;
	lsm6ds3.handle = &lsm_data;
	lsm6ds3.read_reg = lsm_read_reg;
	lsm6ds3.write_reg = lsm_write_reg;

	lsm6ds3_reset_set(&lsm6ds3, 1);
	lsm6ds3_xl_full_scale_set(&lsm6ds3, LSM6DS3_16g);
	lsm6ds3_xl_data_rate_set(&lsm6ds3, LSM6DS3_XL_ODR_104Hz);
	lsm6ds3_gy_full_scale_set(&lsm6ds3, LSM6DS3_2000dps);
	lsm6ds3_gy_data_rate_set(&lsm6ds3, LSM6DS3_GY_ODR_208Hz);

	int16_t buf_lsm_gy[3] = {0};
	int16_t buf_lsm_xl[3] = {0};
	volatile float gyro[3] = {0};
	volatile float acc[3] = {0};

	lis2mdl_data_t lis_data;
	lis_data.addr = 0x1E << 1;
	lis_data.hi2c1 = &hi2c1;

	lis2mdl_ctx_t lis2mdl;
	lis2mdl.handle = &lis_data;
	lis2mdl.read_reg = lis_read_reg;
	lis2mdl.write_reg = lis_write_reg;

	lis2mdl_reset_set(&lis2mdl, 1);
	lis2mdl_operating_mode_set(&lis2mdl, LIS2MDL_CONTINUOUS_MODE);
	lis2mdl_data_rate_set(&lis2mdl, LIS2MDL_ODR_50Hz);
	lis2mdl_power_mode_set(&lis2mdl, LIS2MDL_HIGH_RESOLUTION);


	lora_connect_t lora;
	lora.uart = &huart2;
	lora.Aux_Pin = GPIO_PIN_3;
	lora.Aux_Port = GPIOB;
	lora.M0_Pin = GPIO_PIN_1;
	lora.M0_Port = GPIOB;
	lora.M1_Pin = GPIO_PIN_0;
	lora.M1_Port = GPIOB;

	lora_mode_switch(&lora, LORA_MODE_DC);
	HAL_Delay(100);

	lora_channel_control(&lora, 3);
	HAL_Delay(50);
	lora_write_addr(&lora, 0xFFFF);
	HAL_Delay(50);
	lora_set_reg0(&lora, LORA_AIR_D_R_9P6, LORA_SER_P_R_9600, LORA_SPB_8N1);
	HAL_Delay(50);
	lora_set_reg1(&lora, LORA_SPS_200B, LORA_RSSI_ANE_DIS, LORA_TP_22DBM);
	HAL_Delay(50);
	lora_mode_switch(&lora, LORA_MODE_TM);
	HAL_Delay(100);




	int16_t buf_lis[3] = {0};
	volatile float lis[3] = {0};

	FATFS sd;
	FRESULT sd_result_mount = f_mount (&sd, "", 1);
	FIL fp;
	char fp_path[] = "SDPack.bin";
	FRESULT sd_result = 255;
	UINT byte_count;





	while(1)
	{
		bme280_get_sensor_data(BME280_PRESS | BME280_TEMP, &bmp280_data, &bmp280);
		packet.pressure = bmp280_data.pressure;
		packet.temperature = bmp280_data.temperature * 100;

		lsm6ds3_acceleration_raw_get(&lsm6ds3, buf_lsm_xl);
		lsm6ds3_angular_rate_raw_get(&lsm6ds3, buf_lsm_gy);

		for(int i = 0; i < 3; i++)
		{
			packet.acc[i] = buf_lsm_xl[i];
			packet.gyro[i] =buf_lsm_gy[i];

		}
		lis2mdl_magnetic_raw_get(&lis2mdl, buf_lis);
		for(int i = 0; i < 3; i++)
		{
			packet.magn[i] = buf_lis[i];
		}

		for(int i = 0; i < 10; i++)
		{
			if (neo6mv2_work())
				break;
		}

		volatile GPS_Data getData = neo6mv2_GetData();
		//printf("cookie = %d\n", getData.cookie);
		//printf("fix = %d\n", getData.fixQuality);
		//printf("%d %d %f %f\n", getData.cookie, getData.fixQuality, getData.latitude, getData.longitude);

		packet.gps_fix = getData.fixQuality;
		packet.gps_latitude = getData.latitude;
		packet.gps_longitude = getData.longitude;
		packet.gps_height = getData.altitude;

		if((HAL_GetTick() - ds18b20_timer) > 750 )
		{
			volatile float res = ds18b20_read_temp();

			ds18b20_conv();
			ds18b20_timer = HAL_GetTick();
			packet.termometr = res * 10;
		}



		if (sd_result_mount != FR_OK)
		{
			f_mount(NULL, "", 1);
			sd_result_mount = f_mount(&sd, "", 1);
		}

		if ((sd_result_mount == FR_OK) && (sd_result != FR_OK))
		{
			if (sd_result != 255)
				f_close(&fp);
			sd_result = f_open(&fp, (const TCHAR*)&fp_path, FA_WRITE|FA_OPEN_ALWAYS|FA__WRITTEN);
		}


		if ((sd_result_mount == FR_OK)&&(sd_result == FR_OK))
		{
			sd_result = f_write(&fp, &packet, sizeof(packet_t), &byte_count);
			f_sync(&fp);
		}

		packet.packet_num++;
		packet.time = HAL_GetTick();
		packet.sum = checksum(&packet, offsetof(packet_t, sum));
		packet.sum2 = checksum(
				((uint8_t*)&packet) + offsetof(packet_t, sum) + sizeof(packet.sum),
				sizeof(packet) - offsetof(packet_t, sum) - sizeof(packet.sum) - sizeof(packet.sum2)
		);
		lora_send_packet(&lora, (uint8_t *)&packet, sizeof(packet_t));

	}


	float photores_data;
	typedef enum
	{
		STATE_PRESTART = 0,	//укладка
		STATE_PRESTART_WAIT = 1,	//укладка
		STATE_IN_ROCKET = 1,	//В ракете
		STATE_SP_OPENING = 2,	//Открытие СП
		STATE_DROP = 3,	//Спуск
		STATE_GROUND = 4	//Земля
	}state_name_t;

	/*state_name_t state = STATE_PRESTART;
	uint32_t state_timer
	switch(state)
	{
		case STATE_PRESTART:
			if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == GPIO_PIN_SET)
			{
				photores_data = photores_read_data;
				HAL_Delay(15000);
				state_timer = HAL_GetTick();
				state = STATE_PRESTART_WAIT;
			}
			break;

		case STATE_PRESTART_WAIT:
			if ()
			{
				state = STATE_IN_ROCKET;
			}
			break;

		case STATE_IN_ROCKET:
			//if ()   //добавить условие освещенности
			{
				HAL_Delay(5000);
				state = SP_OPENING;
			}
			break;

		case STATE_SP_OPENING:
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
				HAL_Delay(1000);
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);	//Включение пережигателя
				state = DROP;
			}
			break;

		case STATE_DROP:

			//основная часть с наведением
			break;

		case STATE_GROUND:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
			break;

	}*/











	return;

}
