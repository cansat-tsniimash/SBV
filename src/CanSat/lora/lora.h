/*
 * lora.h
 *
 *  Created on: Mar 18, 2026
 *      Author: Светлана
 */

#ifndef LORA_H_
#define LORA_H_

#include "stm32f1xx.h"

typedef struct
{
	UART_HandleTypeDef *uart;
	GPIO_TypeDef *M0_Port;
	uint16_t M0_Pin;
	GPIO_TypeDef *M1_Port;
	uint16_t M1_Pin;
	GPIO_TypeDef *Aux_Port;
	uint16_t Aux_Pin;
}lora_connect_t;

typedef enum
{
	LORA_MODE_TM = 0,
	LORA_MODE_WorTM = 1,
	LORA_MODE_WorRC = 2,
	LORA_MODE_DC = 3
} lora_mode_t;

/*typedef enum
{
	LORA_AIR_RATE_2pP4 = 2,
	LORA_MODE_WOR_SM = 2,
	LORA_MODE_WOR_RM = 2,
	LORA_MODE_SN = 3,
}lora_air_rate_t;*/

typedef enum
{
	AIR_D_R_2P4 = 2,
	AIR_D_R_4P8 = 3,
	AIR_D_R_9P6 = 4,
	AIR_D_R_19P2 = 5,
	AIR_D_R_38P4 = 6,
	AIR_D_R_62P5 = 7
}air_data_rate_t;

typedef enum
{
	SER_P_R_1200 = 0,
	SER_P_R_2400 = 1,
	SER_P_R_4800 = 2,
	SER_P_R_9600 = 3,
	SER_P_R_19200 = 4,
	SER_P_R_38400 = 5,
	SER_P_R_57600 = 6,
	SER_P_R_115200 = 7
}serial_port_rate_t;

typedef enum
{
	SPB_8N1 = 0,
	SPB_8O1 = 1,
	SPB_8E1 = 2,
}serial_party_bit_t;




#endif /* LORA_H_ */
