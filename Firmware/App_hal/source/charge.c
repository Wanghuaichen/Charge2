#include "charge.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "gprs.h"
#include "message.h"
#include "timers.h"
#include "flash.h"

static QueueHandle_t ChargeCmdQueue;    
/*
���ָ����ն��г�ʼ������
*/
void ChargeConfig(void)
{
	
}