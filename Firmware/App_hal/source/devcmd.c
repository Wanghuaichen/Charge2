#include "devcmd.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "gprs.h"
#include "message.h"
#include "timers.h"
#include "active.h"

QueueHandle_t DevCmdQueue;   
void DecCmdConfig(void)
{
		DevCmdQueue = xQueueCreate(5,100);
}

void DevCmdTask(void* pAgr)
{
	char* buf = pvPortMalloc(100);
	while(1)
	{
		memset(buf,0,100);
		xQueueReceive(DevCmdQueue,buf,portMAX_DELAY);
		printf("DevCmdBuf: %s\r\n",buf);
		
		if(NULL!=strstr(buf,(const char*)ACTIVATE))   /*�豸����*/
		{
			ActivateDevice(buf);	
		}
		if(NULL!=strstr(buf,(const char*)RELEASE))   /*�豸���*/
		{
			ReleaseDevice();
		}
		vTaskDelay(100);
	}
	vPortFree(buf);
}