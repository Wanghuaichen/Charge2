#include "sys.h"
#include "gprs.h"
#include "message.h"
#include "string.h"
#include "timers.h"
#include "queue.h"
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
	.rep[12] = "+CSQ: ",
	.rep[13] = "+CIMI: ",       /*Get imsi*/
	.rep[14] = "+CGSN: \"",     /*Get imei*/
	
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
	.cmd[13] = "AT+CIMI\r\n",    /*Get imsi*/
	.cmd[14] = "AT+CGSN?\r\n",   /*Get imei*/

	.IPFlashAddr = 0,
	.OtaIPFlashAddr =0,
	.Connect = deviceConnect,
	.UpdateCSQ = deviceUpdateCSQ,
	.UpdateIMEI = deviceUpdateIMEI,
	.UpdateIMSI = deviceUpdateIMSI,
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
	G510.GprsCSQBinarySemaphore = xSemaphoreCreateBinary();
	G510.IMEIBinarySemaphore = xSemaphoreCreateBinary();
	G510.IMSIBinarySemaphore = xSemaphoreCreateBinary();
//	G510.GprsNetCheckBinarySemaphore = xSemaphoreCreateBinary();
	G510.GprsRepQueue = xQueueCreate(1,30);
	G510.CSQRepQueue = xQueueCreate(1,120);
	G510.IMEIRepQueue = xQueueCreate(1,120);
	G510.IMSIRepQueue = xQueueCreate(1,120);
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

static int pdeviceUpdateCSQ(void)
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
			    err = xSemaphoreTake(G510.GprsCSQBinarySemaphore,15000);
					printf("%d\r\n",count);
					if(err == pdTRUE)
					{
							char * csqbuf = pvPortMalloc(120);
							xQueueReceive(G510.CSQRepQueue,csqbuf,10);
							if(NULL != strstr(csqbuf,"CSQ"))   /*Receive csq*/
							{
								 char * result;
								 result = strstr(csqbuf,G510.rep[12]);
								 result+=strlen(G510.rep[12]);
								 if((*(result+1))!=',')
								 {
									 G510.csq = ((*result)-'0')*10+(*(result+1))-'0';
								 }
								 else 
								 {
									 G510.csq = (*result)-'0';
								 }
								 vPortFree(csqbuf);
								 printf("CSQ Value %d\r\n",G510.csq);
								 return 1;
							 }
							vPortFree(csqbuf);
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
static int pdeviceUpdateIMEI(void)
{
		u8 count = 0;
		BaseType_t err;
		if(G510.GprsRepQueue!=NULL)
	  {
			err = xQueueOverwrite(G510.GprsRepQueue,G510.rep[14]);
			if(err!=pdTRUE)
			{
				printf("Rep Send failed\r\n");
				return -1;
			}
  	}
	  MessageSend(G510.cmd[14],0);
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
							if(NULL != strstr(imeibuf,G510.rep[14]))   /*Receive imei*/
							{
								 char * result;
								 result = strstr(imeibuf,G510.rep[14]);
								 result+=strlen(G510.rep[14]);
								 memcpy(G510.imei,result,15);
								 printf("IMEI Value %s\r\n",G510.imei);
								 vPortFree(imeibuf);
								 break;
							 }
							vPortFree(imeibuf);
					}
					else
					{
						MessageSend(G510.cmd[14],0);
						if(G510.GprsRepQueue!=NULL)
						{
							err = xQueueOverwrite(G510.GprsRepQueue,G510.rep[14]);
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
static int pdeviceUpdateIMSI(void)
{
		u8 count = 0;
		BaseType_t err;
		if(G510.GprsRepQueue!=NULL)
	  {
			err = xQueueOverwrite(G510.GprsRepQueue,G510.rep[13]);
			if(err!=pdTRUE)
			{
				printf("Rep Send failed\r\n");
				return -1;
			}
  	}
	  MessageSend(G510.cmd[13],0);
		while(1)
		{
			count++;
			if(G510.IMSIBinarySemaphore!=NULL)
			{
			    err = xSemaphoreTake(G510.IMSIBinarySemaphore,15000);
					printf("%d\r\n",count);
					if(err == pdTRUE)
					{
							char * imsibuf = pvPortMalloc(120);
							xQueueReceive(G510.IMSIRepQueue,imsibuf,10);
							if(NULL != strstr(imsibuf,G510.rep[13]))   /*Receive imei*/
							{
								 char * result;
								 result = strstr(imsibuf,G510.rep[13]);
								 result+=strlen(G510.rep[13]);
								 memcpy(G510.imsi,result,15);
								 printf("IMSI Value %s\r\n",G510.imsi);
								 vPortFree(imsibuf);
								 break;
							 }
							vPortFree(imsibuf);
					}
					else
					{
						MessageSend(G510.cmd[14],0);
						if(G510.GprsRepQueue!=NULL)
						{
							err = xQueueOverwrite(G510.GprsRepQueue,G510.rep[14]);
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
extern xTimerHandle testTimerHandler;

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
		  xTimerStart(testTimerHandler,portMAX_DELAY);
			return 1;
		}
    printf("Connect to cloud failed %d.....\r\n",i+1);
	}
	printf("Device Rebooting\r\n");
	vTaskSuspendAll();
	while(1);
}

int deviceUpdateCSQ(void)
{
  G510.csqFlag = 1;
	for(int i=0;i<5;i++)
	{
		if(pdeviceUpdateCSQ()>0)
		{
			G510.csqFlag = 0;
			return 1;
		}
    printf("Get CSQ failed %d.....\r\n",i+1);
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
int deviceUpdateIMSI(void)
{
  G510.imsiFlag = 1;
	for(int i=0;i<5;i++)
	{
		if(pdeviceUpdateIMSI()>0)
		{
			G510.imsiFlag = 0;
			return 1;
		}
    printf("Get IMSI failed %d.....\r\n",i+1);
	}
	printf("Device Rebooting\r\n");
	vTaskSuspendAll();
	while(1);
}
