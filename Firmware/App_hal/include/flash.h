#pragma once
#include "sys.h"
#include "stm32f1xx_hal_flash_ex.h"

//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
#define FLASH_WAITETIME  50000          //FLASH等待超时时间

#define STM32_FLASH_SIZE 512 	 		 
#define STM32_FLASH_WREN 1             

extern void STMFLASH_Write(u32 WriteAddr, u32 *pBuffer, u32 NumToWrite);		//从指定地址开始写入指定长度的数据
extern void STMFLASH_Read(u32 ReadAddr, u32 *pBuffer, u32 NumToRead);   		//从指定地址开始读出指定长度的数据
extern void STMFLASH_Store(u32 ReadAddr, u32 *pBuffer, u32 NumToRead);     		//从指定地址开始读出指定长度的数据
