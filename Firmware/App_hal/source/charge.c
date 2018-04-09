#include "charge.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "gprs.h"
#include "message.h"
#include "timers.h"
#include "flash.h"


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
//		if(NULL!=strstr(buf,(const char*)ACTIVATE))   /*设备激活*/
//		{
//			ActivateDevice(buf);	
//		}
//		if(NULL!=strstr(buf,(const char*)RELEASE))    /*设备解绑*/
//		{
//			ReleaseDevice();
//		}
		vTaskDelay(100);
	}
	vPortFree(buf);
}