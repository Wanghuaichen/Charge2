#include "sys.h"
#include "gprs.h"
#include "message.h"
#include "string.h"
#include "timers.h"
#include "queue.h"
#include "register.h"
#include "gps.h"
//const char* AT =			      	  "AT\r\n";
//const char* ATi8 =		          "ATi8\r\n";
//const char* ATE0 =			        "ATE0\r\n\0";
////const char* ATE0 =			      "ATE1\r\n\0";
//const char* AT_CMEE =           "AT+CMEE=2\r\n";
//const char* AT_CPIN =           "AT+CPIN?\r\n";
//const char* AT_CGREG =          "AT+CGREG?\r\n";
//const char* AT_COPS =           "AT+COPS?\r\n";
//const char* AT_MIPCALL =	      "AT+MIPCALL=1,\"CMNET\"\r\n";
//const char* AT_MIPCALL_Q =      "AT+MIPCALL?\r\n";
/*鉴权信息*/
//const char* AT_CLOUDAUTH =      "AT+CLOUDAUTH=\"LNAHrbyMmy8\",\"ForG510\",\"1YDVQlNsFz0kBr2xeGRJ3avV8iP1Kq4f\",\"iot-auth.aliyun.com\"\r\n";  

//const char* AT_CLOUDAUTH =      "AT+CLOUDAUTH=\"vQOi1lw2z9r\",\"device0107\",\"crDaCt5G1fcUqAZeogHc7vERc8PUs0cT\",\"iot-auth.aliyun.com\"\r\n";  
//+ CLOUDAUTH[= <pkey>, <dname>, <dsrt>, <host>]
//const char* AT_CLOUDCONN =	    "AT+CLOUDCONN=70,0,4\r\n";

/*MQTT 参数*/
//const char* AT_CLOUDCONN =	    "AT+CLOUDCONN=70,0,3\r\n";
//保活时间
//clean session信息   0：不清除
//MQTT版本号

/*订阅信息*/
//const char * AT_CLOUDSUB =     "AT+CLOUDSUB=\"/LNAHrbyMmy8/ForG510/get\",0\r\n";

/*RRPC订阅信息*/
//const char * AT_RRPC_SUB =     "AT+CLOUDSUB=\"/sys/LNAHrbyMmy8/ForG510/rrpc/request/+\",0\r\n";


extern TaskHandle_t MsgRecTaskHanhler;        /*消息解析任务*/
extern QueueHandle_t UsartRecMsgQueue;        /*消息接收队列*/
extern TaskHandle_t MsgSendTaskHanhler;       /*消息封装发送任务*/
static char authenticationCode[160] = {0};
static char subscriptionCode[160] = {0};
Gprs G510={
	
	.cmdNum = 8,
	.rep[0] = "OK",
	.rep[1] = "OK",
	.rep[2] = "READY",
	.rep[3] = "CGATT: 1",
	.rep[4] = "OK",
	.rep[5] = "OK",
	.rep[6] = ".",
	.rep[7] = "MIPOPEN: 1,1",
	.rep[8] = "+CGSN: \"",     /*Get imei*/
	
	.cmd[0] = "AT\r\n",
	.cmd[1] = "ATE0\r\n\0",
	.cmd[2] = "AT+CPIN?\r\n\0",
	.cmd[3] = "AT+CGATT?\r\n\0",
	.cmd[4] = "AT+GTSET=\"IPRFMT\",2\r\n\0",
	.cmd[5] = "AT+MIPCALL=1,\"CMNET\"\r\n\0",
	.cmd[6] = "AT+MIPCALL?\r\n\0",
	.cmd[7] = "AT+MIPOPEN=1,,\"139.224.129.147\",5000,0\r\n",
	.cmd[8] = "AT+CGSN?\r\n",   /*Get imei*/

	.IPFlashAddr = 0,
	.OtaIPFlashAddr =0,
	.Connect = deviceConnect,
	.UpdateIMEI = deviceUpdateIMEI,
	.Config = deviceConfig,
};
void deviceConfig(const char *pproductKey,const char *pdeviceName,const char * pdeviceScreat)
{
	G510.connectFlag = 0;
	G510.csqFlag = 0;
	G510.netFlag = 0;
	G510.imeiFlag = 0;
	G510.imsiFlag = 0;
	char * pcmd = pvPortMalloc(160);
	memset(pcmd,0,160);

	printf("G510 Config\r\n");
	
	memset(G510.productKey,0,16);
	memset(G510.deviceName,0,20);
	memset(G510.deviceScreat,0,36);
	strcpy(G510.productKey,pproductKey);
	strcpy(G510.deviceName,pdeviceName);
	strcpy(G510.deviceScreat,pdeviceScreat);
	
	/*生成鉴权信息*/
	strcpy(pcmd,"AT+CLOUDAUTH=\"");
	strcat(pcmd,G510.productKey);
	strcat(pcmd,"\",\"");
	strcat(pcmd,G510.deviceName);
	strcat(pcmd,"\",\"");
	strcat(pcmd,G510.deviceScreat);
	strcat(pcmd,"\",\"");
	strcat(pcmd,"iot-auth.aliyun.com\"\r\n");
	strcpy(authenticationCode,pcmd);
	G510.cmd[9] = authenticationCode;
	
	memset(pcmd,0,160);
	/*生成订阅信息*/
	strcpy(pcmd,"AT+CLOUDSUB=\"/");
	strcat(pcmd,G510.productKey);
	strcat(pcmd,"/");
	strcat(pcmd,G510.deviceName);
	strcat(pcmd,"/get\",0\r\n");
	strcpy(subscriptionCode,pcmd);
	G510.cmd[11] = subscriptionCode;
	
	G510.GprsConnectBinarySemaphore = xSemaphoreCreateBinary();
	G510.IMEIBinarySemaphore = xSemaphoreCreateBinary();
	G510.GprsRepQueue = xQueueCreate(1,30);
	G510.IMEIRepQueue = xQueueCreate(1,120);

}
static int pdeviceConnect(void)
{
	printf("G510 Connecting\r\n");
	printf("Please add the moduel reset command\r\n");
	for(int i=0;i<G510.cmdNum;i++)
	{
		u8 count = 0;
		BaseType_t err;
		if(G510.GprsRepQueue!=NULL)
	  {
			err = xQueueOverwrite(G510.GprsRepQueue,G510.rep[i]);
			if(err!=pdTRUE)
			{
				printf("Rep Send failed\r\n");
				return -1;
			}
  	}
	  MessageSend(G510.cmd[i],0);
		while(1)
		{
			count++;
			if(G510.GprsConnectBinarySemaphore!=NULL)
			{
			    err = xSemaphoreTake(G510.GprsConnectBinarySemaphore,15000);
					printf("%d\r\n",count);
					if(err == pdTRUE)
					{
						break;
					}
					else
					{
						MessageSend(G510.cmd[i],0);
						if(G510.GprsRepQueue!=NULL)
						{
							err = xQueueOverwrite(G510.GprsRepQueue,G510.rep[i]);
							if(err!=pdTRUE)
							{
								printf("Rep Sen failed\r\n");
								return -1;
							}
						}
					}
			}
			else
			{
				printf("No G510 semaphore \r\n");
			}
			if(count>=5)
			{
				return -1; 
			}
		}

	}
	return 1;
}

