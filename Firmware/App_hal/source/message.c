#include <string.h>
#include "message.h"
#include "gprs.h"


QueueHandle_t UsartRecMsgQueue;   //接收信息队列句柄
static QueueHandle_t UsartSenMsgQueue;   //接收信息队列句柄
extern Gprs G510;

static Mes Message; 
void MsgInfoConfig(char *productKey,char *deviceName,char *deviceScreat)
{
	UsartRecMsgQueue = xQueueCreate(10,120);
	UsartSenMsgQueue = xQueueCreate(8,200);
	Message.OKBinarySemaphore = xSemaphoreCreateBinary();
	Message.send = deviceSend;

	memset(Message.productKey,0,16);
	memset(Message.deviceName,0,20);
	memset(Message.deviceScreat,0,36);
	
	strcpy(Message.productKey,productKey);
	strcpy(Message.deviceName,deviceName);
	strcpy(Message.deviceScreat,deviceScreat);
}
/*将PayLoad数据包发送到发送队列中*/
int MessageSend(const char *msg)
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
/*在中断中将PayLoad数据包发送到发送队列中*/
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
			printf("usart send add queue failed\r\n");
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
		memset(buf,0,200);
		xQueueReceive(UsartSenMsgQueue,buf,portMAX_DELAY);
		Message.payLoad = buf;
		Message.send(1);
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
			if(i>=2)
			{
				break;
			}
		}
		vTaskDelay(100);
	}
	vPortFree(buf);
}
void NetMessageSendTask(void *pArg)
{
  char* buf = pvPortMalloc(200);
	while(1)
	{
		memset(buf,0,200);
		xQueueReceive(UsartSenMsgQueue,buf,portMAX_DELAY);
		Message.payLoad = buf;
		Message.send(0);
		vTaskDelay(100);
	}
	vPortFree(buf);
}
/*消息发送注册函数*/
void deviceSend(u8 head)
{
	if(head==1)
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
	else
	{
		char *cmd = pvPortMalloc(300);
		memset(cmd,0,300);
		strcpy(cmd,Message.payLoad);
		UsartWrite((uint8_t*)cmd);
		printf("cmd %s\r\n",cmd);
		vPortFree(cmd);
	}
}

void NetMessageReceiveTask(void *pArg)   //命令解析任务
{
	char* buf = pvPortMalloc(120);
	while(1)
	{
		xQueueReceive(UsartRecMsgQueue,buf,portMAX_DELAY);
		printf("Get buf %s\r\n",buf);
		if(NULL != strstr(buf,G510.rep[G510.repNum]))
		{
		   xSemaphoreGive(G510.GprsBinarySemaphore);
		}
		vTaskDelay(100);
	}
	vPortFree(buf);
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
	vPortFree(buf);
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
				printf("usart idel add queue failed\r\n");
			}
			portYIELD_FROM_ISR(xHighPriorityTaskWoken);
		}
}