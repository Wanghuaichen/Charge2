#pragma once 
#include "FreeRTOS.h"
#include "sys.h"
#include "queue.h"
#include "task.h"
#include "semphr.h"


typedef struct MESSAGE
{
	char  deviceScreat[36];
	char  productKey[16];
	char  deviceName[20];
	const char  *payLoad;
	
	SemaphoreHandle_t OKBinarySemaphore;
	void (*send)(u8 head);
}Mes;

extern void deviceSend(u8 head);
extern void MsgInfoConfig(char *productKey,char *deviceName,char *deviceScreat);
extern int  MessageSend(const char *msg,u8 at);

extern void MessageSendTask(void *pArg);
extern int  MessageSendFromISR(char *msg,u8 at);

extern void MessageReceiveTask(void *pArg);
