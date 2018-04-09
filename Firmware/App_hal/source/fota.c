#include "fota.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "gprs.h"
#include "message.h"
#include "timers.h"
#include "active.h"
#include "string.h"
#include "portaddr.h"
#include "cjson.h"


void SetFotaPara(u8 *buf)
{
	char * st = NULL;
	char * ed = NULL;
	if (NULL != strstr((const char *)buf, (const char *)FOTA_END))
	{
		printf("\r\n%s\r\n", buf);
		printf("fota suc\r\n");
		char * s = NULL;
		s = strstr((const char *)buf, (const char *)FOTA_STAT);
		s = s + strlen(FOTA_STAT);
		char * vision = pvPortMalloc(20*sizeof(char));
		char * socket = pvPortMalloc(30*sizeof(char));
		memset(vision, 0, 20);
		memset(socket, 0, 30);
		for (int i = 0; i < 4; i++)
		{
			vision[i] = *s;
			s++;
		}
		for (int i = 0; i < 22; i++)
		{
			socket[i] = *s;
			s++;
		}
		SetFotaSoftVision(vision);
		SetFotaIpSocket(socket);
		vPortFree(vision);
		vPortFree(socket);
	}
}
void SetFotaSoftVision(char* vision)
{
	char* Msg = pvPortMalloc(64);
	memset(Msg, 0, 64);
	strcpy(Msg, "visionstart");
	strcat(Msg, vision);
	strcat(Msg, "visionend");
	STMFLASH_Write(SOFT_VISION_ADDR, (u32*)Msg, 16);
	vPortFree(Msg);
}
void SetFotaIpSocket(char * socket)
{
	char* Msg = pvPortMalloc(64);
	memset(Msg, 0, 64);
	strcpy(Msg, "socketstart");
	strcat(Msg, socket);
	strcat(Msg, "socketend");
	STMFLASH_Write(UPDATE_SOCKET_ADDR, (u32*)Msg, 16);
	vPortFree(Msg);
}
void FotaRep(void)
{
	char * js = NULL;
	cJSON *actJson = cJSON_CreateObject();
  cJSON_AddStringToObject(actJson, "type", "fotaok");
	js = cJSON_Print(actJson); 
	cJSON_Delete(actJson);
	char * msg = pvPortMalloc(150);
	strcpy(msg, js);
	MessageSend(msg,1);
	vPortFree(msg);
}
void StartFota(u8 * buf)
{
	SetFotaPara(buf);
	printf("Starting Fota !!!\r\n");
	NVIC_SystemReset();
	while(1);
}