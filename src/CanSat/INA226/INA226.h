/*
 * INA226.h
 *
 *  Created on: Apr 1, 2026
 *      Author: Светлана
 */

#ifndef INA226_H_
#define INA226_H_
#include "stm32f1xx.h"

typedef struct
{
	uint8_t addr;
	I2C_HandleTypeDef *hi2c2;
}ina226_bus_t;

typedef enum
{
	INA_AVG_1 = 0,
	INA_AVG_4 = 1,
	INA_AVG_16 = 2,
	INA_AVG_64 = 3,
	INA_AVG_128 = 4,
	INA_AVG_256 = 5,
	INA_AVG_512 = 6,
	INA_AVG_1024 = 7

}ina226_avg_t;

typedef enum
{
	VBUSCT_140US = 0,
	VBUSCT_204US = 1,
	VBUSCT_332US = 2,
	VBUSCT_588US = 3,
	VBUSCT_1p1MS = 4,
	VBUSCT_2p116MS = 5,
	VBUSCT_4p156 = 6,
	VBUSCT_8p244MS = 7

}ina226_vbusct_t;

typedef enum
{
	VSHCT_140US = 0,
	VSHCT_204US = 1,
	VSHCT_332US = 2,
	VSHCT_588US = 3,
	VSHCT_1p1MS = 4,
	VSHCT_2p116MS = 5,
	VSHCT_4p156 = 6,
	VSHCT_8p244MS = 7

}ina226_vshct_t;

typedef enum
{
	OP_MODE_SHUTDOWN = 0,
	OP_MODE_SHUNTnVOLT_TRIGGERED = 1,
	OP_MODE_BUSnVOLT_TRIGGERED = 2,
	OP_MODE_SHUNTnBUS_TRIGGERED = 3,
	//OP_MODE_SHUTDOWN = 4,
	OP_MODE_SHUNTnVOLT = 5,
	OP_MODE_BUSnVOLT = 6,
	OP_MODE_SHUNTnBUS = 7

}ina226_operatig_mode_t;








#endif /* INA226_H_ */

