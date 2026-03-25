/*
 * lora.c
 *
 *  Created on: Mar 18, 2026
 *      Author: Светлана
 */
#include "stm32f1xx.h"
#include "lora.h"


void lora_mode_switch(lora_connect_t* lora, lora_mode_t mode)
{
	switch(mode)
	{
	case LORA_MODE_TM:
		HAL_GPIO_WritePin(lora->M0_Port, lora->M0_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(lora->M1_Port, lora->M1_Pin, GPIO_PIN_RESET);
		break;
	case LORA_MODE_WorTM:
		HAL_GPIO_WritePin(lora->M0_Port, lora->M0_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(lora->M1_Port, lora->M1_Pin, GPIO_PIN_RESET);
		break;
	case LORA_MODE_WorRC:
		HAL_GPIO_WritePin(lora->M0_Port, lora->M0_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(lora->M1_Port, lora->M1_Pin, GPIO_PIN_SET);
		break;
	case LORA_MODE_DC:
		HAL_GPIO_WritePin(lora->M0_Port, lora->M0_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(lora->M1_Port, lora->M1_Pin, GPIO_PIN_SET);
		break;
	}
}



void lora_write_reg(lora_connect_t* lora, uint8_t addr, uint8_t *data, uint8_t size)
{
	uint8_t buff_reg[12] = {0};
	buff_reg[0] = 0xC0;
	buff_reg[1] = addr;
	buff_reg[2] = size;
	for(int i = 0; i < size; i++)
	{
		buff_reg[i + 3] = data[i];
	}
	HAL_UART_Transmit(lora->uart, buff_reg, size + 3, 100);
}

void lora_channel_control(lora_connect_t* lora, uint8_t chanel)
{
	lora_write_reg(lora, 0x00, &chanel, 1);

}

void lora_write_addr(lora_connect_t* lora, uint16_t addr)
{
	lora_write_reg(lora, 0x00, (uint8_t*)&addr , 2);
}

void lora_set_reg0(lora_connect_t* lora, air_data_rate_t adr, serial_port_rate_t spr, serial_party_bit_t spb)
{

	uint8_t data = 0;
	data = data | adr;
	data = data | (spb << 3);
	data = data | (spr << 5);
}

void lora_set_reg1(lora_connect_t* lora, sum_packet_settings_t sps, rssi_ambient_noise_enable_t rane, transmitting_power_t tp)
{

	uint8_t data = 0;
	data = data | (sps << 6);
	data = data | tp;
	data = data | (rane << 5);
}





//HAL_UART_Transmit();



//HAL_GPIO_WritePin();
