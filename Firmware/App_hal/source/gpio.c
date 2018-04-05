#include "gpio.h"
void McuPowerInit(void)
{
	__GPIOF_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.Pin = GPIO_PIN_3;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);
}

void McuPowerEnable(unsigned char status)
{
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_3, status);
}