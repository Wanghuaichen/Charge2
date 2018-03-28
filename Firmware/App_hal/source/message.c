#include <string.h>
#include "message.h"


 QueueHandle_t UsartRecMsgQueue;   //接收信息队列句柄
static QueueHandle_t UsartSenMsgQueue;   //接收信息队列句柄


static Mes Message; 
void MsgInfoConfig(char *deviceName,char *productKey,char *deviceScreat)
{
	UsartRecMsgQueue = xQueueCreate(10,120);
	UsartSenMsgQueue = xQueueCreate(8,200);
	Message.OKBinarySemaphore = xSemaphoreCreateBinary();
	Message.send = deviceSend;
	memset(Message.deviceName,0,20);
	memset(Message.productKey,0,16);
	memset(Message.deviceScreat,0,36);
	strcpy(Message.deviceName,deviceName);
	strcpy(Message.productKey,productKey);
	strcpy(Message.deviceScreat,deviceScreat);
}
int MessageSend(char *msg)
{
	BaseType_t err;
	char * Msg = pvPortMalloc(200);
	strcpy(Msg,msg);
	if(UsartSenMsgQueue!=NULL)
	{
		err = xQueueSend(UsartSenMsgQueue,Msg,portMAX_DELAY);
		if(err!=pdTRUE)
		{
			printf("Message failed\r\n");
			return -1;
		}
	}
	vPortFree(Msg);
	return 1;
}
int MessageSendFromISR(char *msg)
{
	BaseType_t err;
	BaseType_t xHighPriorityTaskWoken;
	char * Msg = pvPortMalloc(200);
	strcpy(Msg,msg);
	if(UsartSenMsgQueue!=NULL)
	{
		err = xQueueSendFromISR(UsartSenMsgQueue,Msg,&xHighPriorityTaskWoken);
		if(err!=pdTRUE)
		{
			printf("failed\r\n");
			return -1;
		}
		portYIELD_FROM_ISR(xHighPriorityTaskWoken);
	}
	vPortFree(Msg);
}
void MessageSendTask(void *pArg)
{
	char* buf = pvPortMalloc(200);
	u8 i = 0;
	while(1)
	{
		xQueueReceive(UsartSenMsgQueue,buf,portMAX_DELAY);
		Message.payLoad = buf;
		Message.send();
		BaseType_t err = pdFALSE;
		i = 0;
		while(1)
		{
			i++;
			if(Message.OKBinarySemaphore!=NULL)
			{
			   err = xSemaphoreTake(Message.OKBinarySemaphore,5000);
				{
					printf("%d\r\n",i);
					if(err == pdTRUE)
					{
						break;
					}
				}
			}
			if(i>=3)
			{
				printf("NO OK\r\n");
				break;
			}
		}
		memset(buf,0,200);
		vTaskDelay(100);
	}
}

void deviceSend(void)
{
	char *cmd = pvPortMalloc(300);
	memset(cmd,0,300);
	strcpy(cmd,"AT+");
	strcat(cmd,Message.deviceName);
	strcat(cmd,"/");
	strcat(cmd,Message.productKey);
	strcat(cmd,"/");
	strcat(cmd,Message.deviceScreat);
	strcat(cmd,"/");
	strcat(cmd,Message.payLoad);
	UsartWrite((uint8_t*)cmd);
	printf("cmd %s\r\n",cmd);
	vPortFree(cmd);
}

void MessageReceiveTask(void *pArg)   //命令解析任务
{
	char* buf = pvPortMalloc(120);
	while(1)
	{
		xQueueReceive(UsartRecMsgQueue,buf,portMAX_DELAY);
		printf("Get buf %s\r\n",buf);
		if(NULL != strstr(buf,"OK"))
		{
		   xSemaphoreGive(Message.OKBinarySemaphore);
		}
		vTaskDelay(100);
	}
}

int MessageReceiveFromISR(char *msg)
{
		u8 Res;
	  BaseType_t err;
	  BaseType_t xHighPriorityTaskWoken;
		if(UsartRecMsgQueue!=NULL)
		{
			err = xQueueSendFromISR(UsartRecMsgQueue,msg,&xHighPriorityTaskWoken);
			if(err!=pdTRUE)
			{
				printf("failed\r\n");
			}
			portYIELD_FROM_ISR(xHighPriorityTaskWoken);
		}
}