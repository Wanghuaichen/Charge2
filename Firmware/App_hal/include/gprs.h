#pragma once
#include "sys.h"
#include "FreeRTOS.h"
#include "semphr.h"

typedef struct GPRS {
	u8 status;
	u8 cmdId;
	char ip[30];
    char *rep[10];
	const u32 IPFlashAddr;
    const u32 OtaIPFlashAddr;
	int(*SetOtaIP)(char*ip);
	int(*SetIP)(char*ip);
	int(*UpdateIP)(void);
	int(*Connect)(void);
	void(*reset)(void);
	int(*CheckNet)(void);
	void (*Config)(struct GPRS * gprs);
	
	SemaphoreHandle_t ConnectBinarySemaphore;
	
}Gprs;

int deviceConnect(void);