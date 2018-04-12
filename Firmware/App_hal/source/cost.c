#include "cost.h"
#include <string.h>
#include "port.h"
#include "para.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "message.h"
#include "chargerep.h"

static int stopCheck[20] = { 0 };
QueueHandle_t PostFinshQueue;   
QueueHandle_t PostFinshRepQueue;
void PostFinishQueueConfig(void)
{
    PostFinshRepQueue = xQueueCreate(3,120);
		PostFinshQueue = xQueueCreate(10,100);
}

void PortSubUseTime(u8 portnumber)
{
	if (PortGetUseTime(portnumber) < 1)              //费用不足  充电停止
		{
			PortChargeFinish(portnumber, MONEY_OVER);
			return ;
		}
	else if (PortGetAdcValue(portnumber) >= CURRENT_MIN + 10)  //充电正常，继续扣费
		{
			static u8 costCount = 0;
			costCount++;
			if (costCount >= 60)  //3min扣一次费用
			{
				costCount = 0;
				float p = GetPrice();
				printf("price is %d\r\n", (int)p);
				float cur =  GetCurrentValue(PortGetAdcValue(portnumber));
				printf("currten is %f\r\n", cur);
	      float cost = (p)*(cur *0.22)*(0.05);
				printf("cost %f\r\n", cost);
				SubUseTime(portnumber,cost);
				char m[20] = { 0};
				printf("money %f\r\n", PortGetUseTime(portnumber));

			}
		}
	else if (PortGetAdcValue(portnumber) <= CURRENT_MIN)    //充电电流减小
		{
			stopCheck[portnumber]++;
			if (stopCheck[portnumber] >= 3)
			{
				stopCheck[portnumber] = 0;
				PortChargeFinish(portnumber, CHARGE_OVER);      //连续3次减小，则充电完成
			}
		}
}

#define door 1084

float GetCurrentValue(u16 adcValue)             //返回对应的电流值，单位mA
{
	//将adc的采样值转化为对应的电流值
	float dcValue = (adcValue / 4096.0) * 5;
	//printf("adcValue :%d\r\n", adcValue);

	printf("dcValue :%f\r\n",dcValue);
	if ((adcValue >= 0)&&(adcValue <= door))
	{
		float cValue = dcValue * 1.106 + 0.0108;
		return cValue;
	}
	else if ((adcValue > door)&&(adcValue <= 4096))
	{
		float cValue = (dcValue*dcValue * 2.0127) - 3.6036*dcValue + 2.7791;
		return cValue;
	}
	else return -1;
}

void PostFinishTask(void * pAgr)
{
	char* buf = pvPortMalloc(100);
	char* repRec = pvPortMalloc(120);
	PostFinishQueueConfig();
	int port =0;
	BaseType_t err;
	while(1)
	{
		memset(buf,0,100);
		xQueuePeek(PostFinshQueue,buf,portMAX_DELAY);
		if(GetPortNumberFromJson(buf)>0)
		{
			port = GetPortNumberFromJson(buf);
			printf("finish port %d\r\n",port);
		}
		for(int i=0;i<3;i++)
		{
			MessageSend(buf,1);
			memset(repRec,0,120);
			err = xQueueReceive(PostFinshRepQueue,repRec,10000);
			printf("Check %d times\r\n",i);
			if(err == pdTRUE)
			{
				 int repPort = GetRepPort(buf);
				 if(repPort == port)
				 {
            xQueueReceive(PostFinshQueue,buf,0);
					 	PortClearUseStatus(repPort);
          	PortClearFinishStatus(repPort);
	          PortClearUseTime(repPort);
					 	break;
				 }
			}
		}
		vTaskDelay(100);
	}
	vPortFree(buf);
	vPortFree(repRec);
	
}

#define PORT_KEY  "\"port\":"
int GetPortNumberFromJson(u8 *buf)
{
	if(NULL!=strstr(buf,(const char*)PORT_KEY))   
		{
			int port = 0;
				char * result = strstr(buf,(const char*)PORT_KEY);
			result += strlen(PORT_KEY);
			if((*(result+2))==',')
			{
				port = (*(result)-'0')*10 + (*(result+1)-'0');
			}
			else if((*(result+1))==',')
			{
				port = (*(result)-'0');
			}
			return port;
		}
	 return 0;
}