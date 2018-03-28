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
	void (*send)(void);
}Mes;

extern void deviceSend(void);
extern void MsgInfoConfig(char *deviceName,char *productKey,char *deviceScreat);
extern int MessageSend(char *msg);
extern void MessageSendTask(void *pArg);
extern void MessageReceiveTask(void *pArg);