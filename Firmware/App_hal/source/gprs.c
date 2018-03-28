#pragma once
#include "sys.h"
#include "gprs.h"





Gprs G510={
	.status = 0,
	.rep[0] = "OK0",
	.rep[1] = "OK1",
	.rep[2] = "OK2",
	.rep[3] = "OK3",
	.rep[4] = "OK4",
	.rep[5] = "OK5",
	.rep[6] = "OK6",
	.rep[7] = "OK7",
	.rep[8] = "OK8s",
	.cmdId = 0,
	.IPFlashAddr = 0,
	.OtaIPFlashAddr =0,
	.Connect = deviceConnect,
};
void deviceConfig(struct GPRS* gprs)
{
	gprs->cmdId = 0;
	gprs->status = 0;
	gprs->ConnectBinarySemaphore = xSemaphoreCreateBinary();
}
int deviceConnect(void)
{
	
	
}