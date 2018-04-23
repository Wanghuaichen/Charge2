#pragma once
#include "sys.h"

extern int GPSReceiveFromISR(char *msg);
extern void GPSReceiveTask(void *pArg);
extern void GpsUpload(void);