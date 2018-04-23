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
#include  "gps.h"


/*开始任务*/
#define START_TASK_PRIO    1          //任务优先级
#define START_STK_SIZE     256        //任务堆栈大小
TaskHandle_t StartTaskHanhler;        //任务句柄
void StartTask(void *pvParameters);   //任务函数

xTimerHandle testTimerHandler;
void testTask(void *pArg);
int main()
{
 	HAL_Init();                    	  
  Stm32_Clock_Init(RCC_PLL_MUL9);   	               		 
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	delay_init();
	LogInit(230400);
	McuPowerInit();
	delay_xms(5000);
	
	printf("hello usart1\r\n");
	// weight


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
	

	/*触发ceshi定时器*/
	 testTimerHandler = xTimerCreate((const char *  )"OneShotTimer",
										(TickType_t    )5000,
										(UBaseType_t   )pdTRUE,
										(void *        )2,
										(TimerCallbackFunction_t)testTask
										);		
	 xTimerStart(testTimerHandler,portMAX_DELAY);	
									 
		/*删除开始任务*/
		vTaskDelete(StartTaskHanhler);
	  taskEXIT_CRITICAL();      /*退出临界区*/
}

void testTask(void *pArg)
{
	//MessageSend("hesldhf%&^zhxc123",1);
	
	// G510.UpdateIMEI();
}
