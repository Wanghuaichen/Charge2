#pragma once
#include "stdio.h"
#include "sys.h"

#define usart1RecBufLen  			512  
#define usart3RecBufLen  			120 
#define usart2RecBufLen  			1024 

extern void LogInit(unsigned int bound);
extern void Usart3Init(unsigned int bound);
extern void Usart2Init(void);

extern void UsartTest(uint8_t *Data, uint32_t len);

extern unsigned char  usart1RecBuf[usart1RecBufLen];
extern unsigned char  usart3RecBuf[usart3RecBufLen];
extern unsigned char  usart2RecBuf[usart2RecBufLen];

extern volatile unsigned short int usart1RecLen;
extern volatile unsigned short int usart3RecLen;
extern volatile unsigned short int usart2RecLen;

extern void UsartPrint(unsigned char v);
extern void UsartWrite(const char *Data);

extern void UsartEnable(u8 usartid);
extern void UsartDisable(u8 usartid);
