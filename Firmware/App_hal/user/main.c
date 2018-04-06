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
#include "gpio.h"

/*开始任务*/
#define START_TASK_PRIO    1          //任务优先级
#define START_STK_SIZE     256        //任务堆栈大小
TaskHandle_t StartTaskHanhler;        //任务句柄
void StartTask(void *pvParameters);   //任务函数

/*串口接收队列解析任务*/
#define MsgRecTask_TASK_PRIO    4                    //任务优先级
#define MsgRecTask_STK_SIZE     256                  //任务堆栈大小
TaskHandle_t MsgRecTaskHanhler;                  //任务句柄
//回调函数在message.c中定义，创建任务时注册          //任务函数


/*数据包封装串口发送*/
#define MsgSend_TASK_PRIO    3          //任务优先级
#define MsgSend_STK_SIZE     256        //任务堆栈大小
TaskHandle_t MsgSendTaskHanhler;        //任务句柄


extern Gprs G510;
xTimerHandle netTimerHandler;
xTimerHandle testTimerHandler;

void testTask(void *pArg);
int main()
{
	HAL_Init();                    	  
    Stm32_Clock_Init(RCC_PLL_MUL9);   	               		 
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	delay_init();
	LogInit(115200);
	Usart2Init(115200);
	McuPowerInit();
	McuPowerEnable(0);	
	delay_xms(500);
	McuPowerEnable(1);	
	delay_xms(5000);
	printf("hello usart1\r\n");
	// V3.0.0
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
	 testTimerHandler = xTimerCreate(  (const char *  )"OneShotTimer",
																		(TickType_t    )10000,
																		(UBaseType_t   )pdTRUE,
																	  (void *        )2,
																		(TimerCallbackFunction_t)testTask
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

	  	/*创建串口发送任务*/
		xTaskCreate( (TaskFunction_t) MessageSendTask,       /*任务函数*/
							 (const char*   ) "MsgTask",     /*任务名称*/
							 (uint16_t      ) MsgSend_STK_SIZE,  /*任务堆栈大小*/
							 (void *        ) NULL,            /*传递给任务函数的参数*/
							 (UBaseType_t   ) MsgSend_TASK_PRIO, /*任务优先级*/
							 (TaskHandle_t* ) &MsgSendTaskHanhler/*任务句柄*/
							 );
		xTimerStart(netTimerHandler,portMAX_DELAY);
						
		/*删除开始任务*/
		vTaskDelete(StartTaskHanhler);
	  taskEXIT_CRITICAL();      /*退出临界区*/
}

void testTask(void *pArg)
{
	MessageSend("hello",1);
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