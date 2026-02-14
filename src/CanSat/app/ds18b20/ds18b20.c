/*
 * ds18b20.c
 *

 */

#include "stm32f1xx.h"
#include "main.h"
#include "delay/dwt_delay.h"
#include "ds18b20.h"

/*
  HAL_GPIO_WritePin(ds18b20_GPIO_Port, ds18b20_Pin, GPIO_PIN_RESET);

  Configure GPIO pin : ds18b20_Pin
  GPIO_InitStruct.Pin = ds18b20_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ds18b20_GPIO_Port, &GPIO_InitStruct); */

void one_wire_force_down()
{
	  GPIO_InitTypeDef GPIO_InitStruct = {0};
	  GPIO_InitStruct.Pin = ds18b20_Pin;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
	  HAL_GPIO_Init(ds18b20_GPIO_Port, &GPIO_InitStruct);
	  HAL_GPIO_WritePin(ds18b20_GPIO_Port, ds18b20_Pin, GPIO_PIN_RESET);
}

void one_wire_release()
{
	  GPIO_InitTypeDef GPIO_InitStruct = {0};
	  GPIO_InitStruct.Pin = ds18b20_Pin;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	  GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
	  HAL_GPIO_Init(ds18b20_GPIO_Port, &GPIO_InitStruct);
	  HAL_GPIO_WritePin(ds18b20_GPIO_Port, ds18b20_Pin, GPIO_PIN_RESET);
}

void one_wire_write_bit(uint8_t bit)
{

	if(bit == 0)
	{
		 one_wire_force_down();
		 dwt_delay_us(60);
		 one_wire_release();

	}
	else
	{
		 one_wire_force_down();
		 dwt_delay_us(1);
		 one_wire_release();
		 dwt_delay_us(59);
	}
}

void one_wire_write_byte(uint8_t byte)
{
	for(int i = 0; i < 8; i++)
	{
		if ((byte & (1 << i)) == 0)
		{
			one_wire_write_bit(0);
		}
		else
		{
			one_wire_write_bit(1);
		}
	}
}

GPIO_PinState one_wire_read_bit()
{
	one_wire_force_down();
	dwt_delay_us(1);
	one_wire_release();
	//dwt_delay_us(5);
	GPIO_PinState bit = HAL_GPIO_ReadPin(ds18b20_GPIO_Port, ds18b20_Pin);
	dwt_delay_us(60-6);

	return bit;
}

uint8_t one_wire_read_byte()
{
	uint8_t byte = 0;
	for(int i = 0; i < 8; i++)
	{
		if (one_wire_read_bit() == GPIO_PIN_SET)
		{
			byte = byte | (1 << i);
		}
	}
	return byte;
}

void one_wire_reset()
{
	one_wire_force_down();
	dwt_delay_us(500);
	one_wire_release();
	dwt_delay_us(500);

}

void ds18b20_conv()
{
	one_wire_reset();
	one_wire_write_byte(0xCC);
	one_wire_write_byte(0x44);
}

float ds18b20_read_temp()
{
	one_wire_reset();
	one_wire_write_byte(0xCC);
	one_wire_write_byte(0xBE);
	uint8_t buff[8] = {0};
	for(int i = 0; i < 8; i++)
	{
		buff[i] = one_wire_read_byte();
	}

	return (int16_t)((buff[1] << 8) | buff[0]) / 16.0;
}

void ds18b20_init(ds18b20_bit_t conf)
{
	dwt_delay_init();
	one_wire_reset();
	one_wire_write_byte(0xCC);
	one_wire_write_byte(0x4E);
	one_wire_write_byte(0);
	one_wire_write_byte(0);
	one_wire_write_byte(conf);
}



















