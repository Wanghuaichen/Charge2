#include "board.h"
#include "FreeRTOS.h "

#define  BOARD_IN   0
#define  BOARD_OUT  1

static u8 startPin = 0;
static u8 stopPin  = 0;
static u8 boardPortNum = 0;

void UpdateBoardPara(void)
{
	GPIO_InitTypeDef GPIO_Initure;
    
	__HAL_RCC_GPIOB_CLK_ENABLE();           
	__HAL_RCC_GPIOA_CLK_ENABLE();                    
    
	GPIO_Initure.Pin = GPIO_PIN_7;             
	GPIO_Initure.Mode = GPIO_MODE_INPUT;        
	GPIO_Initure.Pull = GPIO_PULLDOWN;           
	GPIO_Initure.Speed = GPIO_SPEED_HIGH;       
	HAL_GPIO_Init(GPIOB, &GPIO_Initure);            //board 1
	
	GPIO_Initure.Pin = GPIO_PIN_11;            
  GPIO_Initure.Mode = GPIO_MODE_INPUT;        
  GPIO_Initure.Pull = GPIO_PULLDOWN;            
  GPIO_Initure.Speed = GPIO_SPEED_HIGH;      
  HAL_GPIO_Init(GPIOA, &GPIO_Initure);            //board 2
	
	if((HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == BOARD_IN)&&(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == BOARD_IN))  // board 1,2
	{
		startPin = 0;
		stopPin = 20;
		boardPortNum = 11;
	}
	if ((HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == BOARD_IN)&&(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == BOARD_OUT))  // board 1
	{
		startPin = 0;
		stopPin = 10;
		boardPortNum = 10;
	}
	if ((HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == BOARD_OUT)&&(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == BOARD_IN))  // board 2
	{
		startPin = 10;
		stopPin = 20;
		boardPortNum = 1;
	}
	if ((HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == BOARD_OUT)&&(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == BOARD_OUT))  // board 2
	{
		startPin = 0;
		stopPin = 0;
		boardPortNum = 0;
	}
}

u8 GetStartPin(void)
{
	return startPin;
}
u8 GetStopPin(void)
{
	return stopPin;
}
u8 GetBoardPinNum(void)
{
	return boardPortNum;
}