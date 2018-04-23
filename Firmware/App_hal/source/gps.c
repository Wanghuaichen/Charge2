#include "gps.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "usart.h"
#include "string.h"
#include "message.h"
#include "timers.h"

extern xTimerHandle GPSTimerHandler;


QueueHandle_t GPSRecMsgQueue;   //接收信息队列句柄

//QueueHandle_t GpsGNRMCMsgQueue;   //接收信息队列句柄
//QueueHandle_t GpsGNVTGMsgQueue;   //接收信息队列句柄
//QueueHandle_t GpsGNGGAMsgQueue;   //接收信息队列句柄
//QueueHandle_t GpsGNGSAMsgQueue;   //接收信息队列句柄
//QueueHandle_t GpsGPGSVMsgQueue;   //接收信息队列句柄
//QueueHandle_t GpsGLGSVMsgQueue;   //接收信息队列句柄
//QueueHandle_t GpsGNGLLMsgQueue;   //接收信息队列句柄

void GPSConfig(void)
{
	GPSRecMsgQueue = xQueueCreate(1,1024);
//	GpsGNRMCMsgQueue = xQueueCreate(1,120);    
//    GpsGNVTGMsgQueue = xQueueCreate(1,120);    
//    GpsGNGGAMsgQueue = xQueueCreate(1,120);     
//    GpsGNGSAMsgQueue = xQueueCreate(1,120);   
//    GpsGPGSVMsgQueue = xQueueCreate(1,120);     
//    GpsGLGSVMsgQueue = xQueueCreate(1,120);     
//    GpsGNGLLMsgQueue = xQueueCreate(1,120);    
	Usart2Init();
	xTimerStart(GPSTimerHandler,portMAX_DELAY);	
}
int GPSReceiveFromISR(char *msg)
{
	  u8 Res;
	  BaseType_t err;
	  BaseType_t xHighPriorityTaskWoken;
		if(GPSRecMsgQueue!=NULL)
		{
			err = xQueueOverwriteFromISR(GPSRecMsgQueue,msg,&xHighPriorityTaskWoken);
			if(err!=pdTRUE)
			{
				printf("usart idel add queue failed\r\n");
			}
			portYIELD_FROM_ISR(xHighPriorityTaskWoken);
		}
}

void GPSReceiveTask(void *pArg)   //命令解析任务
{
	GPSConfig();
	//char* buf = pvPortMalloc(500);
	while(1)
	{ 
//		if(GPSRecMsgQueue != NULL)
//		xQueueReceive(GPSRecMsgQueue,buf,portMAX_DELAY);
//		if(NULL != strstr(buf,"GNRMC"))
//		{
//		   if(GpsGNRMCMsgQueue != NULL)
//		   xQueueOverwrite(GpsGNRMCMsgQueue,buf);
//		   printf("GPS buf 1 %s\r\n",buf);
//		}
//		if(NULL != strstr(buf,"GNVTG"))
//		{
//		   if(GpsGNVTGMsgQueue != NULL)
//		   xQueueOverwrite(GpsGNVTGMsgQueue,buf);
//		   printf("GPS buf 2 %s\r\n",buf);
//		}
//		if(NULL != strstr(buf,"GNGGA"))
//		{
//	       if(GpsGNGGAMsgQueue != NULL)
//		   xQueueOverwrite(GpsGNGGAMsgQueue,buf);
//		   printf("GPS buf 3 %s\r\n",buf);
//		}
//		if(NULL != strstr(buf,"GNGSA"))
//		{
//		   if(GpsGNGSAMsgQueue != NULL)
//		   xQueueOverwrite(GpsGNGSAMsgQueue,buf);
//		   printf("GPS buf 4 %s\r\n",buf);
//		}
//		if(NULL != strstr(buf,"GPGSV"))
//		{
//		   if(GpsGPGSVMsgQueue != NULL)
//		   xQueueOverwrite(GpsGPGSVMsgQueue,buf);
//		   printf("GPS buf 5 %s\r\n",buf);
//		}
//		if(NULL != strstr(buf,"GLGSV"))
//		{
//		   if(GpsGLGSVMsgQueue != NULL)
//		   xQueueOverwrite(GpsGLGSVMsgQueue,buf);
//		   printf("GPS buf 6 %s\r\n",buf);
//		}
//		if(NULL != strstr(buf,"GNGLL"))
//		{
//		   if(GpsGNGLLMsgQueue != NULL)
//		   xQueueOverwrite(GpsGNGLLMsgQueue,buf);
//		   printf("GPS buf 7 %s\r\n",buf);
//		}
		vTaskDelay(1000);
	}
	//vPortFree(buf);
}

void GpsUpload(void)
{
	printf("GPS Task\r\n");
	char* msg = pvPortMalloc(1024);
	
//	char* g1 = pvPortMalloc(100);
//	char* g2 = pvPortMalloc(100);
//	char* g3 = pvPortMalloc(100);
//	char* g4 = pvPortMalloc(100);
//	char* g5 = pvPortMalloc(100);
//	char* g6 = pvPortMalloc(100);
//	char* g7 = pvPortMalloc(100);
	memset(msg,0,1024);
//	memset(g1,0,100);
//	memset(g2,0,100);
//	memset(g3,0,100);
//	memset(g4,0,100);
//	memset(g5,0,100);
//	memset(g6,0,100);
//	memset(g7,0,100);
	
	xQueueReceive(GPSRecMsgQueue,msg,0);
 
    printf("Mesg %s\r\n",msg);
//	xQueueReceive(GpsGNRMCMsgQueue,g1,0);
//	xQueueReceive(GpsGNVTGMsgQueue,g2,0);
//	xQueueReceive(GpsGNGGAMsgQueue,g3,0);
//	xQueueReceive(GpsGNGSAMsgQueue,g4,0);
//	xQueueReceive(GpsGPGSVMsgQueue,g5,0);
//	xQueueReceive(GpsGLGSVMsgQueue,g6,0);
//	xQueueReceive(GpsGNGLLMsgQueue,g7,0);
	
//	strcpy(msg,g1);
//	strcat(msg,g2);
//	strcat(msg,g3);
//	strcat(msg,g4);
//	strcat(msg,g5);
//	strcat(msg,g6);
//	strcat(msg,g7);
	
	MessageSend(msg,1);
	
	vPortFree(msg);
//	vPortFree(g1);
//	vPortFree(g2);
//	vPortFree(g3);
//	vPortFree(g4);
//	vPortFree(g5);
//	vPortFree(g6);
//	vPortFree(g7);
}