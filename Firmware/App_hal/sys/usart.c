#include "sys.h"
#include "usart.h"	

#if SYSTEM_SUPPORT_FREERTOS
#include "FreeRTOS.h"	
#include "queue.h"
#include "task.h"
#include "message.h"
#include "gps.h"
#endif

#if 1
#pragma import(__use_no_semihosting)             
             
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
 
void _sys_exit(int x) 
{ 
	x = x; 
} 

int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

UART_HandleTypeDef UART1_Handler; 


void LogInit(unsigned int  bound)
{	
	GPIO_InitTypeDef GPIO_Initure;

	__HAL_RCC_GPIOA_CLK_ENABLE();			 
	__HAL_RCC_USART1_CLK_ENABLE();			 
	
	GPIO_Initure.Pin = GPIO_PIN_9;			 
	GPIO_Initure.Mode = GPIO_MODE_AF_PP;		 
	GPIO_Initure.Pull = GPIO_PULLUP;			 
	GPIO_Initure.Speed = GPIO_SPEED_HIGH;		 
	HAL_GPIO_Init(GPIOA, &GPIO_Initure);	   	 

	GPIO_Initure.Pin = GPIO_PIN_10;			 
	HAL_GPIO_Init(GPIOA, &GPIO_Initure);	
 
	UART1_Handler.Instance = USART1;					  
	UART1_Handler.Init.BaudRate = bound;				   
	UART1_Handler.Init.WordLength = UART_WORDLENGTH_8B;   
	UART1_Handler.Init.StopBits = UART_STOPBITS_1;	    
	UART1_Handler.Init.Parity = UART_PARITY_NONE;		    
	UART1_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;    
	UART1_Handler.Init.Mode = UART_MODE_TX_RX;
	HAL_UART_Init(&UART1_Handler);
} 


void USART1_IRQHandler(void)                	
{ 
	HAL_UART_IRQHandler(&UART1_Handler);	
} 


