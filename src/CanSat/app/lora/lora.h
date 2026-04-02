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

typedef enum
{
	LORA_AIR_D_R_2P4 = 2,
	LORA_AIR_D_R_4P8 = 3,
	LORA_AIR_D_R_9P6 = 4,
	LORA_AIR_D_R_19P2 = 5,
	LORA_AIR_D_R_38P4 = 6,
	LORA_AIR_D_R_62P5 = 7
}air_data_rate_t;

typedef enum
{
	LORA_SER_P_R_1200 = 0,
	LORA_SER_P_R_2400 = 1,
	LORA_SER_P_R_4800 = 2,
	LORA_SER_P_R_9600 = 3,
	LORA_SER_P_R_19200 = 4,
	LORA_SER_P_R_38400 = 5,
	LORA_SER_P_R_57600 = 6,
	LORA_SER_P_R_115200 = 7
}serial_port_rate_t;

typedef enum
{
	LORA_SPB_8N1 = 0,
	LORA_SPB_8O1 = 1,
	LORA_SPB_8E1 = 2,
}serial_party_bit_t;

typedef enum
{
	LORA_SPS_200B = 0,
	LORA_SPS_128B = 1,
	LORA_SPS_64B = 2,
	LORA_SPS_32B = 3

}sum_packet_settings_t;

typedef enum
{
	LORA_RSSI_ANE_DIS = 0,
	LORA_RSSI_ANE_ENBL = 1

}rssi_ambient_noise_enable_t;

typedef enum
{
	LORA_TP_22DBM = 0,
	LORA_TP_17DBM = 1,
	LORA_TP_13DBM = 2,
	LORA_TP_10DBM = 3

}transmitting_power_t;

typedef enum
{
	ENBL_RSSI_BYTE_DIS = 0,
	ENBL_RSSI_BYTE_ENBL = 1
}enable_RSSI_byte_t;

typedef enum
{
	TRANSPARENT_TM = 0,
	FIXED_TRANSMITION_MOD = 1
}transmitions_method_t;

typedef enum
{
	LBT_DISABLE = 0,
	LBT_ENABLE = 1
}lbt_enable_t;

typedef enum
{
	WOR_CYCLE_500MS = 0,
	WOR_CYCLE_1000MS = 1,
	WOR_CYCLE_1500MS = 2,
	WOR_CYCLE_2000MS = 3,
	WOR_CYCLE_2500MS = 4,
	WOR_CYCLE_3000MS = 5,
	WOR_CYCLE_3500MS = 6,
	WOR_CYCLE_4000MS = 7
}wor_cycle_t;

void lora_channel_control(lora_connect_t* lora, uint8_t chanel);
void lora_write_addr(lora_connect_t* lora, uint16_t addr);
void lora_set_reg0(lora_connect_t* lora, air_data_rate_t adr, serial_port_rate_t spr, serial_party_bit_t spb);
void lora_set_reg1(lora_connect_t* lora, sum_packet_settings_t sps, rssi_ambient_noise_enable_t rane, transmitting_power_t tp);
void lora_mode_switch(lora_connect_t* lora, lora_mode_t mode);

void lora_send_packet(lora_connect_t* lora, uint8_t *reg_data, uint16_t len);



#endif /* LORA_H_ */
