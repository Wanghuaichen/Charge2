#include "sys.h"
#include "gprs.h"
#include "message.h"
#include "string.h"
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
	
	.cmdNum = 12,
	.rep[0] = "OK",
	.rep[1] = "OK",
	.rep[2] = "OK",
	.rep[3] = "OK",
	.rep[4] = "+CPIN: READY",
	.rep[5] = "+CGREG: 0,1",
	.rep[6] = "OK",
	.rep[7] = "OK",
	.rep[8] = ".",
	
	.rep[9] = "+CLOUDAUTH: OK",
	.rep[10] = "CLOUDCONN",
	.rep[11] = "OK",
	.rep[12] = "CSQ",
	
	.cmd[0] = "AT\r\n",
	.cmd[1] = "ATi8\r\n",
	.cmd[2] = "ATE0\r\n\0",
	.cmd[3] = "AT+CMEE=2\r\n",
	.cmd[4] = "AT+CPIN?\r\n",
	.cmd[5] = "AT+CGREG?\r\n",
	.cmd[6] = "AT+COPS?\r\n",
	.cmd[7] = "AT+MIPCALL=1,\"CMNET\"\r\n",
	.cmd[8] = "AT+MIPCALL?\r\n",
	
	.cmd[10] = "AT+CLOUDCONN=70,0,3\r\n",
	
	.cmd[12] = "AT+CSQ?\r\n",
	.IPFlashAddr = 0,
	.OtaIPFlashAddr =0,
	.Connect = deviceConnect,
	.Config = deviceConfig,
};
void deviceConfig(const char *pproductKey,const char *pdeviceName,const char * pdeviceScreat)
{
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
	G510.GprsCSQBinarySemaphore = xSemaphoreCreateBinary();
	G510.GprsNetCheckBinarySemaphore = xSemaphoreCreateBinary();
	G510.GprsRepQueue = xQueueCreate(1,30);
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
			    err = xSemaphoreTake(G510.GprsConnectBinarySemaphore,10000);
				{
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
static int pdeviceGetCSQ(void)
{
		u8 count = 0;
		BaseType_t err;
		if(G510.GprsRepQueue!=NULL)
	  {
			err = xQueueOverwrite(G510.GprsRepQueue,G510.rep[12]);
			if(err!=pdTRUE)
			{
				printf("Rep Send failed\r\n");
				return -1;
			}
  	}
	  MessageSend(G510.cmd[12],0);
		while(1)
		{
			count++;
			if(G510.GprsCSQBinarySemaphore!=NULL)
			{
			    err = xSemaphoreTake(G510.GprsCSQBinarySemaphore,10000);
				{
					printf("%d\r\n",count);
					if(err == pdTRUE)
					{
							return 1;
					}
					else
					{
						MessageSend(G510.cmd[12],0);
						if(G510.GprsRepQueue!=NULL)
						{
							err = xQueueOverwrite(G510.GprsRepQueue,G510.rep[12]);
							if(err!=pdTRUE)
							{
								printf("Rep Sen failed\r\n");
								return -1;
							}
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
int deviceConnect(void)
{

	for(int i=0;i<5;i++)
	{
		if(pdeviceConnect()>0)
		{
      printf("Connecet to cloud successful\r\n");
			return 1;
		}
    printf("Connect to cloud failed %d.....\r\n",i+1);
	}
	printf("Device Rebooting\r\n");
	vTaskSuspendAll();
	while(1);
}
int deviceGetCSQ(void)
{

	for(int i=0;i<5;i++)
	{
		if(pdeviceConnect()>0)
		{
      printf("Connecet to cloud successful\r\n");
			return 1;
		}
    printf("Connect to cloud failed %d.....\r\n",i+1);
	}
	printf("Device Rebooting\r\n");
	vTaskSuspendAll();
	while(1);
}