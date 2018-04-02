#pragma once
#include "sys.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"

typedef struct GPRS {
	u8 status;
	volatile u8 regStatus;
	volatile u8 cmdNum;
	volatile u8 cmdStartNumber;
	volatile u8 repNum;
	char ip[30];
    const  char *rep[15];
	const  char *cmd[15];
    char imei[20];
    char imsi[20];
    int csq;

	char  productKey[16];
	char  deviceName[20];
	char  deviceScreat[36];
	
	const u32 IPFlashAddr;
    const u32 OtaIPFlashAddr;
	int(*SetOtaIP)(char*ip);
	int(*SetIP)(char*ip);
	int(*UpdateIP)(void);
	int(*Connect)(void);
	void(*reset)(void);
	int(*CheckNet)(void);
	int(*UpdateCSQ)(void);
	void (*Config)(const char *pimei,const char *pimsi,const char * pcsq);
	
	SemaphoreHandle_t GprsBinarySemaphore;
	SemaphoreHandle_t CSQBinarySemaphore;
	SemaphoreHandle_t NetCheckBinarySemaphore;
	QueueHandle_t csqQueueHandler;
	
}Gprs;

extern int deviceConnect(void);
extern void deviceConfig(const char *pimei,const char *pimsi,const char * pcsq);
extern int deviceUpdateCSQ(void);