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



UART_HandleTypeDef UART2_Handler;
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
 
static u8 usart2RecBuf[usart2RecBufLen];     
volatile unsigned short int usart2RecLen = 0;  

void Usart2Init(unsigned int  bound)
{	
	GPIO_InitTypeDef GPIO_Initure;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();			 
	__HAL_RCC_USART2_CLK_ENABLE();			 
	
	GPIO_Initure.Pin = GPIO_PIN_2;			 
	GPIO_Initure.Mode = GPIO_MODE_AF_PP;		 
	GPIO_Initure.Pull = GPIO_PULLUP;			 
	GPIO_Initure.Speed = GPIO_SPEED_HIGH;		 
	HAL_GPIO_Init(GPIOA, &GPIO_Initure);	   	 

	GPIO_Initure.Pin = GPIO_PIN_3;		
	GPIO_Initure.Mode = GPIO_MODE_AF_INPUT;	//模式要设置为复用输入模式！			
	HAL_GPIO_Init(GPIOA, &GPIO_Initure);	
 
	UART2_Handler.Instance = USART2;					  
	UART2_Handler.Init.BaudRate = bound;				   
	UART2_Handler.Init.WordLength = UART_WORDLENGTH_8B;   
	UART2_Handler.Init.StopBits = UART_STOPBITS_1;	    
	UART2_Handler.Init.Parity = UART_PARITY_NONE;		    
	UART2_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;    
	UART2_Handler.Init.Mode = UART_MODE_TX_RX;
	HAL_UART_Init(&UART2_Handler);
	HAL_UART_Receive_IT(&UART2_Handler, (u8 *)usart2RecBuf, usart2RecBufLen); 
	HAL_NVIC_EnableIRQ(USART2_IRQn);			 
	HAL_NVIC_SetPriority(USART2_IRQn, 6, 0);
	__HAL_UART_CLEAR_IDLEFLAG(&UART2_Handler); 
	__HAL_UART_ENABLE_IT(&UART2_Handler, UART_IT_IDLE);

}

extern int i;
extern QueueHandle_t UsartRecMsgQueue;   //信息队列句柄
void USART2_IRQHandler(void)                	
{ 
	u8 Res;
	BaseType_t err;
	BaseType_t xHighPriorityTaskWoken;
	if ((__HAL_UART_GET_FLAG(&UART2_Handler, UART_FLAG_RXNE) != RESET))  
	{
		Res = USART2->DR;
		usart2RecBuf[usart2RecLen++] = Res;	 
	}
	if ((__HAL_UART_GET_FLAG(&UART2_Handler, UART_FLAG_IDLE) != RESET))  
	{
		__HAL_UART_CLEAR_IDLEFLAG(&UART2_Handler);  
		//MessageReceiveFromISR(usart2RecBuf);
	  if(UsartRecMsgQueue!=NULL)
		{
			err = xQueueSendFromISR(UsartRecMsgQueue,usart2RecBuf,&xHighPriorityTaskWoken);
			if(err!=pdTRUE)
			{
				printf("failed\r\n");
			}
			portYIELD_FROM_ISR(xHighPriorityTaskWoken);
		}
		usart2RecLen = 0;
		memset(usart2RecBuf,0,120);
	}
	HAL_UART_IRQHandler(&UART2_Handler);	
} 

void UsartWrite(const char *Data)
{
	int len = strlen(Data);
	for (int i = 0; i < len; i++)
	{
		while ((USART2->SR & 0X40) == 0)
			;
		USART2->DR = (u8) Data[i];      
	}
}

void UsartEnable(u8 usartid)
{
	if (usartid == 1)
	{
		
	}
	else if(usartid==2)
	{
		__HAL_UART_ENABLE(&UART2_Handler);
	}
}

void UsartDisable(u8 usartid)
{
	if (usartid == 1)
	{
		
	}
	else if (usartid == 2)
	{
		__HAL_UART_DISABLE(&UART2_Handler);
	}
}

void USART1_IRQHandler(void)                	
{ 
	HAL_UART_IRQHandler(&UART1_Handler);	
} 


