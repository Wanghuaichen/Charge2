#pragma once 
#include "sys.h"

typedef struct DxPort 
{
	u8 portNumber;
	uint16_t controlPin;
	uint16_t displayLedPin;
	uint16_t adcPin;
	uint16_t loadPin;
	ADC_HandleTypeDef adcHandler;
	ADC_TypeDef * ADCX;
	u8 adcChannel;
	GPIO_TypeDef * controlPinGPIOX;
	GPIO_TypeDef * displayLedPinGPIOX;
	GPIO_TypeDef * adcPinGPIOX;
	GPIO_TypeDef * loadPinGPIOX;
	
	u8 useStatus;
	float useTime;
	u8 finishFlag;
	u8 errorFlag;

	const u32 useStatusFlashAddr;
	const u32 useTimeFlashAddr;
	const u32 finishFlagFlashAddr;
	const u32 errorFlagFlashAddr;

	
	void(*SetUseStatus)(struct DxPort *port);
	void(*SetErrorStatus)(struct DxPort *port);
	void(*SetFinishStatus)(struct DxPort *port, u8 status);
	void(*SetUseTime)(struct DxPort *port, float time);
	
	int(*GetUseStatus)(struct DxPort *port);
	int(*GetErrorStatus)(struct DxPort *port);
	int(*GetAdcValue)(struct DxPort *port);
	float(*GetUseTime)(struct DxPort *port);
	int(*GetFinishStatus)(struct DxPort *port);
	
	void(*ClearUseStatus)(struct DxPort *port);
	void(*ClearUseTime)(struct DxPort *port);
	void(*ClearErrorStatus)(struct DxPort *port);
	void(*ClearFinishStatus)(struct DxPort *port);
	
	void(*UpdateUseTime)(struct DxPort *port);
	void(*UpdateErrorStatus)(struct DxPort *port);
	void(*UpdateUseStatus)(struct DxPort *port);
	void(*UpdateFinishStatus)(struct DxPort *port);
	
	void(*SubUseTime)(struct DxPort *port);
	void(*PortInit)(struct DxPort *port);
}DevicePort;




extern void deviceSetUseStatus(struct DxPort *port);
extern void deviceSetFinishStatus(struct DxPort *port, u8 status);
extern void deviceSetUseTime(struct DxPort *port, float time);
extern void deviceSetErrorStatus(struct DxPort *port);


extern int  deviceGetAdcValue(struct DxPort *port);
extern float  deviceGetUseTime(struct DxPort *port);
extern int  deviceGetFinishStatus(struct DxPort *port);
extern int  deviceGetUseStatus(struct DxPort *port);
extern int  deviceGetErrorStatus(struct DxPort *port);

extern void deviceClearUseStatus(struct DxPort *port);
extern void deviceClearFinishStatus(struct DxPort *port);
extern void deviceClearErrorStatus(struct DxPort *port);
extern void deviceClearUseTime(struct DxPort *port);



extern void deviceUpdateUseTime(struct DxPort *port);
extern void deviceUpdateUseStatus(struct DxPort *port);
extern void deviceUpdateErrorStatus(struct DxPort *port);
extern void deviceUpdateFinishStatus(struct DxPort *port);
	
extern void deviceSubUseTime(struct DxPort *port);
extern void devicePortInit(struct DxPort *port);


extern void PortSetUseStatus(u8 portnumber);
extern void PortSetErrorStatus(u8 portnumber);
extern void PortSetFinishStatus(u8 portnumber, u8 status);
extern void PortSetUseTime(u8 portnumber, float time);

extern void PortClearUseStatus(u8 portnumber);
extern void PortClearUseTime(u8 portnumber);
extern void PortClearFinishStatus(u8 portnumber);

extern int  GetPortUseTime(u8 portnumber);
extern void PortUpdateUseTime(u8 portnumber);
extern void PortUpdateUseStatus(u8 portnumber);
extern void PortUpdateFinishStatus(u8 portnumber);
extern void PortUpdateErrorStatus(u8 portnumber);


extern int PortGetUseStatus(u8 portnumber);
extern float PortGetUseTime(u8 portnumber);
extern int PortGetErrorStatus(u8 portnumber);
extern int PortGetFinishStatus(u8 portnumber);
extern int PortGetAdcValue(u8 portnumber);

extern void PortSubUseTime(u8 portnumber);
extern void PortConfig(u8 portnumber);
extern void PortChargeFinish(u8 portnumber, u8 status);
extern int  GetChargePortNumber(u8 *buf);
extern int  GetChargeMoney(u8 *buf);
extern int  GetStopPortNumber(u8 *buf);
extern int  GetErrorPortNumber(u8 *buf);
extern void StartCharge(u8 *buf);
extern void StopCharge(u8 *buf);
extern void PortError(u8 *buf);
extern void PortSleep(u8 port);




extern int CheckPortErrorCmd(u8 * buf);


extern void PortErrorRep(int port);
extern void StopChargeRep(int port);