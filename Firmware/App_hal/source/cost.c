#include "cost.h"
#include "port.h"
#include "para.h"


#define CURRENT_MIN        100    //电池充电完成后端口的  输出电流  阈值

#define CHARGE_OVER        1
#define MONEY_OVER         4


void PortSubUseTime(u8 portnumber)
{
	
//	static int stopCheck[20] = { 0 };

//	if (PortGetUseTime(portnumber) < 1)              //费用不足  充电停止
//		{
//			PortChargeFinish(portnumber, MONEY_OVER);
//			return ;
//		}
//	else if (PortGetAdcValue(portnumber) >= CURRENT_MIN + 10)  //充电正常，继续扣费
//		{
//			static u8 costCount = 0;
//			costCount++;
//			if (costCount >= 60)  //3min扣一次费用
//			{
//				costCount = 0;
//				float p = GetPrice();
//				printf("price is %d\r\n", (int)p);
//				float cur =  GetCurrentValue(PortGetAdcValue(portnumber));
//				printf("currten is %f\r\n", cur);
//	         	float cost = (p)*(cur *0.22)*(0.05);
//				printf("cost %f\r\n", cost);
//				Port[portnumber].useTime = Port[portnumber].useTime - cost;
//				char m[20] = { 0};
//				sprintf(m, "%f", Port[portnumber].useTime);
//            	printf("money %s\r\n",m);
//				return ;
//			}
//		}
//	else if (PortGetAdcValue(portnumber) <= CURRENT_MIN)    //充电电流减小
//		{
//			stopCheck[portnumber]++;
//			if (stopCheck[portnumber] >= 3)
//			{
//				stopCheck[portnumber] = 0;
//				PortChargeFinish(portnumber, CHARGE_OVER);      //连续3次减小，则充电完成
//			}
//		}
}