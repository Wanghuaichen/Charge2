#include "chargerep.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "gprs.h"
#include "message.h"
#include "timers.h"
#include <string.h>
#include "cjson.h"
#include "port.h"

#define CHARGE_BAD  "getbad"

QueueHandle_t ChaRepQueue;   
QueueHandle_t ChaRepCheckQueue;
void ChaRepConfig(void)
{
		ChaRepQueue = xQueueCreate(22,10);
	  ChaRepCheckQueue = xQueueCreate(3,120);
}

void ChaRepCheckTask(void* pAgr)
{
	char* buf = pvPortMalloc(120);
	ChaRepConfig();
	int port = 0;
	while(1)
	{
		memset(buf,0,120);
		xQueuePeek(ChaRepQueue,buf,portMAX_DELAY);
		printf("DevCmdBuf: %s\r\n",buf);
		vTaskDelay(1000);
		port = GetRepPort(buf);
		cJSON *res = cJSON_CreateObject();
		u8 chargecheck  = 0;   /*Get Charge Check*/
		if(chargecheck)
		{
				cJSON_AddStringToObject(res, "type", "chargeok");
		}
		else
		{
			  cJSON_AddStringToObject(res, "type", "chfailed");
		}
	  cJSON_AddNumberToObject(res, "port",port);
	  char * str= cJSON_Print(res);
		BaseType_t err;
		for(int i=0;i<3;i++)
		{
			MessageSend(str,1);
			memset(buf,0,120);
			err = xQueueReceive(ChaRepCheckQueue,buf,20000);
			printf("Check %d times\r\n",i);
			if(err == pdTRUE)
			{
				 int repPort = GetRepPort(buf);
				 if(repPort == port)
				 {
            xQueueReceive(ChaRepQueue,buf,0);
					  if(NULL!=strstr(buf,CHARGE_BAD))
						{
							PortStopCharge(repPort);
						}
					 	break;
				 }
			}
		}
	  cJSON_Delete(res); 	
		vTaskDelay(100);
	}
	vPortFree(buf);
}
int PostChaRep(int port)
{
	char * head = "sre";
	char * end = "dre";
	char num[3] = {0};
	num[0] = port/10 + '0';
	num[1] = port%10 + '0';
	num[2] = 0;
	char * msg = pvPortMalloc(10);
	memset(msg,0,10);
	strcpy(msg,head);
	strcat(msg,num);
	strcat(msg,num);
	if(ChaRepQueue!=NULL)
	{
		xQueueSend(ChaRepQueue,msg,0);
	}
	vPortFree(msg);
}
int GetRepPort(char* buf)
{
	char * head = "sre";
	char * end = "dre";
	int port;
  if(NULL != strstr(buf,end))
  {
	    if(NULL != strstr(buf,head))
      {
				char * result = strstr(buf,head);
				result += strlen(head);
				port = (*result - '0')*10 + (*(result+1)-'0');
				return port;
			}
  }
	return 0;
}