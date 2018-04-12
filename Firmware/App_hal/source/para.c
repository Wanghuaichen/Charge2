#include "para.h"
#include "FreeRTOS.h"
#include "board.h"
#include "portaddr.h"
#include "cjson.h"
#include "string.h"
#include "message.h"

static int devicePrice          = 100;  //100 分 ，1元钱每kwh
static int currentMax           = 3000;

static char updateSocket[50] = { 0 };
static int hardWareVision       = 0;
static int softWareVision       = 0;

static int heartBite            = 0;


int CheckParaCmd(u8 *buf)
{
	if (NULL != strstr((const char *)buf, (const char *)"chfres"))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

#define PARA_STAT  "chfres"
#define PARA_END   "chfred"

void SetPareStatus(u8 *buf)
{
	char * st = NULL;
	char * ed = NULL;
	if (NULL != strstr((const char *)buf, (const char *)PARA_END))
	{
		printf("\r\n%s\r\n", buf);
		char * s = NULL;
		s = strstr((const char *)buf, (const char *)PARA_STAT);
		s = s + strlen(PARA_STAT);
		char * cur = pvPortMalloc(20*sizeof(char));
		char * pri = pvPortMalloc(20*sizeof(char));
		memset(cur, 0, 20);
		memset(pri, 0, 20);
		for (int i = 0; i < 4; i++)
		{
			cur[i] = *s;
			s++;
		}
		for (int i = 0; i < 3; i++)
		{
			pri[i] = *s;
			s++;
		}
		int current = (cur[0] - '0') * 1000 + (cur[1] - '0') * 100 + (cur[2] - '0') * 10 + (cur[3] - '0');
		int price = (pri[0] - '0') * 100 + (pri[1] - '0') * 10 + (pri[2] - '0');
		
		SetPrice(price);
		SetCurrentMax(current);

		printf("price %d\r\n", price);
		printf("current %d\r\n", current);
		RepPara();
		vPortFree(cur);
		vPortFree(pri);   
	}
}


int GetPrice(void)
{
	return devicePrice;
}

void SetPrice(int p)            
{
	if ((p < 0)&&(p > 999)) 
	{
		return ;
	}
	devicePrice = p;
	char * Msg = pvPortMalloc(64);
	memset(Msg, 0, 64);
	char price[4] = {0};
	price[0] = (p/100)+'0';
	price[1] = (p/10)%10+'0';
	price[2] = p%10+'0';
	const char * head = "price";
	const char * end =  "status";
	strcpy(Msg, head);
	strcat(Msg, price);
	strcat(Msg, end);
	STMFLASH_Write(PRICE_ADDR, (u32*)Msg, 10);
	vPortFree(Msg);
}

void  UpdatePrice(void)
{
	char * Msg = pvPortMalloc(64);
	memset(Msg, 0, 64);
	
	const char * head = "price";
	
	const char * end = "status";
	
	STMFLASH_Read(PRICE_ADDR, (u32*)Msg, 10);
	//	printf("Msg %s\n", Msg);
	
	if(NULL != strstr((const char *)Msg, (const char *)end))
	{
		if (NULL != strstr((const char *)Msg, (const char *)head))
		{
			char * s  = strstr((const char *)Msg, (const char *)head);
			s = s + strlen(head);
			int tim = ((*s)-'0')*100+((*(s+1))-'0')*10+((*(s+2))-'0');
			if(tim<999&&tim>0)
			{
				devicePrice = tim;
			}
			else
			{
				devicePrice = 100;
			}
		}
	}
	printf("device price Updaet from flash is %d\r\n", devicePrice);
	vPortFree(Msg);
}

int GetCurrentMax(void)
{
	return currentMax;
}

void SetCurrentMax(int c)            
{
	if ((c < 0)&&(c > 9999)) 
	{
		printf("current max is invilid\r\n");
		return ;
	}
	currentMax = c;
	char * Msg = pvPortMalloc(64);
	memset(Msg, 0, 64);
  char current[5] = {0};
	current[0] = (c/1000)+'0';
	current[1] = (c/100)%10+'0';
	current[2] = (c/10)%10+'0';
	current[3] = c%10+'0';
	
	const char * head = "current";
	
	const char * end = "status";
	
	strcpy(Msg, head);
	strcat(Msg, current);
	strcat(Msg, end);
	STMFLASH_Write(CURRENT_MAX_ADDR, (u32*)Msg, 10);
	vPortFree(Msg);
}

void UpdateCurrentMax(void)
{
	char * s = NULL;
	char * Msg = pvPortMalloc(64);
	memset(Msg, 0, 64);
	
	const char * head = "current";
	const char * end = "status";

	
	STMFLASH_Read(CURRENT_MAX_ADDR, (u32*)Msg, 10);
	//	printf("Msg %s\n", Msg);

		if(NULL != strstr((const char *)Msg, (const char *)end))
  	{
			if (NULL != strstr((const char *)Msg, (const char *)head))
			{
				s = strstr((const char *)Msg,(const char *)head);
				s = s + strlen(head);
				int tim = ((*s)-'0')*1000+((*(s+1))-'0')*100+((*(s+2))-'0')*10+((*(s+3))-'0');
				if(tim<9999&&tim>0)
				{
					currentMax = tim;
				}
				else
				{
					currentMax = 3000;
				}
				
			}
	  }
	printf("device currentMax is %d\r\n", currentMax);
	vPortFree(Msg);
}


void RepPara(void)
{
	char * js = NULL;
	cJSON *actJson = cJSON_CreateObject();
	cJSON_AddStringToObject(actJson, "type", "freok");
	js = cJSON_Print(actJson); 
	cJSON_Delete(actJson);
	char * msg = pvPortMalloc(150);
	memset(msg,0,150);
	strcpy(msg, js);
	MessageSend(msg,1);
	vPortFree(msg);
}