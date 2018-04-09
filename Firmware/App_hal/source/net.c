#include "net.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "gprs.h"
#include "cjson.h"
#include "message.h"
extern Gprs G510;
SemaphoreHandle_t NetBinarySemaphore;

int NetCheck(void)
{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
	NetBinarySemaphore = xSemaphoreCreateBinary();
  G510.netFlag = 1;
  BaseType_t err;
  for(int i=0;i<3;i++)
  {
		MessageSend(G510.cmd[8],0);
		err = xSemaphoreTake(NetBinarySemaphore,10000);
		printf("NetCheck %d times\r\n",i);
	  if(err == pdTRUE)
		{
			G510.netFlag = 0;
			vSemaphoreDelete(NetBinarySemaphore);
      #ifdef DEBUG
      printf("Net Check ok\r\n");
			#endif
			return 1;
		}
  }
	printf("Net Check failed\r\n");
	printf("System Rebooting\r\n");
	NVIC_SystemReset();
	while(1);
	return -1;
}