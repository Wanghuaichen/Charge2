#include "device.h"
#include "para.h"
#include "port.h"



void DeviceConfig(void)
{
	UpdateCurrentMax();
	UpdatePrice();
	for (int i = 1; i < 21; i++)
	{
		PortConfig(i);
	}
	for (int i = 1; i < 21; i++)
	{
		PortUpdateUseStatus(i);
		PortUpdateUseTime(i);
		PortUpdateFinishStatus(i);
		PortUpdateErrorStatus(i);
	}
	printf("Update Para Succ\r\n");
}