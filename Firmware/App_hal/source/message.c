#include <string.h>
#include "message.h"
#include "gprs.h"
#include "register.h"
#include "csq.h"
#include "devcmd.h"
#include "charge.h"


QueueHandle_t UsartRecMsgQueue;   //接收信息队列句柄
static QueueHandle_t UsartSenMsgQueue;   //接收信息队列句柄
extern Gprs G510;
extern SemaphoreHandle_t RigisterBinarySemaphore;
extern SemaphoreHandle_t CSQBinarySemaphore;
extern QueueHandle_t DevCmdQueue;   
extern QueueHandle_t ChaCmdQueue;

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
int MessageSend(const char *msg,u8 head)
{
	BaseType_t err;
	char * Msg = pvPortMalloc(200);
	memset(Msg,0,200);
	if(head==0)
	{
	  strcpy(Msg,"AT-");
		strcat(Msg,msg);
	}
	else if(head==1)
	{
		strcpy(Msg,msg);
	}
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
int MessageSendFromISR(char *msg,u8 at)
{
	BaseType_t err;
	BaseType_t xHighPriorityTaskWoken;
	char * Msg = pvPortMalloc(200);
  if(at==1)
	{
	  strcpy(Msg,"AT");
		strcat(Msg,msg);
	}
	else if(at==0)
	{
		strcpy(Msg,msg);
	}
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

		if(NULL!=strstr(buf,"AT-"))
		{
			char * result = strstr(buf,"AT-");
			result+=3;
			Message.payLoad = result;
			Message.send(0);
		}
		else
		{
			Message.payLoad = buf;
			Message.send(1);
		}
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
/*消息发送注册函数*/
void deviceSend(u8 head)
{
	char *cmd = pvPortMalloc(200);
	char *len = pvPortMalloc(6);
	if(head==1)
	{
		memset(cmd,0,200);
		memset(len,0,6);
		sprintf(len, "%d", strlen(Message.payLoad));
		strcpy(cmd,"AT+CLOUDPUB=\"/");
		strcat(cmd,Message.productKey);
		strcat(cmd,"/");
		strcat(cmd,Message.deviceName);
		strcat(cmd,"/data\",0,");
		strcat(cmd,len);
		strcat(cmd,"\r\n");
		UsartWrite((uint8_t*)cmd);
		vTaskDelay(100);
		UsartWrite((uint8_t*)Message.payLoad);
#ifdef 	DEBUG
	  printf("send message: %s",cmd);
		printf("%s\r\n",Message.payLoad);
#endif
	}
	else
	{
		memset(cmd,0,200);
		strcpy(cmd,Message.payLoad);
		UsartWrite((uint8_t*)cmd);
#ifdef 	DEBUG
	  printf("send message: %s",cmd);
		printf("%s\r\n",Message.payLoad);
#endif
		
	}
	vPortFree(len);
	vPortFree(cmd);
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
void MessageReceiveTask(void *pArg)   //命令解析任务
{
	char* buf = pvPortMalloc(120);
	char* rep = pvPortMalloc(60);
	while(1)
	{
		xQueueReceive(UsartRecMsgQueue,buf,portMAX_DELAY);
		printf("Get buf %s\r\n",buf);
		if(G510.connectFlag == 1)
		{
			if(xQueuePeek(G510.GprsRepQueue,rep,0)==pdTRUE)
			{
				if(NULL != strstr(buf,rep))
				{
					xQueueReceive(G510.GprsRepQueue,rep,0);
					xSemaphoreGive(G510.GprsConnectBinarySemaphore);
				}
			}
	  }
		if(G510.csqFlag == 1)
		{
			if(xQueuePeek(G510.GprsRepQueue,rep,0)==pdTRUE)
			{
				if(NULL != strstr(buf,rep))
				{
					xQueueReceive(G510.GprsRepQueue,rep,0);
					if(G510.GprsRepQueue!=NULL)
	        {
			       xQueueOverwrite(G510.CSQRepQueue,buf);
  	      }
					xSemaphoreGive(G510.GprsCSQBinarySemaphore);
				}
			}
	  }
		if(G510.imeiFlag == 1)
		{
			if(xQueuePeek(G510.GprsRepQueue,rep,0)==pdTRUE)
			{
				if(NULL != strstr(buf,rep))
				{
					xQueueReceive(G510.GprsRepQueue,rep,0);
					if(G510.IMEIRepQueue!=NULL)
	        {
			       xQueueOverwrite(G510.IMEIRepQueue,buf);
  	      }
					xSemaphoreGive(G510.IMEIBinarySemaphore);
				}
			}
	  }
		if(G510.imsiFlag == 1)
		{
			if(xQueuePeek(G510.GprsRepQueue,rep,0)==pdTRUE)
			{
				if(NULL != strstr(buf,rep))
				{
					xQueueReceive(G510.GprsRepQueue,rep,0);
					if(G510.IMSIRepQueue!=NULL)
	        {
			       xQueueOverwrite(G510.IMSIRepQueue,buf);
  	      }
					xSemaphoreGive(G510.IMSIBinarySemaphore);
				}
			}
	  }
		if(RigisterBinarySemaphore!=NULL)              /*注册回复*/
		{  
			 if(NULL != strstr(buf,REGISTER_REP))
		   {
				 if(RigisterBinarySemaphore!=NULL)
		     xSemaphoreGive(RigisterBinarySemaphore);
			 }
		}
		if(CSQBinarySemaphore!=NULL)                   /*CSQ回复*/
		{  
			 if(NULL != strstr(buf,CSQ_REP))
		   {
				 if(CSQBinarySemaphore!=NULL)
		     xSemaphoreGive(CSQBinarySemaphore);
			 }
		} 
		if(NULL != strstr(buf,DEV_CMD))                /*devcmd Queue*/
		{
			 if(DevCmdQueue!=NULL)
			 xQueueSend(DevCmdQueue,buf,0);
		}
		if(NULL != strstr(buf,DEV_CMD))                /*chacmd Queue*/
		{
			 if(ChaCmdQueue!=NULL)
			 xQueueSend(ChaCmdQueue,buf,0);
		}
		if(NULL != strstr(buf,"OK"))
		{
		   xSemaphoreGive(Message.OKBinarySemaphore);
		}
		
		vTaskDelay(100);
	}
	vPortFree(buf);
	vPortFree(rep);
}