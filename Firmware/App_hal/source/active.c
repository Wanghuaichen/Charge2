#include "active.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "string.h"
#include "portaddr.h"
#include "cjson.h"
#include "message.h"


#define deviceDelete       "delete"
#define deviceActive       "active"
#define deviceActiveHead   "devactives"
#define deviceActiveEnd    "devactived"

void ReleaseDevice(void)
{
	printf("release device\r\n");
	
	char * js = NULL;
	cJSON *actJson = cJSON_CreateObject();
	cJSON_AddStringToObject(actJson, "type", "delok");
	js = cJSON_Print(actJson); 
	cJSON_Delete(actJson);
	char * msg = pvPortMalloc(150);
	memset(msg,0,150);
	strcpy(msg, js);
	MessageSend(msg,1);
	vPortFree(msg);
}

void ActivateDevice(char *buf)
{
	printf("activate device\r\n");
	
	if (NULL != strstr((const char *)buf, (const char *)deviceActiveEnd))
	{
		printf("\r\n%s\r\n", buf);
		printf("active suc\r\n");
		char * s = NULL;
		s = strstr((const char *)buf, (const char *)deviceActiveHead);
		s = s + strlen(deviceActiveHead);
		char * curflag = pvPortMalloc(64*sizeof(char));
		memset(curflag, 0, 64);
		char * preflag = pvPortMalloc(64*sizeof(char));
		memset(preflag, 0, 64);
		char * cur = pvPortMalloc(64*sizeof(char));
		memset(cur, 0, 64);
		char * pre = pvPortMalloc(64*sizeof(char));
		memset(pre, 0, 64);
		for (int i = 0; i < 4; i++)
		{
			cur[i] = *s;
			s++;
		}
		for (int i = 0; i < 3; i++)
		{
			pre[i] = *s;
			s++;
		}
		strcpy(curflag, "current");
		strcat(curflag, cur);
		strcat(curflag, "status");
		strcpy(preflag, "price");
		strcat(preflag, pre);
		strcat(preflag, "status");
		STMFLASH_Write(CURRENT_MAX_ADDR, (u32*)curflag, 10);
		STMFLASH_Write(PRICE_ADDR, (u32*)preflag, 10);
		STMFLASH_Write(DEVICE_ACTIVE_STATUS_ADDR, (u32*)deviceActive, 10);
		vPortFree(cur);
		vPortFree(curflag);
		vPortFree(pre);
		vPortFree(preflag);
		RepActive();
		printf("\r\nrep ACT\r\n");
		return;    
	}
}

void RepActive(void)
{
 	char * js = NULL;
 	cJSON *actJson = cJSON_CreateObject();
	cJSON_AddStringToObject(actJson, "type", "actrep");
	js = cJSON_Print(actJson); 
	cJSON_Delete(actJson);
 	char * msg = pvPortMalloc(150);
	memset(msg,0,150);
 	strcpy(msg, js);
 	MessageSend(msg,1);
 	vPortFree(msg);
}