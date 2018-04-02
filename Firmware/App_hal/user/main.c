#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "gprs.h"
#include "message.h"
#include "timers.h"
#include "flash.h"

/*开始任务*/
#define START_TASK_PRIO    1          //任务优先级
#define START_STK_SIZE     256        //任务堆栈大小
TaskHandle_t StartTaskHanhler;        //任务句柄
void StartTask(void *pvParameters);   //任务函数

/*串口接收队列解析任务*/
#define MsgRecTask_TASK_PRIO    5                    //任务优先级
#define MsgRecTask_STK_SIZE     256                  //任务堆栈大小
TaskHandle_t MsgRecTaskHanhler;                  //任务句柄
//回调函数在message.c中定义，创建任务时注册          //任务函数

/*联网串口接收队列解析任务*/
#define NetMsgRecTask_TASK_PRIO    4                    //任务优先级
#define NetMsgRecTask_STK_SIZE     256                  //任务堆栈大小
TaskHandle_t NetMsgRecTaskHanhler;                  //任务句柄
//回调函数在message.c中定义，创建任务时注册          //任务函数

/*数据包封装串口发送*/
#define MsgSend_TASK_PRIO    5          //任务优先级
#define MsgSend_STK_SIZE     256        //任务堆栈大小
TaskHandle_t MsgSendTaskHanhler;        //任务句柄


/*联网包串口发送,与封装任务互斥运行*/
#define NetMsgSend_TASK_PRIO    3          //任务优先级
#define NetMsgSend_STK_SIZE     256        //任务堆栈大小
TaskHandle_t NetMsgSendTaskHanhler;        //任务句柄

extern Gprs G510;
xTimerHandle netTimerHandler;

xTimerHandle testTimerHandler;
void TestTask(void);
int main()
{
	HAL_Init();                    	  
  Stm32_Clock_Init(RCC_PLL_MUL9);   	               		 
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	delay_init();
	LogInit(115200);
	Usart2Init(115200);
	//UsartWrite("hello usart2\r\n");
	printf("hello usart1\r\n");
	
	char * buf = pvPortMalloc(32);
	memset(buf,0,32);
	//STMFLASH_Write(0X0807D000,(u32*)"hello huka",8);
	STMFLASH_Read(0X0807D000,(u32*)buf,8);
	
	printf("buf %s\r\n",buf);
	

	xTaskCreate( (TaskFunction_t) StartTask,         /*任务函数*/
							 (const char*   ) "StartTask",       /*任务名称*/
							 (uint16_t      ) START_STK_SIZE,    /*任务堆栈大小*/
							 (void *        ) NULL,              /*传递给任务函数的参数*/
							 (UBaseType_t   ) START_TASK_PRIO,   /*任务优先级*/
							 (TaskHandle_t* ) &StartTaskHanhler  /*任务句柄*/
	            );
	vTaskStartScheduler();
}

void StartTask(void * pvParameter)
{
	 taskENTER_CRITICAL();     /*进入临界区*/
	
	/*触发联网定时器*/
	 netTimerHandler = xTimerCreate(  (const char *  )"OneShotTimer",
																		(TickType_t    )2000,
																		(UBaseType_t   )pdFALSE,
																	  (void *        )1,
																		(TimerCallbackFunction_t)G510.Connect
																		);
		

	 testTimerHandler = xTimerCreate(  (const char *  )"testTimer",
																		(TickType_t    )3000,
																		(UBaseType_t   )pdTRUE,
																	  (void *        )2,
																		(TimerCallbackFunction_t)TestTask
																		);

																		
		MsgInfoConfig("C8OzD6Pkm9V","devicename","TGMmysg7DXDBBYUGEeTzv6hcAae4z5M9");
		G510.Config("C8OzD6Pkm9V","devicename","TGMmysg7DXDBBYUGEeTzv6hcAae4z5M9");																
	  /*创建串口接收任务*/
		xTaskCreate( (TaskFunction_t) MessageReceiveTask,       /*任务函数*/
							 (const char*   ) "LED0Task",                 /*任务名称*/
							 (uint16_t      ) MsgRecTask_STK_SIZE,        /*任务堆栈大小*/
							 (void *        ) NULL,                       /*传递给任务函数的参数*/
							 (UBaseType_t   ) MsgRecTask_TASK_PRIO,       /*任务优先级*/
							 (TaskHandle_t* ) &MsgRecTaskHanhler          /*任务句柄*/
	            );
		/*创建联网串口接收任务*/
		xTaskCreate( (TaskFunction_t) NetMessageReceiveTask,       /*任务函数*/
							 (const char*   ) "NetRecTask",                 /*任务名称*/
							 (uint16_t      ) NetMsgRecTask_STK_SIZE,        /*任务堆栈大小*/
							 (void *        ) NULL,                       /*传递给任务函数的参数*/
							 (UBaseType_t   ) NetMsgRecTask_TASK_PRIO,       /*任务优先级*/
							 (TaskHandle_t* ) &NetMsgRecTaskHanhler          /*任务句柄*/
	            );
	  	/*创建串口发送任务*/
		xTaskCreate( (TaskFunction_t) MessageSendTask,       /*任务函数*/
							 (const char*   ) "MsgTask",     /*任务名称*/
							 (uint16_t      ) MsgSend_STK_SIZE,  /*任务堆栈大小*/
							 (void *        ) NULL,            /*传递给任务函数的参数*/
							 (UBaseType_t   ) MsgSend_TASK_PRIO, /*任务优先级*/
							 (TaskHandle_t* ) &MsgSendTaskHanhler/*任务句柄*/
							 );
		  /*创建联网串口发送任务*/
		xTaskCreate( (TaskFunction_t) NetMessageSendTask,       /*任务函数*/
							 (const char*   ) "NetMsgTask",     /*任务名称*/
							 (uint16_t      ) NetMsgSend_STK_SIZE,  /*任务堆栈大小*/
							 (void *        ) NULL,            /*传递给任务函数的参数*/
							 (UBaseType_t   ) NetMsgSend_TASK_PRIO, /*任务优先级*/
							 (TaskHandle_t* ) &NetMsgSendTaskHanhler/*任务句柄*/
							 );
		
		vTaskSuspend(NetMsgSendTaskHanhler);            /*挂起联网串口发送任务*/
		vTaskSuspend(NetMsgRecTaskHanhler);            /*挂起联网串口解析任务*/
		xTimerStart(netTimerHandler,portMAX_DELAY);
		/*删除开始任务*/
		vTaskDelete(StartTaskHanhler);
	  taskEXIT_CRITICAL();      /*退出临界区*/
}

//void LED0Task(void *pArg)
//{
//	char* buf = pvPortMalloc(120);
//	while(1)
//	{
//	//	xQueueReceive(UsartRecMsgQueue,buf,portMAX_DELAY);
//		  MessageSend("Test cmd  1\r\n");
//		  MessageSend("Test cmd  2\r\n");
//		  MessageSend("Test cmd  3\r\n");
//		  MessageSend("Test cmd  4\r\n");
//		memset(buf,0,120);
//		vTaskDelay(3000);
//	}
//}
void TestTask(void)
{	
		printf("hello\r\n");
	  G510.UpdateCSQ();
	//	MessageSend("hello");
}