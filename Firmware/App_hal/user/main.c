#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "gprs.h"
#include "message.h"

#define START_TASK_PRIO    1          //任务优先级
#define START_STK_SIZE     128        //任务堆栈大小
TaskHandle_t StartTaskHanhler;        //任务句柄
void StartTask(void *pvParameters);   //任务函数

#define LED0_TASK_PRIO    4          //任务优先级
#define LED0_STK_SIZE     128        //任务堆栈大小
TaskHandle_t LED0TaskHanhler;        //任务句柄
void LED0Task(void *pArg);           //任务函数

#define Msg_TASK_PRIO    3          //任务优先级
#define Msg_STK_SIZE     128        //任务堆栈大小
TaskHandle_t MsgTaskHanhler;        //任务句柄

#define MsgSend_TASK_PRIO    5          //任务优先级
#define MsgSend_STK_SIZE     128        //任务堆栈大小
TaskHandle_t MsgSendTaskHanhler;        //任务句柄

int main()
{
	HAL_Init();                    	  
  Stm32_Clock_Init(RCC_PLL_MUL9);   	               		 
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	delay_init();
	LogInit(115200);
	Usart2Init(115200);
	UsartWrite("hello usart2\r\n");
	xTaskCreate( (TaskFunction_t) StartTask,       //任务函数
							 (const char*   ) "StartTask",     //任务名称
							 (uint16_t      ) START_STK_SIZE,  //任务堆栈大小
							 (void *        ) NULL,            //传递给任务函数的参数
							 (UBaseType_t   ) START_TASK_PRIO, //任务优先级
							 (TaskHandle_t* ) &StartTaskHanhler//任务句柄
	            );
	vTaskStartScheduler();
}

void StartTask(void * pvParameter)
{
	 taskENTER_CRITICAL();     //进入临界区
	    //创建LED0任务
	 
  	MsgInfoConfig("name","key","screat");
		xTaskCreate( (TaskFunction_t) MessageReceiveTask,       //任务函数
							 (const char*   ) "LED0Task",     //任务名称
							 (uint16_t      ) LED0_STK_SIZE,  //任务堆栈大小
							 (void *        ) NULL,            //传递给任务函数的参数
							 (UBaseType_t   ) LED0_TASK_PRIO, //任务优先级
							 (TaskHandle_t* ) &LED0TaskHanhler//任务句柄
	            );
		//创建LED1任务
		xTaskCreate( (TaskFunction_t) MessageSendTask,       //任务函数
							 (const char*   ) "MsgTask",     //任务名称
							 (uint16_t      ) Msg_STK_SIZE,  //任务堆栈大小
							 (void *        ) NULL,            //传递给任务函数的参数
							 (UBaseType_t   ) Msg_TASK_PRIO, //任务优先级
							 (TaskHandle_t* ) &MsgTaskHanhler//任务句柄
							 );
			xTaskCreate( (TaskFunction_t) LED0Task,       //任务函数
							 (const char*   ) "MsgSendTask",     //任务名称
							 (uint16_t      ) MsgSend_STK_SIZE,  //任务堆栈大小
							 (void *        ) NULL,            //传递给任务函数的参数
							 (UBaseType_t   ) MsgSend_TASK_PRIO, //任务优先级
							 (TaskHandle_t* ) &MsgSendTaskHanhler//任务句柄
	            );
		//删除开始任务
		vTaskDelete(StartTaskHanhler);
	  taskEXIT_CRITICAL();      //退出临界区
}
extern Gprs G510;
void LED0Task(void *pArg)
{
	char* buf = pvPortMalloc(120);
	while(1)
	{
	//	xQueueReceive(UsartRecMsgQueue,buf,portMAX_DELAY);
		  MessageSend("Test cmd  1\r\n");
		  MessageSend("Test cmd  2\r\n");
		  MessageSend("Test cmd  3\r\n");
		  MessageSend("Test cmd  4\r\n");
		memset(buf,0,120);
		vTaskDelay(3000);
	}
}
//void LED1Task(void *pArg)
//{	
//	u8   p[20];
//	u8 msgQRemainSize;   //消息队列剩余大小
//	u8 msgQTotalSize;    //消息队列总大小
//	while(1)
//	{
//		printf("hello\r\n");
//		vTaskDelay(500);
//		
//	}
//}