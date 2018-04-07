#include "charge.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "gprs.h"
#include "message.h"
#include "timers.h"
#include "flash.h"

static QueueHandle_t ChargeCmdQueue;    
/*
充电指令接收队列初始化函数
*/
void ChargeConfig(void)
{
	
}