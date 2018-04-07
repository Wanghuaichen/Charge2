#include "register.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "gprs.h"
#include "cjson.h"
extern Gprs G510;
SemaphoreHandle_t CSQBinarySemaphore;

int DeviceUploadCSQ(void)
{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
	G510.UpdateCSQ();
	
	CSQBinarySemaphore = xSemaphoreCreateBinary();
	cJSON *res = cJSON_CreateObject();

	cJSON_AddStringToObject(res, "type", "sigval");
	cJSON_AddNumberToObject(res, "csq", G510.csq);
	
  char * str;
	str = cJSON_Print(res);
  BaseType_t err;
  for(int i=0;i<3;i++)
  {
		MessageSend(str,1);
		err = xSemaphoreTake(CSQBinarySemaphore,10000);
		printf("CSQ %d times\r\n",i);
	  if(err == pdTRUE)
		{
	    cJSON_Delete(res); 
			vSemaphoreDelete(CSQBinarySemaphore);
      #ifdef DEBUG
      printf("CSQ ok\r\n");
			#endif
			return 1;
		}
  }
	cJSON_Delete(res); 	
	printf("CSQ failed\r\n");
	printf("System Rebooting\r\n");
	NVIC_SystemReset();
	while(1);
	return -1;
}