#pragma once
#include "sys.h"



extern void UpDateParaFromFlash(void);
extern void SetPareStatus(u8 *buf);

extern int CheckParaCmd(u8 *buf);

extern int   GetPrice(void);
extern void  SetPrice(int p);            
extern void  UpdatePrice(void);
extern int   GetCurrentMax(void);
extern void  SetCurrentMax(int c);            
extern void  UpdateCurrentMax(void);
extern void RepPara(void);