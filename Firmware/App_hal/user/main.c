#include "sys.h"
#include <string.h>
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
#include "csq.h"
#include "devcmd.h"
#include "net.h"
#include "charge.h"
#include "chargerep.h"
#include "storemoney.h"
#include "device.h"
#include "cost.h"


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
#define MsgSendTask_TASK_PRIO    3          //任务优先级
#define MsgSendTask_STK_SIZE     256        //任务堆栈大小
TaskHandle_t MsgSendTaskHanhler;        //任务句柄

/*设备参数修改任务*/
#define DeviceCmdTask_TASK_PRIO    2          //任务优先级
#define DeviceCmdTask_STK_SIZE     128        //任务堆栈大小
TaskHandle_t DeviceCmdTaskHanhler;            //任务句柄


/*充电端口相关任务*/
#define ChargeCmdTask_TASK_PRIO    2          //任务优先级
#define ChargeCmdTask_STK_SIZE     128        //任务堆栈大小
TaskHandle_t ChargeCmdTaskHanhler;            //任务句柄

/*充电回复检查相关任务*/
#define ChargeCheckTask_TASK_PRIO    1          //任务优先级
#define ChargeCheckTask_STK_SIZE     128        //任务堆栈大小
TaskHandle_t ChargeCheckTaskHanhler;            //任务句柄

/*充电完成回复检查相关任务*/
#define PostFinishTask_TASK_PRIO      2          //任务优先级
#define PostFinishTask_STK_SIZE     128        //任务堆栈大小
TaskHandle_t PostFinishTaskHanhler;            //任务句柄

extern Gprs G510;
xTimerHandle connectTimerHandler;
xTimerHandle testTimerHandler;
xTimerHandle CSQTimerHandler;
xTimerHandle NetTimerHandler;
xTimerHandle StoreTimerHandler;

