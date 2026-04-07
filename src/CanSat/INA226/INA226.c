/*
 * INA226.C
 *
 *  Created on: Apr 1, 2026
 *      Author: Светлана
 */

#include "stm32f1xx.h"
#include "INA226.h"

void ina226_write_reg(uint8_t reg_addr, uint8_t reg_data)
{
	ina226_bus_t *ina;
	uint8_t buf[3] = {0};
	buf[0] = reg_addr;
	buf[1] = reg_data >> 8;
	buf[2] = reg_data;

	HAL_I2C_Master_Transmit(ina->hi2c2, ina->addr, buf, 2, 100);
}

void ina226_read_reg(uint8_t reg_addr, uint16_t data)
{
	ina226_bus_t *ina;
	HAL_I2C_Master_Transmit(ina->hi2c2, ina->addr, data, 2, 100);
	HAL_I2C_Master_Receive(ina->hi2c2, ina->addr, data, 2, 100);

	return data;
}


