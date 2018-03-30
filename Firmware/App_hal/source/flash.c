#include "flash.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include <stm32_hal_legacy.h>
#include "FreeRTOS.h"
#include "task.h"


#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 
#else 
#define STM_SECTOR_SIZE	2048
#endif		 
u32 STMFLASH_BUF[STM_SECTOR_SIZE / 4]; 

u32 STMFLASH_ReadWord(u32 faddr)
{
	return *(vu32*)faddr; 
}

void STMFLASH_Write(u32 WriteAddr, u32 *pBuffer, u32 NumToWrite)
{
	taskENTER_CRITICAL();     /*进入临界区*/
	FLASH_EraseInitTypeDef FlashEraseInit;
	HAL_StatusTypeDef FlashStatus = HAL_OK;
	u32 offaddr;   //去掉0X08000000后的地址
	u32 SectorError = 0;
	u32 secnum;	   /*扇区地址*/
	u32 secoff;	   //扇区内偏移地址
	offaddr = WriteAddr - STM32_FLASH_BASE;	
	secnum = offaddr / STM_SECTOR_SIZE;
	STMFLASH_Read(secnum*STM_SECTOR_SIZE + STM32_FLASH_BASE, STMFLASH_BUF, STM_SECTOR_SIZE/4);
	secoff = (offaddr % STM_SECTOR_SIZE) / 4;
	for (int i = 0; i < NumToWrite;i++)
	{
		STMFLASH_BUF[secoff + i] = pBuffer[i];
	}
	FlashEraseInit.TypeErase = FLASH_TYPEERASE_PAGES; 
	FlashEraseInit.PageAddress = secnum*STM_SECTOR_SIZE + STM32_FLASH_BASE;
	FlashEraseInit.NbPages = 1;
	HAL_FLASH_Unlock();                      //解锁     
	HAL_FLASHEx_Erase(&FlashEraseInit, &SectorError);
	HAL_FLASH_Lock(); 
	FlashStatus = FLASH_WaitForLastOperation(FLASH_WAITETIME);   
	if (HAL_FLASHEx_Erase(&FlashEraseInit, &SectorError) != HAL_OK) 
	{
	   return ; 
	}
	STMFLASH_Store(secnum*STM_SECTOR_SIZE + STM32_FLASH_BASE, STMFLASH_BUF, STM_SECTOR_SIZE);
	taskEXIT_CRITICAL();      /*退出临界区*/
}
void STMFLASH_Store(u32 WriteAddr, u32 *pBuffer, u32 NumToWrite)	
{ 
	HAL_StatusTypeDef FlashStatus = HAL_OK;	
	u32 addrx = 0;
	u32 endaddr = 0;	
	if (WriteAddr < STM32_FLASH_BASE || WriteAddr % 4)return;	 //小于0x08000000 的非法地址
	HAL_FLASH_Unlock();                     //解锁     
	addrx = WriteAddr;	                    //写入的起始地址			 
	endaddr = WriteAddr + NumToWrite * 4;   //写入的结束地址	           
	while (WriteAddr < endaddr) 
		{
			if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, WriteAddr, *pBuffer) != HAL_OK) 
			{ 
				break;	 
			}
			WriteAddr += 4;
			pBuffer++;
		}  
	HAL_FLASH_Lock(); 
	delay_xms(5);
} 
 
void STMFLASH_Read(u32 ReadAddr, u32 *pBuffer, u32 NumToRead)   	
{
	u32 i;
	for (i = 0; i < NumToRead; i++)
	{
		pBuffer[i] = STMFLASH_ReadWord(ReadAddr); 
		ReadAddr += 4; 
	}
}


