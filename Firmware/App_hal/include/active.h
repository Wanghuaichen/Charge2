#pragma once
#include "sys.h"

#define RELEASE "release"
#define ACTIVATE "actives"

extern void ReleaseDevice(void);
extern void ActivateDevice(char *buf);
extern void RepActive(void);