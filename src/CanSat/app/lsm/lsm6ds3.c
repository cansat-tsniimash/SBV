/*
 * lsm6ds3.c
 *
 *  Created on: Mar 4, 2026
 *      Author: Светлана
 */
#include "stm32f1xx.h"
#include "lsm6ds3.h"
#include "i2c-crutch.h"

int32_t lsm_write_reg (void *handle, uint8_t sub, const uint8_t *data, uint16_t len)
{
	lsm_data_t *lsm_data_ptr = (lsm_data_t*)handle;
	uint8_t addresMass [2] = {sub, data[0]};
	for(int i = 0; i < len; i++)
	{
		addresMass[0] = sub + i;
		addresMass[1] = data[i];
		HAL_StatusTypeDef hal_res = HAL_I2C_Master_Transmit(lsm_data_ptr->hi2c1, lsm_data_ptr->addr, addresMass, 2, 100);
		if(hal_res != HAL_OK)
		{
			if (hal_res == HAL_BUSY)
			{
				I2C_ClearBusyFlagErratum(lsm_data_ptr->hi2c1, 100);
			}
			return hal_res;
		}
	}
	return HAL_OK;
}

int32_t lsm_read_reg (void *handle, uint8_t sub, uint8_t *data, uint16_t len)
{
	HAL_StatusTypeDef hal_res;
	lsm_data_t *lsm_data_ptr = (lsm_data_t*)handle;
	hal_res = HAL_I2C_Master_Transmit(lsm_data_ptr->hi2c1, lsm_data_ptr->addr, &sub, 1, 100);
	if(hal_res != HAL_OK)
	{
		if (hal_res == HAL_BUSY)
		{
			I2C_ClearBusyFlagErratum(lsm_data_ptr->hi2c1, 100);
		}
		return hal_res;
	}
	hal_res = HAL_I2C_Master_Receive(lsm_data_ptr->hi2c1, lsm_data_ptr->addr, (uint8_t*)data, len, 150);
	if(hal_res != HAL_OK)
	{
		if (hal_res == HAL_BUSY)
		{
			I2C_ClearBusyFlagErratum(lsm_data_ptr->hi2c1, 100);
		}
		return hal_res;
	}
	return HAL_OK;
}
