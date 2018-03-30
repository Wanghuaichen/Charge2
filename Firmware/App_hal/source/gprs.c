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
extern TaskHandle_t NetMsgSendTaskHanhler;    /*联网数据包发送任务*/
extern TaskHandle_t NetMsgRecTaskHanhler; /*网络数据解析任务*/
static char authenticationCode[160] = {0};
static char subscriptionCode[160] = {0};
Gprs G510={
	.status = 0,
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
	.cmdNum = 12,
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
	G510.status = 0;
	G510.repNum = 0;
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
	
	G510.GprsBinarySemaphore = xSemaphoreCreateBinary();
}
static int pdeviceConnect(void)
{
	printf("G510 Connecting\r\n");
	//to do
	//reset the gprs moduel
	printf("Please add the moduel reset command\r\n");
	for(int i=0;i<G510.cmdNum;i++)
	{
		
		
		u8 count = 0;
		BaseType_t err;
	    G510.repNum = i;
	    MessageSend(G510.cmd[i]);
		while(1)
		{
			count++;
			if(G510.GprsBinarySemaphore!=NULL)
			{
			    err = xSemaphoreTake(G510.GprsBinarySemaphore,10000);
				{
					printf("%d\r\n",count);
					if(err == pdTRUE)
					{
						break;
					}
					else
					{
						MessageSend(G510.cmd[i]);
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
int deviceConnect(void)
{
	/*挂起解析任务*/
    if(MsgRecTaskHanhler!=NULL)
	{
		vTaskSuspend(MsgRecTaskHanhler);
	}
	/*挂起封装发送任务*/
	if(MsgSendTaskHanhler!=NULL)
	{
		vTaskSuspend(MsgSendTaskHanhler);
	}
	/*恢复网络发送任务*/
	if(NetMsgSendTaskHanhler!=NULL)
	{
		vTaskResume(NetMsgSendTaskHanhler);
	}
    /*恢复网络解析任务*/
	if(NetMsgRecTaskHanhler!=NULL)
	{
		vTaskResume(NetMsgRecTaskHanhler);
	}
	for(int i=0;i<5;i++)
	{
		if(pdeviceConnect()>0)
		{
			/*回复解析任务*/
	       if(MsgRecTaskHanhler!=NULL)
			{
				vTaskResume(MsgRecTaskHanhler);
			}
			/*回复封装发送任务*/
	       if(MsgSendTaskHanhler!=NULL)
			{
				vTaskResume(MsgRecTaskHanhler);
			}
			/*删除网络发送任务*/
		   if(NetMsgSendTaskHanhler!=NULL)
			{
				vTaskDelete(NetMsgSendTaskHanhler);
			}
			/*删除网络解析任务*/
		   if(NetMsgRecTaskHanhler!=NULL)
			{
				vTaskDelete(NetMsgRecTaskHanhler);
			}
            printf("Connecet to cloud successful\r\n");
			return 1;
		}
        printf("Connect to cloud failed %d.....\r\n",i+1);
	}
	printf("Device Rebooting\r\n");
	vTaskSuspendAll();
	while(1);
}