#include "devcmd.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "gprs.h"
#include "message.h"
#include "timers.h"
#include "active.h"
#include "fota.h"
#include <string.h>

QueueHandle_t DevCmdQueue;   
void DecCmdConfig(void)
{
		DevCmdQueue = xQueueCreate(5,100);
}

void DevCmdTask(void* pAgr)
{
	char* buf = pvPortMalloc(100);
	DecCmdConfig();
	while(1)
	{
		memset(buf,0,100);
		xQueueReceive(DevCmdQueue,buf,portMAX_DELAY);
		printf("DevCmdBuf: %s\r\n",buf);
		if(NULL!=strstr(buf,(const char*)ACTIVATE))   /*设备激活*/
		{
			ActivateDevice(buf);	
		}
		if(NULL!=strstr(buf,(const char*)RELEASE))    /*设备解绑*/
		{
			ReleaseDevice();
		}
		if(NULL!=strstr(buf,(const char*)FOTA_END))    /*设备更新*/
		{
			StartFota(buf);
		}
		vTaskDelay(100);
	}
	vPortFree(buf);
}