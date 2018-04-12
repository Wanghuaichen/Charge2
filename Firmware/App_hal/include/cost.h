#pragma once
#include "sys.h"


#define CURRENT_MIN        100    //电池充电完成后端口的  输出电流  阈值

#define CHARGE_OVER        1
#define MONEY_OVER         4

#define FINISH_REP  "statusrepok"

extern float GetCurrentValue(u16 adcValue);

extern int GetPortNumberFromJson(u8 *buf);
extern void PostFinishTask(void * pAgr);