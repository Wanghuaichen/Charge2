#pragma once
#include "sys.h"

#define CHECK_GET   "checkget"

extern int PostChaRep(int port);
extern int GetRepPort(char* buf);
extern void ChaRepCheckTask(void* pAgr);