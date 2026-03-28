/*
 * bmp280.c
 *
 *  Created on: Feb 25, 2026
 *      Author: Светлана
 */

#include "bme280_defs.h"
#include "stm32f1xx.h"
#include "delay/dwt_delay.h"
#include "bmp280.h"
#include "i2c-crutch.h"







 BME280_INTF_RET_TYPE bmp280_read_reg (uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr)
 {
	 bmp280_bus_t *ptr = (bmp280_bus_t*)intf_ptr;
	 HAL_StatusTypeDef hal_res;
	 hal_res = HAL_I2C_Master_Transmit(ptr->hi2c1, ptr->addr, &reg_addr, 1, 100);
	 if(hal_res != HAL_OK)
	 {
		 I2C_ClearBusyFlagErratum(ptr->hi2c1, 100);


		 return hal_res;
	 }
	 hal_res = HAL_I2C_Master_Receive(ptr->hi2c1, ptr->addr, reg_data, len, 150);
	 if(hal_res != HAL_OK)
	 {
		 return hal_res;
	 }
	 return HAL_OK;
 }

 BME280_INTF_RET_TYPE bmp280_write_reg(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr)
 {
	 bmp280_bus_t *ptr = (bmp280_bus_t*)intf_ptr;
	 uint8_t addr [2] = {reg_addr, reg_data[0]};
	for(int i = 0; i < len; i++)
	{
		addr[0] = reg_addr + i;
		addr[1] = reg_data[i];
		HAL_StatusTypeDef hal_res = HAL_I2C_Master_Transmit(ptr->hi2c1, ptr->addr, addr, 2, 100);
		if (hal_res != HAL_OK)
		{
			return hal_res;
		}
		return HAL_OK;
	}

	 return 0;
 }

 void bmp280_delay_us(uint32_t period, void *intf_ptr)
 {
	 dwt_delay_us(period);
 }


