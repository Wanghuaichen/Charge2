#pragma once
#include "sys.h"
#include "FreeRTOS.h"
#include "semphr.h"

typedef struct GPRS {
	u8 cmdNum;
	volatile u8 connectFlag;
	volatile u8 csqFlag;
	volatile u8 netFlag;
	volatile u8 imeiFlag;
	volatile u8 imsiFlag;
	char ip[30];
  const  char *rep[15];
	const  char *cmd[15];
  volatile char imei[20];
  volatile char imsi[20];
  volatile int csq;

	char  productKey[16];
	char  deviceName[20];
	char  deviceScreat[36];
	
	const u32 IPFlashAddr;
  const u32 OtaIPFlashAddr;
	int(*SetOtaIP)(char*ip);
	int(*SetIP)(char*ip);
	int(*UpdateIP)(void);
	int(*UpdateCSQ)(void);
	int(*UpdateIMEI)(void);
	int(*UpdateIMSI)(void);
	int(*Connect)(void);
	void(*reset)(void);
	int(*CheckNet)(void);
	void (*Config)(const char *pimei,const char *pimsi,const char * pcsq);
	
	SemaphoreHandle_t GprsConnectBinarySemaphore;
	SemaphoreHandle_t GprsCSQBinarySemaphore;
	SemaphoreHandle_t IMEIBinarySemaphore;
	SemaphoreHandle_t IMSIBinarySemaphore;
	SemaphoreHandle_t NETBinarySemaphore;
	QueueHandle_t CSQRepQueue;
	QueueHandle_t IMEIRepQueue;
	QueueHandle_t IMSIRepQueue;
	QueueHandle_t GprsRepQueue;
	
}Gprs;

extern int deviceConnect(void);
extern int deviceUpdateCSQ(void);
extern int deviceUpdateIMEI(void);
extern int deviceUpdateIMSI(void);
extern void deviceConfig(const char *pimei,const char *pimsi,const char * pcsq);