static int pdeviceUpdateIMEI(void)
{
		u8 count = 0;
		BaseType_t err;
		if(G510.GprsRepQueue!=NULL)
	  {
			err = xQueueOverwrite(G510.GprsRepQueue,G510.rep[8]);
			if(err!=pdTRUE)
			{
				printf("Rep Send failed\r\n");
				return -1;
			}
  	}
	  MessageSend(G510.cmd[8],0);
		while(1)
		{
			count++;
			if(G510.IMEIBinarySemaphore!=NULL)
			{
			    err = xSemaphoreTake(G510.IMEIBinarySemaphore,15000);
					printf("%d\r\n",count);
					if(err == pdTRUE)
					{
							char * imeibuf = pvPortMalloc(120);
							xQueueReceive(G510.IMEIRepQueue,imeibuf,10);
							if(NULL != strstr(imeibuf,G510.rep[8]))   /*Receive imei*/
							{
								 char * result;
								 result = strstr(imeibuf,G510.rep[8]);
								 result+=strlen(G510.rep[8]);
								 memcpy(G510.imei,result,15);
								 printf("IMEI Value %s\r\n",G510.imei);
								 vPortFree(imeibuf);
								 break;
							 }
							vPortFree(imeibuf);
					}
					else
					{
						MessageSend(G510.cmd[8],0);
						if(G510.GprsRepQueue!=NULL)
						{
							err = xQueueOverwrite(G510.GprsRepQueue,G510.rep[8]);
							if(err!=pdTRUE)
							{
								printf("Rep Sen failed\r\n");
								return -1;
							}
						}
					}
			}
			else
			{
				printf("No G510 semaphore \r\n");
			}
			if(count>=5)
			{
				return -1; 
			}
		}
	return 1;
}

extern xTimerHandle GPSTimerHandler;
int deviceConnect(void)
{
	printf("connecty\r\n");
  G510.connectFlag = 1;
	for(int i=0;i<5;i++)
	{
		if(pdeviceConnect()>0)
		{
      printf("Connecet to cloud successful\r\n");
			G510.connectFlag = 0;
			xTimerStart(GPSTimerHandler,portMAX_DELAY);
			return 1;
		}
    printf("Connect to cloud failed %d.....\r\n",i+1);
	}
	printf("Device Rebooting\r\n");
	vTaskSuspendAll();
	while(1);
}
int deviceUpdateIMEI(void)
{
  G510.imeiFlag = 1;
	for(int i=0;i<5;i++)
	{
		if(pdeviceUpdateIMEI()>0)
		{
			G510.imeiFlag = 0;
			return 1;
		}
    printf("Get IMEI failed %d.....\r\n",i+1);
	}
	printf("Device Rebooting\r\n");
	vTaskSuspendAll();
	while(1);
}