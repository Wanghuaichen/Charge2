#pragma once
#include "sys.h"
#include "FreeRTOS.h"
#include "semphr.h"

typedef struct GPRS {
	u8 status;
	u8 cmdNum;
	volatile u8 repNum;
	char ip[30];
    const  char *rep[12];
	const  char *cmd[12];
    char imei[20];
    char imsi[20];
    char csq[6];

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
	void (*Config)(const char *pimei,const char *pimsi,const char * pcsq);
	
	SemaphoreHandle_t GprsBinarySemaphore;
	
}Gprs;

extern int deviceConnect(void);
extern void deviceConfig(const char *pimei,const char *pimsi,const char * pcsq);