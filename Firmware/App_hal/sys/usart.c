#include "sys.h"
#include "usart.h"	

#if SYSTEM_SUPPORT_FREERTOS
#include "FreeRTOS.h"	
#include "queue.h"
#include "task.h"
#include "message.h"
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



UART_HandleTypeDef UART3_Handler;
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
 
static u8 usart3RecBuf[usart3RecBufLen];     
volatile unsigned short int usart3RecLen = 0;  

void Usart3Init(unsigned int  bound)
{	
	GPIO_InitTypeDef GPIO_Initure;
	
	__HAL_RCC_GPIOB_CLK_ENABLE();			 
	__HAL_RCC_USART3_CLK_ENABLE();			 
	
	GPIO_Initure.Pin = GPIO_PIN_10;			 
	GPIO_Initure.Mode = GPIO_MODE_AF_PP;		 
	GPIO_Initure.Pull = GPIO_PULLUP;			 
	GPIO_Initure.Speed = GPIO_SPEED_HIGH;		 
	HAL_GPIO_Init(GPIOB, &GPIO_Initure);	   	 

	GPIO_Initure.Pin = GPIO_PIN_11;		
	GPIO_Initure.Mode = GPIO_MODE_AF_INPUT;	//模式要设置为复用输入模式！			
	HAL_GPIO_Init(GPIOB, &GPIO_Initure);	
 
	UART3_Handler.Instance = USART3;					  
	UART3_Handler.Init.BaudRate = bound;				   
	UART3_Handler.Init.WordLength = UART_WORDLENGTH_8B;   
	UART3_Handler.Init.StopBits = UART_STOPBITS_1;	    
	UART3_Handler.Init.Parity = UART_PARITY_NONE;		    
	UART3_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;    
	UART3_Handler.Init.Mode = UART_MODE_TX_RX;
	HAL_UART_Init(&UART3_Handler);
	HAL_UART_Receive_IT(&UART3_Handler, (u8 *)usart3RecBuf, usart3RecBufLen); 
	HAL_NVIC_EnableIRQ(USART3_IRQn);			 
	HAL_NVIC_SetPriority(USART3_IRQn, 6, 0);
	__HAL_UART_CLEAR_IDLEFLAG(&UART3_Handler); 
	__HAL_UART_ENABLE_IT(&UART3_Handler, UART_IT_IDLE);

}

extern int i;
extern QueueHandle_t UsartRecMsgQueue;   //信息队列句柄
void USART3_IRQHandler(void)                	
{ 
	u8 Res;
	BaseType_t err;
	BaseType_t xHighPriorityTaskWoken;
	if ((__HAL_UART_GET_FLAG(&UART3_Handler, UART_FLAG_RXNE) != RESET))  
	{
		Res = USART3->DR;
		usart3RecBuf[usart3RecLen++] = Res;	 
	}
	if ((__HAL_UART_GET_FLAG(&UART3_Handler, UART_FLAG_IDLE) != RESET))  
	{
		__HAL_UART_CLEAR_IDLEFLAG(&UART3_Handler);  
		MessageReceiveFromISR(usart3RecBuf);
		usart3RecLen = 0;
		memset(usart3RecBuf,0,120);
	}
	HAL_UART_IRQHandler(&UART3_Handler);	
} 

void UsartWrite(const char *Data)
{
	int len = strlen(Data);
	for (int i = 0; i < len; i++)
	{
		while ((USART3->SR & 0X40) == 0)
			;
		USART3->DR = (u8) Data[i];      
	}
}

void UsartEnable(u8 usartid)
{
	if (usartid == 1)
	{
		
	}
	else if(usartid==2)
	{
		__HAL_UART_ENABLE(&UART3_Handler);
	}
}

void UsartDisable(u8 usartid)
{
	if (usartid == 1)
	{
		
	}
	else if (usartid == 2)
	{
		__HAL_UART_DISABLE(&UART3_Handler);
	}
}

void USART1_IRQHandler(void)                	
{ 
	HAL_UART_IRQHandler(&UART1_Handler);	
} 


