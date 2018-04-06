#include "register.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "gprs.h"
#include "cjson.h"
extern Gprs G510;
SemaphoreHandle_t RigisterBinarySemaphore;

int DeviceRegister(void)
{
	G510.UpdateCSQ();
  G510.UpdateIMEI();
  G510.UpdateIMSI();
	
	RigisterBinarySemaphore = xSemaphoreCreateBinary();
	cJSON *res = cJSON_CreateObject();

	cJSON_AddStringToObject(res, "type", "register");
	cJSON_AddStringToObject(res, "imsi", (const char *)G510.imsi);
	cJSON_AddStringToObject(res, "imei",  (const char *)G510.imei);
	cJSON_AddNumberToObject(res, "csq", G510.csq);
	cJSON_AddNumberToObject(res, "hardv", 10);
	cJSON_AddStringToObject(res, "softv", "0102");
	cJSON_AddNumberToObject(res, "port", 11);
	
  char * str;
	str = cJSON_Print(res);
  BaseType_t err;
  for(int i=0;i<3;i++)
  {
		MessageSend(str,1);
		err = xSemaphoreTake(RigisterBinarySemaphore,10000);
		printf("register %d times\r\n",i);
	  if(err == pdTRUE)
		{
	    cJSON_Delete(res); 
			vSemaphoreDelete(RigisterBinarySemaphore);
      #ifdef DEBUG
      printf("register ok\r\n");
			#endif
			return 1;
		}
  }
	cJSON_Delete(res); 	
	printf("register failed\r\n");
	printf("System Rebooting\r\n");
	NVIC_SystemReset();
	while(1);
	return -1;
}