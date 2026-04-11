/*
 * INA226.C
 *
 *  Created on: Apr 1, 2026
 *      Author: Светлана
 */

#include "stm32f1xx.h"
#include "INA226.h"

void ina226_write_reg(uint8_t reg_addr, uint16_t reg_data, ina226_bus_t *ina)
{
	uint8_t buf[3] = {0};
	buf[0] = reg_addr;
	buf[1] = reg_data >> 8;
	buf[2] = reg_data;

	HAL_I2C_Master_Transmit(ina->hi2c2, ina->addr, buf, 3, 100);
}

void ina226_read_reg(uint8_t reg_addr, uint16_t *data, ina226_bus_t *ina)
{
	HAL_I2C_Master_Transmit(ina->hi2c2, ina->addr, &reg_addr, 1, 100);
	HAL_I2C_Master_Receive(ina->hi2c2, ina->addr, (uint8_t*)data, 2, 100);
}

void ina226_set_configuration_reg(ina226_bus_t *ina, ina226_avg_t avg, ina226_vbusct_t vbusct, ina226_vshct_t vshct, ina226_operatig_mode_t opm)
{
	uint16_t data = 0;
	data = data | (avg << 9);
	data = data | (vbusct << 6);
	data = data | (vshct << 3);
	data = data | opm;
	ina226_write_reg(0x00, data, ina);
}

void ina226_set_calibration_reg(ina226_bus_t ina, uint16_t data)
{
	ina226_write_reg(0x05, data & 0x7FFF, &ina);
}


int16_t ina226_get_shunt_voltage_reg(ina226_bus_t *ina)
{
	int16_t data = 0;
	ina226_read_reg(0x01, (uint16_t *)&data, ina);
	data = data * 2.5 / 1000;
	return data;
}

int16_t ina226_set_bus_voltage_reg(ina226_bus_t *ina)
{
	uint16_t data = 0;
	ina226_read_reg(0x02, &data, ina);
	data = data * 1.25;
	return data;
}

int16_t ina226_set_power_reg(ina226_bus_t *ina)
{
	uint16_t data = 0;
	ina226_read_reg(0x03, &data, ina);
	return data;
}

int16_t ina226_current_reg(ina226_bus_t *ina)
{
	uint16_t data = 0;
	ina226_read_reg(0x04, &data, ina);
	return data;
}


