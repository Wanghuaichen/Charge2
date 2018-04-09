#include "charge.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "gprs.h"
#include "message.h"
#include "timers.h"
#include "flash.h"
#include "port.h"
#include <string.h>

#define CHARGE "charges"
#define PORT_ERROR "porterrors"
#define STOP_CHARGE "stopport"

QueueHandle_t ChaCmdQueue;   
void ChaCmdConfig(void)
{
		ChaCmdQueue = xQueueCreate(5,100);
}

void ChaCmdTask(void* pAgr)
{
	char* buf = pvPortMalloc(100);
	while(1)
	{
		memset(buf,0,100);
		xQueueReceive(ChaCmdQueue,buf,portMAX_DELAY);
		printf("ChaCmdBuf: %s\r\n",buf);
		if(NULL!=strstr(buf,(const char*)CHARGE))   /*充电指令*/
		{
			StartCharge(buf);	
		}
		if(NULL!=strstr(buf,(const char*)PORT_ERROR))    /*端口损坏*/
		{
			PortError(buf);
		}
		if(NULL!=strstr(buf,(const char*)STOP_CHARGE))    /*停止充电*/
		{
			StopCharge(buf);
		}
		vTaskDelay(100);
	}
	vPortFree(buf);
}