#include "storemoney.h"
#include "port.h"

void StoreMoneyTask(void)                
{
	for (int i = 1; i < 21; i++)
	{
		if (PortGetErrorStatus(i) == 0)                   
			{
				if ((PortGetUseStatus(i)==1)&&(PortGetFinishStatus(i) == 0))     
					{
						float tmp = PortGetUseTime(i);
						printf("port %d money %f\r\n",i+1,tmp);
						PortSetUseTime(i, tmp);                                        
					}
			}
	}
}