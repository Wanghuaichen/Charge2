#include "insert.h"
#include "port.h"

void UpdateInsertStatus(void *pAgr)
{
	for(int i=1;i<21;i++)
	{
		if(PortGetInsertPinValue(i)&&(PortGetUseStatus(i)==0))
		{
			PortSetInsertStatus(i,1);
		}
		PortSetInsertStatus(i,0);
	}
}