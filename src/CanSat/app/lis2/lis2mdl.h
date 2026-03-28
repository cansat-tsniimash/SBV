/*
 * lis2mdl.h
 *
 *  Created on: Mar 7, 2026
 *      Author: Светлана
 */

#ifndef LIS2_LIS2MDL_H_
#define LIS2_LIS2MDL_H_

#include "lis2mdl_reg.h"

typedef struct lis2mdl_data_s
{
	uint8_t addr;
	I2C_HandleTypeDef *hi2c1;
}lis2mdl_data_t;

int32_t lis_read_reg (void *handle, uint8_t sub, uint8_t *data, uint16_t len);
int32_t lis_write_reg (void *handle, uint8_t sub, uint8_t *data, uint16_t len);


#endif /* LIS2_LIS2MDL_H_ */
