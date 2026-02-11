/*
 * ds18b20.c
 *

 */

#include "stm32f1xx.h"
#include "main.h"
#include "delay/dwt_delay.h"

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
		 dwt_delay_us(10);
		 one_wire_release();
		 dwt_delay_us(50);
	}



}

void one_wire_read_bit()
{
	int bit = HAL_GPIO_ReadPin(ds18b20_GPIO_Port, ds18b20_Pin);

	if(bit == RESET)
	{
		return 0;
	}
	else
	{
		return 1;
	}

}