void testTask(void *pArg);
int main()
{
	HAL_Init();                    	  
  Stm32_Clock_Init(RCC_PLL_MUL9);   	               		 
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	delay_init();
	LogInit(230400);
	Usart3Init(115200);
	McuPowerInit();
	McuPowerEnable(0);	
	delay_xms(500);
	McuPowerEnable(1);	
	//DeviceConfig();
	delay_xms(5000);
	
	printf("hello usart1\r\n");
	// V4.0 Dynamic Register branch
	while(1);
	char * f = "-0.23423";
	float tf = atof((const char *)f);
	printf("result %f \r\n",tf);

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
	 connectTimerHandler = xTimerCreate((const char *  )"OneShotTimer",
										(TickType_t    )2000,
										(UBaseType_t   )pdFALSE,
										(void *        )1,
										(TimerCallbackFunction_t)G510.Connect
										);
	 testTimerHandler = xTimerCreate(   (const char *  )"OneShotTimer",
										(TickType_t    )10000,
										(UBaseType_t   )pdTRUE,
										(void *        )2,
										(TimerCallbackFunction_t)testTask
										);	
	 NetTimerHandler  = xTimerCreate(   (const char *  )"NetTimer",
										(TickType_t    )10000,      /*24小时定时器*/
										(UBaseType_t   )pdTRUE,
										(void *        )3,
										(TimerCallbackFunction_t)NetCheck
										);	
	 CSQTimerHandler  = xTimerCreate(   (const char *  )"CSQTimer",
										(TickType_t    )86400000,      /*网络检查定时器*/
										(UBaseType_t   )pdTRUE,
									    (void *        )4,
										(TimerCallbackFunction_t)DeviceUploadCSQ
										);	
	 StoreTimerHandler  = xTimerCreate( (const char *  )"StoreTimer",
										(TickType_t    )180000,        /*存储当前余额*/
										(UBaseType_t   )pdTRUE,
									    (void *        )5,
										(TimerCallbackFunction_t)StoreMoneyTask
										);											
		MsgInfoConfig("a1BFMiIbqxv","devicename","dpuJrLdY54jJa6C8");
		G510.Config("a1BFMiIbqxv","devicename","TGMmysg7DXDBBYUGEeTzv6hcAae4z5M9");																
	  /*创建串口接收任务*/
  	xTaskCreate( (TaskFunction_t) MessageReceiveTask,       /*任务函数*/
							 (const char*   ) "LED0Task",                 /*任务名称*/
							 (uint16_t      ) MsgRecTask_STK_SIZE,        /*任务堆栈大小*/
							 (void *        ) NULL,                       /*传递给任务函数的参数*/
							 (UBaseType_t   ) MsgRecTask_TASK_PRIO,       /*任务优先级*/
							 (TaskHandle_t* ) &MsgRecTaskHanhler          /*任务句柄*/
	            );

	  	/*创建串口发送任务*/
		xTaskCreate( (TaskFunction_t) MessageSendTask,          /*任务函数*/
							 (const char*   ) "MsgTask",                  /*任务名称*/
							 (uint16_t      ) MsgSendTask_STK_SIZE,       /*任务堆栈大小*/
							 (void *        ) NULL,                       /*传递给任务函数的参数*/
							 (UBaseType_t   ) MsgSendTask_TASK_PRIO,      /*任务优先级*/
							 (TaskHandle_t* ) &MsgSendTaskHanhler         /*任务句柄*/
							 );
		 /*创建DeviceCmd任务*/
		xTaskCreate( (TaskFunction_t) DevCmdTask,                /*任务函数*/
							 (const char*   ) "DevCmdTask",                /*任务名称*/
							 (uint16_t      ) DeviceCmdTask_STK_SIZE,      /*任务堆栈大小*/
							 (void *        ) NULL,                        /*传递给任务函数的参数*/
							 (UBaseType_t   ) DeviceCmdTask_TASK_PRIO,     /*任务优先级*/
							 (TaskHandle_t* ) &DeviceCmdTaskHanhler        /*任务句柄*/
							 );
		 /*创建ChargeCmd任务*/
		xTaskCreate( (TaskFunction_t) ChaCmdTask,                /*任务函数*/
							 (const char*   ) "ChaCmdTask",                /*任务名称*/
							 (uint16_t      ) ChargeCmdTask_STK_SIZE,      /*任务堆栈大小*/
							 (void *        ) NULL,                        /*传递给任务函数的参数*/
							 (UBaseType_t   ) ChargeCmdTask_TASK_PRIO,     /*任务优先级*/
							 (TaskHandle_t* ) &ChargeCmdTaskHanhler        /*任务句柄*/
							 );
		 /*创建ChargeCheck任务*/
		xTaskCreate( (TaskFunction_t) ChaRepCheckTask,             /*任务函数*/
							 (const char*   ) "ChaCheckTask",                /*任务名称*/
							 (uint16_t      ) ChargeCheckTask_STK_SIZE,      /*任务堆栈大小*/
							 (void *        ) NULL,                          /*传递给任务函数的参数*/
							 (UBaseType_t   ) ChargeCheckTask_TASK_PRIO,     /*任务优先级*/
							 (TaskHandle_t* ) &ChargeCheckTaskHanhler        /*任务句柄*/
							 );
		 /*创建ChargeCheck任务*/
		xTaskCreate( (TaskFunction_t)PostFinishTask,             /*任务函数*/
							 (const char*   ) "PostFinishTask",                /*任务名称*/
							 (uint16_t      ) PostFinishTask_STK_SIZE,      /*任务堆栈大小*/
							 (void *        ) NULL,                          /*传递给任务函数的参数*/
							 (UBaseType_t   ) PostFinishTask_TASK_PRIO,     /*任务优先级*/
							 (TaskHandle_t* ) &PostFinishTaskHanhler        /*任务句柄*/
							 );
		xTimerStart(connectTimerHandler,portMAX_DELAY);
		xTimerStart(NetTimerHandler,portMAX_DELAY);	
    xTimerStart(CSQTimerHandler,portMAX_DELAY);
    xTimerStart(StoreTimerHandler,portMAX_DELAY);								 
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