#include "port.h"
#include "portaddr.h"
#include "FreeRTOS.h"
#include "string.h"

#define CHARGEPOWERON      0  //更换继电器后0-1状态要反置
#define CHARGEPOWEROFF     1  //更换继电器后0-1状态要反置
#define CHARGELEDON        1
#define CHARGELEDOFF       0

#define CURRENT_MIN        100    //电池充电完成后端口的  输出电流  阈值

#define CHARGE_OVER        1
#define MONEY_OVER         4

static  DevicePort Port[20] =
{ 
	//充电端口1
{
	//地址
	.useTimeFlashAddr = PORT1_TIME_ADDR,                          
	.useStatusFlashAddr = PORT1_STATUS_ADDR, 
	.finishFlagFlashAddr = PORT1_FINISH_ADDR,
	.errorFlagFlashAddr = PORT1_ERROR_ADDR,
	//状态变量
	.portNumber = 1,
	.useTime = 0.0,	
	.useStatus = 0,
	.finishFlag = 0,
	.errorFlag = 0,
	//IO
	.controlPin = GPIO_PIN_8,								 
	.controlPinGPIOX = GPIOB,
	.displayLedPin = GPIO_PIN_4,
	.displayLedPinGPIOX = GPIOA,
	.adcPin = GPIO_PIN_0,									 
	.adcPinGPIOX = GPIOC,
	.loadPin = GPIO_PIN_0,
	.loadPinGPIOX = GPIOC,
	.ADCX = ADC1,
	.adcChannel = 6,
	//回调函数
	.SetUseTime = deviceSetUseTime,
	.SetUseStatus = deviceSetUseStatus,
	.SetFinishStatus = deviceSetFinishStatus,
	.SetErrorStatus = deviceSetErrorStatus,
		
			
	.GetUseTime = deviceGetUseTime,		
	.GetAdcValue = deviceGetAdcValue,
	.GetUseStatus = deviceGetUseStatus,
	.GetErrorStatus = deviceGetErrorStatus,
	.GetFinishStatus = deviceGetFinishStatus,
			
	.ClearUseTime = deviceClearUseTime,
	.ClearUseStatus = deviceClearUseStatus,
	.ClearFinishStatus = deviceClearFinishStatus,
	.ClearErrorStatus = deviceClearErrorStatus,
			
			
	.UpdateUseTime = deviceUpdateUseTime,	
	.UpdateUseStatus = deviceUpdateUseStatus,
	.UpdateFinishStatus = deviceUpdateFinishStatus,
	.UpdateErrorStatus = deviceUpdateErrorStatus,
		
	.SubUseTime = deviceSubUseTime,
	.PortInit = devicePortInit,
},
	//充电端口2
{
	//地址
	.useTimeFlashAddr = PORT2_TIME_ADDR,                          
	.useStatusFlashAddr = PORT2_STATUS_ADDR, 
	.finishFlagFlashAddr = PORT2_FINISH_ADDR,
	.errorFlagFlashAddr = PORT2_ERROR_ADDR,
	//状态变量
	.portNumber = 2,
	.useTime = 0.0,	
	.useStatus = 0,
	.finishFlag = 0,
	.errorFlag = 0,
	//IO
	.controlPin = GPIO_PIN_2,								 
	.controlPinGPIOX = GPIOE,
	.displayLedPin = GPIO_PIN_5,
	.displayLedPinGPIOX = GPIOA,
	.adcPin = GPIO_PIN_1,									 
	.adcPinGPIOX = GPIOC,
	.loadPin = GPIO_PIN_0,
	.loadPinGPIOX = GPIOC,
	.ADCX = ADC1,
	.adcChannel = 7,
	//回调函数
	.SetUseTime = deviceSetUseTime,
	.SetUseStatus = deviceSetUseStatus,
	.SetFinishStatus = deviceSetFinishStatus,
	.SetErrorStatus = deviceSetErrorStatus,
		
			
	.GetUseTime = deviceGetUseTime,		
	.GetAdcValue = deviceGetAdcValue,
	.GetUseStatus = deviceGetUseStatus,
	.GetErrorStatus = deviceGetErrorStatus,
	.GetFinishStatus = deviceGetFinishStatus,
			
	.ClearUseTime = deviceClearUseTime,
	.ClearUseStatus = deviceClearUseStatus,
	.ClearFinishStatus = deviceClearFinishStatus,
	.ClearErrorStatus = deviceClearErrorStatus,
			
			
	.UpdateUseTime = deviceUpdateUseTime,	
	.UpdateUseStatus = deviceUpdateUseStatus,
	.UpdateFinishStatus = deviceUpdateFinishStatus,
	.UpdateErrorStatus = deviceUpdateErrorStatus,
		
	.SubUseTime = deviceSubUseTime,
	.PortInit = devicePortInit,
},
	//充电端口3
{
	//地址
	.useTimeFlashAddr = PORT3_TIME_ADDR,                          
	.useStatusFlashAddr = PORT3_STATUS_ADDR, 
	.finishFlagFlashAddr = PORT3_FINISH_ADDR,
	.errorFlagFlashAddr = PORT3_ERROR_ADDR,
	//状态变量
	.portNumber = 3,
	.useTime = 0.0,	
	.useStatus = 0,
	.finishFlag = 0,
	.errorFlag = 0,
	.errorFlag = 0,
	//IO
	.controlPin = GPIO_PIN_3,								 
	.controlPinGPIOX = GPIOE,
	.displayLedPin = GPIO_PIN_6,
	.displayLedPinGPIOX = GPIOA,
	.adcPin = GPIO_PIN_2,									 
	.adcPinGPIOX = GPIOC,
	.ADCX = ADC1,
	.adcChannel = 8,
	//回调函数
	.SetUseTime = deviceSetUseTime,
	.SetUseStatus = deviceSetUseStatus,
	.SetFinishStatus = deviceSetFinishStatus,
	.SetErrorStatus = deviceSetErrorStatus,
		
			
	.GetUseTime = deviceGetUseTime,		
	.GetAdcValue = deviceGetAdcValue,
	.GetUseStatus = deviceGetUseStatus,
	.GetErrorStatus = deviceGetErrorStatus,
	.GetFinishStatus = deviceGetFinishStatus,
			
	.ClearUseTime = deviceClearUseTime,
	.ClearUseStatus = deviceClearUseStatus,
	.ClearFinishStatus = deviceClearFinishStatus,
	.ClearErrorStatus = deviceClearErrorStatus,
			
			
	.UpdateUseTime = deviceUpdateUseTime,	
	.UpdateUseStatus = deviceUpdateUseStatus,
	.UpdateFinishStatus = deviceUpdateFinishStatus,
	.UpdateErrorStatus = deviceUpdateErrorStatus,
		
	.SubUseTime = deviceSubUseTime,
	.PortInit = devicePortInit,
},
	//充电端口4
{
	//地址
	.useTimeFlashAddr = PORT4_TIME_ADDR,                          
	.useStatusFlashAddr = PORT4_STATUS_ADDR, 
	.finishFlagFlashAddr = PORT4_FINISH_ADDR,
	.errorFlagFlashAddr = PORT4_ERROR_ADDR,
	//状态变量
	.portNumber = 4,
	.useTime = 0.0,	
	.useStatus = 0,
	.finishFlag = 0,
	.errorFlag = 0,
	//IO
	.controlPin = GPIO_PIN_4,								 
	.controlPinGPIOX = GPIOE,
	.displayLedPin = GPIO_PIN_7,
	.displayLedPinGPIOX = GPIOA,
	.adcPin = GPIO_PIN_3,									 
	.adcPinGPIOX = GPIOC,
	.ADCX = ADC1,
	.adcChannel = 9,
	//回调函数
	.SetUseTime = deviceSetUseTime,
	.SetUseStatus = deviceSetUseStatus,
	.SetFinishStatus = deviceSetFinishStatus,
		
			
	.GetUseTime = deviceGetUseTime,		
	.GetAdcValue = deviceGetAdcValue,
	.GetUseStatus = deviceGetUseStatus,
	.GetErrorStatus = deviceGetErrorStatus,
	.GetFinishStatus = deviceGetFinishStatus,
			
	.ClearUseTime = deviceClearUseTime,
	.ClearUseStatus = deviceClearUseStatus,
	.ClearFinishStatus = deviceClearFinishStatus,
			
			
	.UpdateUseTime = deviceUpdateUseTime,	
	.UpdateUseStatus = deviceUpdateUseStatus,
	.UpdateFinishStatus = deviceUpdateFinishStatus,
	.UpdateErrorStatus = deviceUpdateErrorStatus,
		
	.SubUseTime = deviceSubUseTime,
	.PortInit = devicePortInit,
},
	//充电端口5
{
	//地址
	.useTimeFlashAddr = PORT5_TIME_ADDR,                          
	.useStatusFlashAddr = PORT5_STATUS_ADDR, 
	.finishFlagFlashAddr = PORT5_FINISH_ADDR,
	.errorFlagFlashAddr = PORT5_ERROR_ADDR,
	//状态变量
	.portNumber = 5,
	.useTime = 0.0,	
	.useStatus = 0,
	.finishFlag = 0,
	.errorFlag = 0,
	//IO
	.controlPin = GPIO_PIN_5,								 
	.controlPinGPIOX = GPIOE,
	.displayLedPin = GPIO_PIN_4,
	.displayLedPinGPIOX = GPIOC,
	.adcPin = GPIO_PIN_2,									 
	.adcPinGPIOX = GPIOF,
	.ADCX = ADC1,
	.adcChannel = 10,
	//回调函数
	.SetUseTime = deviceSetUseTime,
	.SetUseStatus = deviceSetUseStatus,
	.SetFinishStatus = deviceSetFinishStatus,
	.SetErrorStatus = deviceSetErrorStatus,
		
			
	.GetUseTime = deviceGetUseTime,		
	.GetAdcValue = deviceGetAdcValue,
	.GetUseStatus = deviceGetUseStatus,
	.GetErrorStatus = deviceGetErrorStatus,
	.GetFinishStatus = deviceGetFinishStatus,
			
	.ClearUseTime = deviceClearUseTime,
	.ClearUseStatus = deviceClearUseStatus,
	.ClearFinishStatus = deviceClearFinishStatus,
	.ClearErrorStatus = deviceClearErrorStatus,
			
			
	.UpdateUseTime = deviceUpdateUseTime,	
	.UpdateUseStatus = deviceUpdateUseStatus,
	.UpdateFinishStatus = deviceUpdateFinishStatus,
	.UpdateErrorStatus = deviceUpdateErrorStatus,
		
	.SubUseTime = deviceSubUseTime,
	.PortInit = devicePortInit,
},
	//充电端口6
{
	//地址
	.useTimeFlashAddr = PORT6_TIME_ADDR,                          
	.useStatusFlashAddr = PORT6_STATUS_ADDR, 
	.finishFlagFlashAddr = PORT6_FINISH_ADDR,
	.errorFlagFlashAddr = PORT6_ERROR_ADDR,
	//状态变量
	.portNumber = 6,
	.useTime = 0.0,	
	.useStatus = 0,
	.finishFlag = 0,
	.errorFlag = 0,
	//IO
	.controlPin = GPIO_PIN_6,								 
	.controlPinGPIOX = GPIOE,
	.displayLedPin = GPIO_PIN_5,
	.displayLedPinGPIOX = GPIOC,
	.adcPin = GPIO_PIN_0,									 
	.adcPinGPIOX = GPIOA,
	.ADCX = ADC1,
	.adcChannel = 1,
	//回调函数
	.SetUseTime = deviceSetUseTime,
	.SetUseStatus = deviceSetUseStatus,
	.SetFinishStatus = deviceSetFinishStatus,
	.SetErrorStatus = deviceSetErrorStatus,
		
			
	.GetUseTime = deviceGetUseTime,		
	.GetAdcValue = deviceGetAdcValue,
	.GetUseStatus = deviceGetUseStatus,
	.GetErrorStatus = deviceGetErrorStatus,
	.GetFinishStatus = deviceGetFinishStatus,
			
	.ClearUseTime = deviceClearUseTime,
	.ClearUseStatus = deviceClearUseStatus,
	.ClearFinishStatus = deviceClearFinishStatus,
	.ClearErrorStatus = deviceClearErrorStatus,
			
			
	.UpdateUseTime = deviceUpdateUseTime,	
	.UpdateUseStatus = deviceUpdateUseStatus,
	.UpdateFinishStatus = deviceUpdateFinishStatus,
	.UpdateErrorStatus = deviceUpdateErrorStatus,
		
	.SubUseTime = deviceSubUseTime,
	.PortInit = devicePortInit,
},
	//充电端口7
{
	//地址
	.useTimeFlashAddr = PORT7_TIME_ADDR,                          
	.useStatusFlashAddr = PORT7_STATUS_ADDR, 
	.finishFlagFlashAddr = PORT7_FINISH_ADDR,
	.errorFlagFlashAddr = PORT7_ERROR_ADDR,
	//状态变量
	.portNumber = 7,
	.useTime = 0.0,	
	.useStatus = 0,
	.finishFlag = 0,
	.errorFlag = 0,
	//IO
	.controlPin = GPIO_PIN_13,								 
	.controlPinGPIOX = GPIOC,
	.displayLedPin = GPIO_PIN_0,
	.displayLedPinGPIOX = GPIOB,
	.adcPin = GPIO_PIN_1,									 
	.adcPinGPIOX = GPIOA,
	.ADCX = ADC1,
	.adcChannel = 2,
	//回调函数
	.SetUseTime = deviceSetUseTime,
	.SetUseStatus = deviceSetUseStatus,
	.SetFinishStatus = deviceSetFinishStatus,
	.SetErrorStatus = deviceSetErrorStatus,
		
			
	.GetUseTime = deviceGetUseTime,		
	.GetAdcValue = deviceGetAdcValue,
	.GetUseStatus = deviceGetUseStatus,
	.GetErrorStatus = deviceGetErrorStatus,
	.GetFinishStatus = deviceGetFinishStatus,
			
	.ClearUseTime = deviceClearUseTime,
	.ClearUseStatus = deviceClearUseStatus,
	.ClearFinishStatus = deviceClearFinishStatus,
	.ClearErrorStatus = deviceClearErrorStatus,
			
			
	.UpdateUseTime = deviceUpdateUseTime,	
	.UpdateUseStatus = deviceUpdateUseStatus,
	.UpdateFinishStatus = deviceUpdateFinishStatus,
	.UpdateErrorStatus = deviceUpdateErrorStatus,
		
	.SubUseTime = deviceSubUseTime,
	.PortInit = devicePortInit,
},
	//充电端口8
{
	//地址
	.useTimeFlashAddr = PORT8_TIME_ADDR,                          
	.useStatusFlashAddr = PORT8_STATUS_ADDR, 
	.finishFlagFlashAddr = PORT8_FINISH_ADDR,
	.errorFlagFlashAddr = PORT8_ERROR_ADDR,
	//状态变量
	.portNumber = 8,
	.useTime = 0.0,	
	.useStatus = 0,
	.finishFlag = 0,
	.errorFlag = 0,
	//IO
	.controlPin = GPIO_PIN_9,								 
	.controlPinGPIOX = GPIOF,
	.displayLedPin = GPIO_PIN_1,
	.displayLedPinGPIOX = GPIOB,
	.adcPin = GPIO_PIN_2,									 
	.adcPinGPIOX = GPIOA,
	.ADCX = ADC1,
	.adcChannel = 3,
	//回调函数
	.SetUseTime = deviceSetUseTime,
	.SetUseStatus = deviceSetUseStatus,
	.SetFinishStatus = deviceSetFinishStatus,
	.SetErrorStatus = deviceSetErrorStatus,
		
			
	.GetUseTime = deviceGetUseTime,		
	.GetAdcValue = deviceGetAdcValue,
	.GetUseStatus = deviceGetUseStatus,
	.GetErrorStatus = deviceGetErrorStatus,
	.GetFinishStatus = deviceGetFinishStatus,
			
	.ClearUseTime = deviceClearUseTime,
	.ClearUseStatus = deviceClearUseStatus,
	.ClearFinishStatus = deviceClearFinishStatus,
	.ClearErrorStatus = deviceClearErrorStatus,
			
			
	.UpdateUseTime = deviceUpdateUseTime,	
	.UpdateUseStatus = deviceUpdateUseStatus,
	.UpdateFinishStatus = deviceUpdateFinishStatus,
	.UpdateErrorStatus = deviceUpdateErrorStatus,
		
	.SubUseTime = deviceSubUseTime,
	.PortInit = devicePortInit,
},
	//充电端口9
{
	//地址
	.useTimeFlashAddr = PORT9_TIME_ADDR,                          
	.useStatusFlashAddr = PORT9_STATUS_ADDR, 
	.finishFlagFlashAddr = PORT9_FINISH_ADDR,
	.errorFlagFlashAddr = PORT9_ERROR_ADDR,
	//状态变量
	.portNumber = 9,
	.useTime = 0.0,	
	.useStatus = 0,
	.finishFlag = 0,
	.errorFlag = 0,
	//IO
	.controlPin = GPIO_PIN_10,								 
	.controlPinGPIOX = GPIOF,
	.displayLedPin = GPIO_PIN_2,
	.displayLedPinGPIOX = GPIOB,
	.adcPin = GPIO_PIN_3,									 
	.adcPinGPIOX = GPIOA,
	.ADCX = ADC1,
	.adcChannel = 4,
	//回调函数
	.SetUseTime = deviceSetUseTime,
	.SetUseStatus = deviceSetUseStatus,
	.SetFinishStatus = deviceSetFinishStatus,
	.SetErrorStatus = deviceSetErrorStatus,
		
			
	.GetUseTime = deviceGetUseTime,		
	.GetAdcValue = deviceGetAdcValue,
	.GetUseStatus = deviceGetUseStatus,
	.GetErrorStatus = deviceGetErrorStatus,
	.GetFinishStatus = deviceGetFinishStatus,
			
	.ClearUseTime = deviceClearUseTime,
	.ClearUseStatus = deviceClearUseStatus,
	.ClearFinishStatus = deviceClearFinishStatus,
	.ClearErrorStatus = deviceClearErrorStatus,
			
			
	.UpdateUseTime = deviceUpdateUseTime,	
	.UpdateUseStatus = deviceUpdateUseStatus,
	.UpdateFinishStatus = deviceUpdateFinishStatus,
	.UpdateErrorStatus = deviceUpdateErrorStatus,
		
	.SubUseTime = deviceSubUseTime,
	.PortInit = devicePortInit,
},
	//充电端口10
{
	//地址
	.useTimeFlashAddr = PORT10_TIME_ADDR,                          
	.useStatusFlashAddr = PORT10_STATUS_ADDR, 
	.finishFlagFlashAddr = PORT10_FINISH_ADDR,
	.errorFlagFlashAddr = PORT10_ERROR_ADDR,
	//状态变量
	.portNumber = 10,
	.useTime = 0.0,	
	.useStatus = 0,
	.finishFlag = 0,
	.errorFlag = 0,
	//IO
	.controlPin = GPIO_PIN_6,								 
	.controlPinGPIOX = GPIOB,
	.displayLedPin = GPIO_PIN_7,
	.displayLedPinGPIOX = GPIOE,
	.adcPin = GPIO_PIN_4,									 
	.adcPinGPIOX = GPIOF,
	.ADCX = ADC1,
	.adcChannel = 5,
	//回调函数
	.SetUseTime = deviceSetUseTime,
	.SetUseStatus = deviceSetUseStatus,
	.SetFinishStatus = deviceSetFinishStatus,
	.SetErrorStatus = deviceSetErrorStatus,
		
			
	.GetUseTime = deviceGetUseTime,		
	.GetAdcValue = deviceGetAdcValue,
	.GetUseStatus = deviceGetUseStatus,
	.GetErrorStatus = deviceGetErrorStatus,
	.GetFinishStatus = deviceGetFinishStatus,
			
	.ClearUseTime = deviceClearUseTime,
	.ClearUseStatus = deviceClearUseStatus,
	.ClearFinishStatus = deviceClearFinishStatus,
	.ClearErrorStatus = deviceClearErrorStatus,
			
			
	.UpdateUseTime = deviceUpdateUseTime,	
	.UpdateUseStatus = deviceUpdateUseStatus,
	.UpdateFinishStatus = deviceUpdateFinishStatus,
	.UpdateErrorStatus = deviceUpdateErrorStatus,
		
	.SubUseTime = deviceSubUseTime,
	.PortInit = devicePortInit,
},
	//充电端口11
{
		//地址
		.useTimeFlashAddr = PORT11_TIME_ADDR,                          
		.useStatusFlashAddr = PORT11_STATUS_ADDR, 
		.finishFlagFlashAddr = PORT11_FINISH_ADDR,
		.errorFlagFlashAddr = PORT11_ERROR_ADDR,
		//状态变量
		.portNumber = 11,
		.useTime = 0.0,	
		.useStatus = 0,
		.finishFlag = 0,
		.errorFlag = 0,
		//IO
		.controlPin = GPIO_PIN_12,								 
		.controlPinGPIOX = GPIOA,
		.displayLedPin = GPIO_PIN_13,
		.displayLedPinGPIOX = GPIOD,
		.adcPin = GPIO_PIN_8,									 
		.adcPinGPIOX = GPIOE,
		.ADCX = ADC3,
		.adcChannel = 6,
		//回调函数
		.SetUseTime = deviceSetUseTime,
		.SetUseStatus = deviceSetUseStatus,
		.SetFinishStatus = deviceSetFinishStatus,
		.SetErrorStatus = deviceSetErrorStatus,
		
			
		.GetUseTime = deviceGetUseTime,		
		.GetAdcValue = deviceGetAdcValue,
		.GetUseStatus = deviceGetUseStatus,
	    .GetErrorStatus = deviceGetErrorStatus,
		.GetFinishStatus = deviceGetFinishStatus,
			
		.ClearUseTime = deviceClearUseTime,
		.ClearUseStatus = deviceClearUseStatus,
		.ClearFinishStatus = deviceClearFinishStatus,
		.ClearErrorStatus = deviceClearErrorStatus,
			
			
		.UpdateUseTime = deviceUpdateUseTime,	
		.UpdateUseStatus = deviceUpdateUseStatus,
		.UpdateFinishStatus = deviceUpdateFinishStatus,
		.UpdateErrorStatus = deviceUpdateErrorStatus,
		
		.SubUseTime = deviceSubUseTime,
		.PortInit = devicePortInit,
	},
	//充电端口12
{
		//地址
		.useTimeFlashAddr = PORT12_TIME_ADDR,                          
		.useStatusFlashAddr = PORT12_STATUS_ADDR, 
		.finishFlagFlashAddr = PORT12_FINISH_ADDR,
		.errorFlagFlashAddr = PORT12_ERROR_ADDR,
		//状态变量
		.portNumber = 12,
		.useTime = 0.0,	
		.useStatus = 0,
		.finishFlag = 0,
		.errorFlag = 0,
		//IO
		.controlPin = GPIO_PIN_14,								 
		.controlPinGPIOX = GPIOB,
		.displayLedPin = GPIO_PIN_14,
		.displayLedPinGPIOX = GPIOD,
		.adcPin = GPIO_PIN_9,									 
		.adcPinGPIOX = GPIOE,
		.ADCX = ADC3,
		.adcChannel = 2,
		//回调函数
		.SetUseTime = deviceSetUseTime,
		.SetUseStatus = deviceSetUseStatus,
		.SetFinishStatus = deviceSetFinishStatus,
		.SetErrorStatus = deviceSetErrorStatus,
		
			
		.GetUseTime = deviceGetUseTime,		
		.GetAdcValue = deviceGetAdcValue,
		.GetUseStatus = deviceGetUseStatus,
    	.GetErrorStatus = deviceGetErrorStatus,
	 	.GetFinishStatus = deviceGetFinishStatus,
			
		.ClearUseTime = deviceClearUseTime,
		.ClearUseStatus = deviceClearUseStatus,
		.ClearFinishStatus = deviceClearFinishStatus,
		.ClearErrorStatus = deviceClearErrorStatus,
			
			
		.UpdateUseTime = deviceUpdateUseTime,	
		.UpdateUseStatus = deviceUpdateUseStatus,
		.UpdateFinishStatus = deviceUpdateFinishStatus,
		.UpdateErrorStatus = deviceUpdateErrorStatus,
		
		.SubUseTime = deviceSubUseTime,
		.PortInit = devicePortInit,
	},
	//充电端口13
{
		//地址
		.useTimeFlashAddr = PORT13_TIME_ADDR,                          
		.useStatusFlashAddr = PORT13_STATUS_ADDR, 
		.finishFlagFlashAddr = PORT13_FINISH_ADDR,
		.errorFlagFlashAddr = PORT13_ERROR_ADDR,
		//状态变量
		.portNumber = 13,
		.useTime = 0.0,	
		.useStatus = 0,
		.finishFlag = 0,
		.errorFlag = 0,
		.errorFlag = 0,
		//IO
		.controlPin = GPIO_PIN_15,								 
		.controlPinGPIOX = GPIOB,
		.displayLedPin = GPIO_PIN_15,
		.displayLedPinGPIOX = GPIOD,
		.adcPin = GPIO_PIN_10,									 
		.adcPinGPIOX = GPIOE,
		.ADCX = ADC3,
		.adcChannel = 14,
		//回调函数
		.SetUseTime = deviceSetUseTime,
		.SetUseStatus = deviceSetUseStatus,
		.SetFinishStatus = deviceSetFinishStatus,
		.SetErrorStatus = deviceSetErrorStatus,
		
			
		.GetUseTime = deviceGetUseTime,		
		.GetAdcValue = deviceGetAdcValue,
		.GetUseStatus = deviceGetUseStatus,
		.GetErrorStatus = deviceGetErrorStatus,
		.GetFinishStatus = deviceGetFinishStatus,
			
		.ClearUseTime = deviceClearUseTime,
		.ClearUseStatus = deviceClearUseStatus,
		.ClearFinishStatus = deviceClearFinishStatus,
		.ClearErrorStatus = deviceClearErrorStatus,
			
			
		.UpdateUseTime = deviceUpdateUseTime,	
		.UpdateUseStatus = deviceUpdateUseStatus,
		.UpdateFinishStatus = deviceUpdateFinishStatus,
		.UpdateErrorStatus = deviceUpdateErrorStatus,
		
		.SubUseTime = deviceSubUseTime,
		.PortInit = devicePortInit,
	},
	//充电端口14
{
		//地址
		.useTimeFlashAddr = PORT14_TIME_ADDR,                          
		.useStatusFlashAddr = PORT14_STATUS_ADDR, 
		.finishFlagFlashAddr = PORT14_FINISH_ADDR,
		.errorFlagFlashAddr = PORT14_ERROR_ADDR,
		//状态变量
		.portNumber = 14,
		.useTime = 0.0,	
		.useStatus = 0,
		.finishFlag = 0,
		.errorFlag = 0,
		//IO
		.controlPin = GPIO_PIN_10,								 
		.controlPinGPIOX = GPIOB,
		.displayLedPin = GPIO_PIN_6,
		.displayLedPinGPIOX = GPIOC,
		.adcPin = GPIO_PIN_11,									 
		.adcPinGPIOX = GPIOE,
		.ADCX = ADC3,
		.adcChannel = 15,
		//回调函数
		.SetUseTime = deviceSetUseTime,
		.SetUseStatus = deviceSetUseStatus,
		.SetFinishStatus = deviceSetFinishStatus,
		
			
		.GetUseTime = deviceGetUseTime,		
		.GetAdcValue = deviceGetAdcValue,
		.GetUseStatus = deviceGetUseStatus,
		.GetErrorStatus = deviceGetErrorStatus,
		.GetFinishStatus = deviceGetFinishStatus,
			
		.ClearUseTime = deviceClearUseTime,
		.ClearUseStatus = deviceClearUseStatus,
		.ClearFinishStatus = deviceClearFinishStatus,
			
			
		.UpdateUseTime = deviceUpdateUseTime,	
		.UpdateUseStatus = deviceUpdateUseStatus,
		.UpdateFinishStatus = deviceUpdateFinishStatus,
		.UpdateErrorStatus = deviceUpdateErrorStatus,
		
		.SubUseTime = deviceSubUseTime,
		.PortInit = devicePortInit,
	},
	//充电端口15
{
		//地址
		.useTimeFlashAddr = PORT15_TIME_ADDR,                          
		.useStatusFlashAddr = PORT15_STATUS_ADDR, 
		.finishFlagFlashAddr = PORT15_FINISH_ADDR,
		.errorFlagFlashAddr = PORT15_ERROR_ADDR,
		//状态变量
		.portNumber = 15,
		.useTime = 0.0,	
		.useStatus = 0,
		.finishFlag = 0,
		.errorFlag = 0,
		//IO
		.controlPin = GPIO_PIN_11,								 
		.controlPinGPIOX = GPIOB,
		.displayLedPin = GPIO_PIN_7,
		.displayLedPinGPIOX = GPIOC,
		.adcPin = GPIO_PIN_12,									 
		.adcPinGPIOX = GPIOE,
		.ADCX = ADC3,
		.adcChannel = 16,
		//回调函数
		.SetUseTime = deviceSetUseTime,
		.SetUseStatus = deviceSetUseStatus,
		.SetFinishStatus = deviceSetFinishStatus,
		.SetErrorStatus = deviceSetErrorStatus,
		
			
		.GetUseTime = deviceGetUseTime,		
		.GetAdcValue = deviceGetAdcValue,
		.GetUseStatus = deviceGetUseStatus,
		.GetErrorStatus = deviceGetErrorStatus,
		.GetFinishStatus = deviceGetFinishStatus,
			
		.ClearUseTime = deviceClearUseTime,
		.ClearUseStatus = deviceClearUseStatus,
		.ClearFinishStatus = deviceClearFinishStatus,
		.ClearErrorStatus = deviceClearErrorStatus,
			
			
		.UpdateUseTime = deviceUpdateUseTime,	
		.UpdateUseStatus = deviceUpdateUseStatus,
		.UpdateFinishStatus = deviceUpdateFinishStatus,
		.UpdateErrorStatus = deviceUpdateErrorStatus,
		
		.SubUseTime = deviceSubUseTime,
		.PortInit = devicePortInit,
	},
	//充电端口16
{
		//地址
		.useTimeFlashAddr = PORT16_TIME_ADDR,                          
		.useStatusFlashAddr = PORT16_STATUS_ADDR, 
		.finishFlagFlashAddr = PORT16_FINISH_ADDR,
		.errorFlagFlashAddr = PORT16_ERROR_ADDR,
		//状态变量
		.portNumber = 16,
		.useTime = 0.0,	
		.useStatus = 0,
		.finishFlag = 0,
		.errorFlag = 0,
		//IO
		.controlPin = GPIO_PIN_8,								 
		.controlPinGPIOX = GPIOD,
		.displayLedPin = GPIO_PIN_8,
		.displayLedPinGPIOX = GPIOC,
		.adcPin = GPIO_PIN_13,									 
		.adcPinGPIOX = GPIOE,
		.ADCX = ADC3,
		.adcChannel = 3,
		//回调函数
		.SetUseTime = deviceSetUseTime,
		.SetUseStatus = deviceSetUseStatus,
		.SetFinishStatus = deviceSetFinishStatus,
		.SetErrorStatus = deviceSetErrorStatus,
		
			
		.GetUseTime = deviceGetUseTime,		
		.GetAdcValue = deviceGetAdcValue,
		.GetUseStatus = deviceGetUseStatus,
		.GetErrorStatus = deviceGetErrorStatus,
		.GetFinishStatus = deviceGetFinishStatus,
			
		.ClearUseTime = deviceClearUseTime,
		.ClearUseStatus = deviceClearUseStatus,
		.ClearFinishStatus = deviceClearFinishStatus,
		.ClearErrorStatus = deviceClearErrorStatus,
			
			
		.UpdateUseTime = deviceUpdateUseTime,	
		.UpdateUseStatus = deviceUpdateUseStatus,
		.UpdateFinishStatus = deviceUpdateFinishStatus,
		.UpdateErrorStatus = deviceUpdateErrorStatus,
		
		.SubUseTime = deviceSubUseTime,
		.PortInit = devicePortInit,
	},
	//充电端口17
{
		//地址
		.useTimeFlashAddr = PORT17_TIME_ADDR,                          
		.useStatusFlashAddr = PORT17_STATUS_ADDR, 
		.finishFlagFlashAddr = PORT17_FINISH_ADDR,
		.errorFlagFlashAddr = PORT17_ERROR_ADDR,
		//状态变量
		.portNumber = 17,
		.useTime = 0.0,	
		.useStatus = 0,
		.finishFlag = 0,
		.errorFlag = 0,
		//IO
		.controlPin = GPIO_PIN_9,								 
		.controlPinGPIOX = GPIOD,
		.displayLedPin = GPIO_PIN_9,
		.displayLedPinGPIOX = GPIOC,
		.adcPin = GPIO_PIN_14,									 
		.adcPinGPIOX = GPIOE,
		.ADCX = ADC3,
		.adcChannel = 1,
		//回调函数
		.SetUseTime = deviceSetUseTime,
		.SetUseStatus = deviceSetUseStatus,
		.SetFinishStatus = deviceSetFinishStatus,
		.SetErrorStatus = deviceSetErrorStatus,
		
			
		.GetUseTime = deviceGetUseTime,		
		.GetAdcValue = deviceGetAdcValue,
		.GetUseStatus = deviceGetUseStatus,
		.GetErrorStatus = deviceGetErrorStatus,
		.GetFinishStatus = deviceGetFinishStatus,
			
		.ClearUseTime = deviceClearUseTime,
		.ClearUseStatus = deviceClearUseStatus,
		.ClearFinishStatus = deviceClearFinishStatus,
		.ClearErrorStatus = deviceClearErrorStatus,
			
			
		.UpdateUseTime = deviceUpdateUseTime,	
		.UpdateUseStatus = deviceUpdateUseStatus,
		.UpdateFinishStatus = deviceUpdateFinishStatus,
		.UpdateErrorStatus = deviceUpdateErrorStatus,
		
		.SubUseTime = deviceSubUseTime,
		.PortInit = devicePortInit,
	},
	//充电端口18
{
		//地址
		.useTimeFlashAddr = PORT18_TIME_ADDR,                          
		.useStatusFlashAddr = PORT18_STATUS_ADDR, 
		.finishFlagFlashAddr = PORT18_FINISH_ADDR,
		.errorFlagFlashAddr = PORT18_ERROR_ADDR,
		//状态变量
		.portNumber = 18,
		.useTime = 0.0,	
		.useStatus = 0,
		.finishFlag = 0,
		.errorFlag = 0,
		//IO
		.controlPin = GPIO_PIN_10,								 
		.controlPinGPIOX = GPIOD,
		.displayLedPin = GPIO_PIN_8,
		.displayLedPinGPIOX = GPIOA,
		.adcPin = GPIO_PIN_15,									 
		.adcPinGPIOX = GPIOE,
		.ADCX = ADC3,
		.adcChannel = 2,
		//回调函数
		.SetUseTime = deviceSetUseTime,
		.SetUseStatus = deviceSetUseStatus,
		.SetFinishStatus = deviceSetFinishStatus,
		.SetErrorStatus = deviceSetErrorStatus,
		
			
		.GetUseTime = deviceGetUseTime,		
		.GetAdcValue = deviceGetAdcValue,
		.GetUseStatus = deviceGetUseStatus,
		.GetErrorStatus = deviceGetErrorStatus,
		.GetFinishStatus = deviceGetFinishStatus,
			
		.ClearUseTime = deviceClearUseTime,
		.ClearUseStatus = deviceClearUseStatus,
		.ClearFinishStatus = deviceClearFinishStatus,
		.ClearErrorStatus = deviceClearErrorStatus,
			
			
		.UpdateUseTime = deviceUpdateUseTime,	
		.UpdateUseStatus = deviceUpdateUseStatus,
		.UpdateFinishStatus = deviceUpdateFinishStatus,
		.UpdateErrorStatus = deviceUpdateErrorStatus,
		
		.SubUseTime = deviceSubUseTime,
		.PortInit = devicePortInit,
	},
	//充电端口19
{
		//地址
		.useTimeFlashAddr = PORT19_TIME_ADDR,                          
		.useStatusFlashAddr = PORT19_STATUS_ADDR, 
		.finishFlagFlashAddr = PORT19_FINISH_ADDR,
		.errorFlagFlashAddr = PORT19_ERROR_ADDR,
		//状态变量
		.portNumber = 19,
		.useTime = 0.0,	
		.useStatus = 0,
		.finishFlag = 0,
		.errorFlag = 0,
		//IO
		.controlPin = GPIO_PIN_11,								 
		.controlPinGPIOX = GPIOD,
		.displayLedPin = GPIO_PIN_9,
		.displayLedPinGPIOX = GPIOA,
		.adcPin = GPIO_PIN_12,									 
		.adcPinGPIOX = GPIOB,
		.ADCX = ADC3,
		.adcChannel = 3,
		//回调函数
		.SetUseTime = deviceSetUseTime,
		.SetUseStatus = deviceSetUseStatus,
		.SetFinishStatus = deviceSetFinishStatus,
		.SetErrorStatus = deviceSetErrorStatus,
		
			
		.GetUseTime = deviceGetUseTime,		
		.GetAdcValue = deviceGetAdcValue,
		.GetUseStatus = deviceGetUseStatus,
		.GetErrorStatus = deviceGetErrorStatus,
		.GetFinishStatus = deviceGetFinishStatus,
			
		.ClearUseTime = deviceClearUseTime,
		.ClearUseStatus = deviceClearUseStatus,
		.ClearFinishStatus = deviceClearFinishStatus,
		.ClearErrorStatus = deviceClearErrorStatus,
			
			
		.UpdateUseTime = deviceUpdateUseTime,	
		.UpdateUseStatus = deviceUpdateUseStatus,
		.UpdateFinishStatus = deviceUpdateFinishStatus,
		.UpdateErrorStatus = deviceUpdateErrorStatus,
		
		.SubUseTime = deviceSubUseTime,
		.PortInit = devicePortInit,
	},
	//充电端口20
{
		//地址
		.useTimeFlashAddr = PORT20_TIME_ADDR,                          
		.useStatusFlashAddr = PORT20_STATUS_ADDR, 
		.finishFlagFlashAddr = PORT20_FINISH_ADDR,
		.errorFlagFlashAddr = PORT20_ERROR_ADDR,
		//状态变量
		.portNumber = 20,
		.useTime = 0.0,	
		.useStatus = 0,
		.finishFlag = 0,
		.errorFlag = 0,
		//IO
		.controlPin = GPIO_PIN_12,								 
		.controlPinGPIOX = GPIOD,
		.displayLedPin = GPIO_PIN_10,
		.displayLedPinGPIOX = GPIOA,
		.adcPin = GPIO_PIN_13,									 
		.adcPinGPIOX = GPIOB,
		.ADCX = ADC3,
		.adcChannel = 5,
		//回调函数
		.SetUseTime = deviceSetUseTime,
		.SetUseStatus = deviceSetUseStatus,
		.SetFinishStatus = deviceSetFinishStatus,
		.SetErrorStatus = deviceSetErrorStatus,
		
			
		.GetUseTime = deviceGetUseTime,		
		.GetAdcValue = deviceGetAdcValue,
		.GetUseStatus = deviceGetUseStatus,
		.GetErrorStatus = deviceGetErrorStatus,
		.GetFinishStatus = deviceGetFinishStatus,
			
		.ClearUseTime = deviceClearUseTime,
		.ClearUseStatus = deviceClearUseStatus,
		.ClearFinishStatus = deviceClearFinishStatus,
		.ClearErrorStatus = deviceClearErrorStatus,
			
			
		.UpdateUseTime = deviceUpdateUseTime,	
		.UpdateUseStatus = deviceUpdateUseStatus,
		.UpdateFinishStatus = deviceUpdateFinishStatus,
		.UpdateErrorStatus = deviceUpdateErrorStatus,
		
		.SubUseTime = deviceSubUseTime,
		.PortInit = devicePortInit,
	},
};

void deviceSetUseStatus(struct DxPort *port)          //设置IO充电       充电  状态设置并写入flash
{
	char * Msg = pvPortMalloc(40);
	memset(Msg, 0, 40);
	port->useStatus = 1;
	strcpy(Msg, "port");
	strcat(Msg, "1");
	strcat(Msg, "status");
	STMFLASH_Write(port->useStatusFlashAddr, (u32*)Msg, 4);
	STMFLASH_Read(port->useStatusFlashAddr, (u32*)Msg, 4);
	printf("Set UseStatus %s\r\n",Msg);
	HAL_GPIO_WritePin(port->controlPinGPIOX, port->controlPin, CHARGEPOWERON);           //打开继电器
	HAL_GPIO_WritePin(port->displayLedPinGPIOX, port->displayLedPin, CHARGELEDON);       //打开充电灯
	vPortFree(Msg);
}
void deviceSetFinishStatus(struct DxPort *port, u8 status)       //设置IO断电       完成  状态设置并写入flash
{
	char * Msg = pvPortMalloc(40);
	memset(Msg, 0, 40);
	port->finishFlag = status;
	strcpy(Msg, "port");
	if (status == 1)
	{
		strcat(Msg, "1");
	}
	else if(status == 2)
	{
		strcat(Msg, "2");
	}
	else if(status == 3)
	{
		strcat(Msg, "3");
	}
	else if(status == 4)
	{
		strcat(Msg, "4");
	}
	else
	{
		strcat(Msg, "0");
	}
	strcat(Msg, "status");
	STMFLASH_Write(port->finishFlagFlashAddr, (u32*)Msg, 4);
	STMFLASH_Read(port->finishFlagFlashAddr, (u32*)Msg, 4);
	printf("Set FinishStatus %s\r\n",Msg);
	HAL_GPIO_WritePin(port->controlPinGPIOX, port->controlPin, CHARGEPOWEROFF);            //关闭继电器
	HAL_GPIO_WritePin(port->displayLedPinGPIOX, port->displayLedPin, CHARGELEDOFF);        //关闭充电灯
	vPortFree(Msg);
}
void deviceSetErrorStatus(struct DxPort *port)          //设置端口损坏  状态设置并写入flash
{
	char * Msg = pvPortMalloc(40);
	memset(Msg, 0, 40);
	port->errorFlag = 1;		
	strcpy(Msg, "port");
	strcat(Msg, "1");
	strcat(Msg, "status");
	STMFLASH_Write(port->errorFlagFlashAddr, (u32*)Msg, 4);
	vPortFree(Msg);
	HAL_GPIO_WritePin(port->controlPinGPIOX, port->controlPin, CHARGEPOWEROFF);            //关闭继电器
	HAL_GPIO_WritePin(port->displayLedPinGPIOX, port->displayLedPin, CHARGELEDON);        //打开充电灯
}
void deviceSetUseTime(struct DxPort *port, float time)           //充电  时间   写入flash
{
	port->useTime = time;
	printf("Set use %f time \r\n", time);
	char * Msg = pvPortMalloc(60);
	memset(Msg, 0, 60);
	char *stat = pvPortMalloc(20);
	memset(stat, 0, 20);
	sprintf(stat, "%f", time);
	strcpy(Msg, "port");
	strcat(Msg, stat);
	strcat(Msg, "ed");
	printf("Write UseTime %s\r\n", Msg);
	STMFLASH_Write(port->useTimeFlashAddr, (u32*)Msg, 8);
	memset(Msg, 0, 60);
	STMFLASH_Read(port->useTimeFlashAddr, (u32*)Msg, 8);
	printf("Read  UseTime %s\r\n",Msg);
	vPortFree(Msg);
	vPortFree(stat);
}

int   deviceGetUseStatus(struct DxPort *port)
{
	return port->useStatus;
}  
int   deviceGetErrorStatus(struct DxPort *port)
{
	return port->errorFlag;
}  
float  deviceGetUseTime(struct DxPort *port)
{
	return port->useTime;
}
int  deviceGetAdcValue(struct DxPort *port)
{
	ADC_ChannelConfTypeDef ADC1_ChanConf;
    
	ADC1_ChanConf.Channel = port->adcChannel;                                        //通道
	ADC1_ChanConf.Rank = 1;                                           //第一个序列，序列1
	ADC1_ChanConf.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;              //采样时间
	HAL_ADC_ConfigChannel(&(port->adcHandler), &ADC1_ChanConf);             //通道配置
	
	HAL_ADC_Start(&(port->adcHandler));                                      //开启ADC
	
	HAL_ADC_PollForConversion(&(port->adcHandler), 20);                     //轮训转换
 
	return (int)HAL_ADC_GetValue(&(port->adcHandler));     	        //返回最近一次ADC1规则组的转换结果
}
int   deviceGetFinishStatus(struct DxPort *port)
{
	return port->finishFlag;
}  


void deviceClearUseStatus(struct DxPort *port)        
{
	char * Msg = pvPortMalloc(40);
	memset(Msg, 0, 40);
	port->useStatus = 0;
	strcpy(Msg, "port");
	strcat(Msg, "0");
	strcat(Msg, "status");
	STMFLASH_Write(port->useStatusFlashAddr, (u32*)Msg, 10);
	vPortFree(Msg);
	HAL_GPIO_WritePin(port->controlPinGPIOX, port->controlPin, CHARGEPOWEROFF);            //关闭继电器
    HAL_GPIO_WritePin(port->displayLedPinGPIOX, port->displayLedPin, CHARGELEDOFF);        //关闭充电灯
}
void deviceClearErrorStatus(struct DxPort *port)        
{
	char * Msg = pvPortMalloc(40);
	memset(Msg, 0, 40);
	port->errorFlag = 0;
	strcpy(Msg, "port");
	strcat(Msg, "0");
	strcat(Msg, "status");
	STMFLASH_Write(port->errorFlagFlashAddr, (u32*)Msg, 10);
	vPortFree(Msg);
	HAL_GPIO_WritePin(port->controlPinGPIOX, port->controlPin, CHARGEPOWEROFF);             //关闭继电器
  HAL_GPIO_WritePin(port->displayLedPinGPIOX, port->displayLedPin, CHARGELEDOFF);         //关闭充电灯
}
void deviceClearFinishStatus(struct DxPort *port)     
{
	char * Msg = pvPortMalloc(40);
	memset(Msg, 0, 40);
	port->finishFlag = 0;
	strcpy(Msg, "port");
	strcat(Msg, "0");
	strcat(Msg, "status");
	STMFLASH_Write(port->finishFlagFlashAddr, (u32*)Msg, 10);	
	vPortFree(Msg);
	HAL_GPIO_WritePin(port->controlPinGPIOX, port->controlPin, CHARGEPOWEROFF);             //关闭继电器
	HAL_GPIO_WritePin(port->displayLedPinGPIOX, port->displayLedPin, CHARGELEDOFF);         //关闭充电灯
}
void deviceClearUseTime(struct DxPort *port)    
{
	char * Msg = pvPortMalloc(40);
	memset(Msg, 0, 40);
	port->useTime = 0;
	strcpy(Msg, "port");
	strcat(Msg, "00000");
	strcat(Msg, "status");
	STMFLASH_Write(port->useTimeFlashAddr, (u32*)Msg, 10);
	vPortFree(Msg);
}


void   deviceUpdateUseStatus(struct DxPort *port)        //开机从Flash更新端口  使用  状态参数，不对端口IO操作
{
	char * s = NULL;
	char * Msg = pvPortMalloc(40);
	memset(Msg, 0, 40);
	
	char * head =  "port";
	
	char * end = "ed";
	
	STMFLASH_Read(port->useStatusFlashAddr, (u32*)Msg, 4);
	printf("UpdateUseStatus %s\r\n", Msg);
	
	if (NULL != strstr((const char *)Msg, (const char *)end))
	{
		if (NULL != strstr((const char *)Msg, (const char *)head))
		{
			s = strstr((const char *)Msg, (const char *)head);
			s = s + strlen(head);
			if (*s == '1') 
			{
				port->useStatus = 1;
				HAL_GPIO_WritePin(port->controlPinGPIOX, port->controlPin, CHARGEPOWERON);            //打开继电器
		    	HAL_GPIO_WritePin(port->displayLedPinGPIOX, port->displayLedPin, CHARGELEDON);        //打开充电灯
			}
			else if (*s == '0') 
			{
				port->useStatus = 0;
				HAL_GPIO_WritePin(port->controlPinGPIOX, port->controlPin, CHARGEPOWEROFF);              //关闭继电器
				HAL_GPIO_WritePin(port->displayLedPinGPIOX, port->displayLedPin, CHARGELEDOFF);          //关闭充电灯
			}
			else
			{
				port->useStatus = -1;
				HAL_GPIO_WritePin(port->controlPinGPIOX, port->controlPin, CHARGEPOWEROFF);              //关闭继电器
			  HAL_GPIO_WritePin(port->displayLedPinGPIOX, port->displayLedPin, CHARGELEDOFF);          //关闭充电灯
			}
		}
	}
	vPortFree(Msg);
	return ;
}  
void   deviceUpdateErrorStatus(struct DxPort *port)        //开机从Flash更新端口  使用  状态参数，不对端口IO操作
{
	char * s = NULL;

	
	char * head = "port";
	
	char * end = "status";
	
	char * Msg = pvPortMalloc(40);
	memset(Msg, 0, 40);
	STMFLASH_Read(port->errorFlagFlashAddr, (u32*)Msg, 8);
	printf("ErrorStatus %s\r\n", Msg);
		if(NULL != strstr((const char *)Msg, (const char *)end))
	{
		if (NULL != strstr((const char *)Msg, (const char *)head))
		{
			s = strstr((const char *)Msg, (const char *)head);
			s = s + strlen(head);
			if (*s == '1') 
			{
				*s = 1;
				port->errorFlag = 1;
				HAL_GPIO_WritePin(port->controlPinGPIOX, port->controlPin, CHARGEPOWEROFF);              //关闭继电器
		    	HAL_GPIO_WritePin(port->displayLedPinGPIOX, port->displayLedPin, CHARGELEDON);           //打开充电灯
			}
			else if (*s == '0') 
			{
				*s = 0;
				port->errorFlag = 0;
			}
			else
			{
				*s = -1;
				port->errorFlag = -1;
			}
		}
	}
	vPortFree(Msg);
}  
void   deviceUpdateFinishStatus(struct DxPort *port)     //开机从Flash更新端口  完成  状态参数，不对端口IO操作
{
	char * s = NULL;

	
	char * head = "port";
	
	char * end ="status";
	
	char * Msg = pvPortMalloc(40);
	memset(Msg, 0, 40);
	STMFLASH_Read(port->finishFlagFlashAddr, (u32*)Msg, 8);
	printf("FinishStatus %s\r\n", Msg);

	
	if(NULL != strstr((const char *)Msg, (const char *)end))
	{
		if (NULL != strstr((const char *)Msg, (const char *)head))
		{
			s = strstr((const char *)Msg, (const char *)head);
			s = s + strlen(head);
			if (*s == '1') 
			{
				port->finishFlag = 1;
			}
			else if (*s == '0') 
			{
				port->finishFlag = 0;
			}
			else
			{
				port->finishFlag = -1;
			}
		}
	}
	vPortFree(Msg);
}  
void  deviceUpdateUseTime(struct DxPort *port)
{
	char * s = NULL;
	char * d = NULL;
	char * head = "port";
	char * end = "status";
	char * Msg = pvPortMalloc(40);
	memset(Msg, 0, 40);
	STMFLASH_Read(port->useTimeFlashAddr, (u32*)Msg, 8);
	printf("UseTime %s\r\n", Msg);

		if(NULL != strstr((const char *)Msg, (const char *)end))
	   {
		   if (NULL != strstr((const char *)Msg, (const char *)head))
		{
			s = strstr((const char *)Msg, (const char *)head);
			d = strstr((const char *)Msg, (const char *)end);
			s = s + strlen(head);
            float tim = 0.0;
 			u8 tmp[20] = { 0 };
 			for (int i = 0; i < (d-s); i++)
 			{
 				tmp[i] = *s;
 				s++;
 			}
			tim = atof((const char*)tmp);
//			for (int i = 0; i < 5; i++)
//			{
//				if ((tmp[i] >= '0')&&(tmp[i] <= '9'))
//				{
//					tmp[i] -= '0';
//				}
//				else
//				{
//					tmp[i] = 0;
//				}
//			}
//			tim = tmp[0] * 10000 + tmp[1] * 1000 + tmp[2] * 100 + tmp[3] * 10 + tmp[4];
			port->useTime = tim;
			printf("update time %f\r\n", tim);
			vPortFree(Msg);
			return ;
		}
	}
	port->useTime = 0;
	vPortFree(Msg);
}


void deviceSubUseTime(struct DxPort *port)
{

	
	
}
void devicePortInit(struct DxPort *port)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//LED　　IO
	{
		if (port->displayLedPinGPIOX == GPIOA)
		{
			__GPIOA_CLK_ENABLE();
		}
		else if (port->displayLedPinGPIOX == GPIOB)
		{
			__GPIOB_CLK_ENABLE();
		}
		else if (port->displayLedPinGPIOX == GPIOC)
		{
			__GPIOC_CLK_ENABLE();
		}
		else if (port->displayLedPinGPIOX == GPIOD)
		{
			__GPIOD_CLK_ENABLE();
		}
		else if (port->displayLedPinGPIOX == GPIOE)
		{
			__GPIOE_CLK_ENABLE();
		}
		else if (port->displayLedPinGPIOX == GPIOF)
		{
			__GPIOF_CLK_ENABLE();
		}
		GPIO_InitStructure.Pin = port->displayLedPin;
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStructure.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(port->displayLedPinGPIOX, &GPIO_InitStructure);
	}
	
	//control　　IO
	{
		if (port->controlPinGPIOX == GPIOA)
		{
			__GPIOA_CLK_ENABLE();
		}
		else if (port->controlPinGPIOX == GPIOB)
		{
			__GPIOB_CLK_ENABLE();
		}
		else if (port->controlPinGPIOX == GPIOC)
		{
			__GPIOC_CLK_ENABLE();
		}
		else if (port->controlPinGPIOX == GPIOD)
		{
			__GPIOD_CLK_ENABLE();
		}
		else if (port->controlPinGPIOX == GPIOE)
		{
			__GPIOE_CLK_ENABLE();
		}
		else if (port->controlPinGPIOX == GPIOF)
		{
			__GPIOF_CLK_ENABLE();
		}
		GPIO_InitStructure.Pin = port->controlPin;
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStructure.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(port->controlPinGPIOX, &GPIO_InitStructure);
	}
	//adc　　IO
	{
		GPIO_InitTypeDef GPIO_Initure;
			
		GPIO_Initure.Pin = port->adcPin;                  
		GPIO_Initure.Mode = GPIO_MODE_ANALOG;          
		GPIO_Initure.Pull = GPIO_NOPULL;              
		HAL_GPIO_Init(port->adcPinGPIOX, &GPIO_Initure);   
		if (port->adcPinGPIOX == GPIOA)
		{
			__GPIOA_CLK_ENABLE();
		}
		else if (port->adcPinGPIOX == GPIOB)
		{
			__GPIOB_CLK_ENABLE();
		}
		else if (port->adcPinGPIOX == GPIOC)
		{
			__GPIOC_CLK_ENABLE();     			 
		}
		else if (port->adcPinGPIOX == GPIOD)
		{
			__GPIOD_CLK_ENABLE();
		}
		else if (port->adcPinGPIOX == GPIOE)
		{
			__GPIOE_CLK_ENABLE();
		}
		else if (port->adcPinGPIOX == GPIOF)
		{
			__GPIOF_CLK_ENABLE();
		}
		if (port->ADCX == ADC1)
		{
			__HAL_RCC_ADC1_CLK_ENABLE();               
		}
		else if (port->ADCX == ADC2)
		{
			__HAL_RCC_ADC2_CLK_ENABLE();   
		}
		else if (port->ADCX == ADC3)
		{
			__HAL_RCC_ADC3_CLK_ENABLE();       			 
		}
		else if (port->ADCX == ADC3)
		{
			__HAL_RCC_ADC3_CLK_ENABLE();   
		}
		port->adcHandler.Instance = port->ADCX;
		port->adcHandler.Init.DataAlign = ADC_DATAALIGN_RIGHT;                         //右对齐
		port->adcHandler.Init.ScanConvMode = DISABLE;                                  //非扫描模式
		port->adcHandler.Init.ContinuousConvMode = DISABLE;                            //关闭连续转换
		port->adcHandler.Init.NbrOfConversion = 1;                                     //1个转换在规则序列中 也就是只转换规则序列1
		port->adcHandler.Init.DiscontinuousConvMode = DISABLE;                         //禁止不连续采样模式
		port->adcHandler.Init.NbrOfDiscConversion = 0;                                 //不连续采样通道数为0
		port->adcHandler.Init.ExternalTrigConv = ADC_SOFTWARE_START;   			           //软件触发
		HAL_ADC_Init(&(port->adcHandler));                                              //初始化
	}
}


void PortSetUseStatus(u8 portnumber)
{
	if (portnumber < 0 || portnumber >= 20)
	{
		printf("port number is invalid\n");
		while (1)
			;
	}
	Port[portnumber].SetUseStatus(&Port[portnumber]);
}
void PortSetFinishStatus(u8 portnumber, u8 status)
{
	if (portnumber < 0 || portnumber >= 20)
	{
		printf("port number is invalid\n");
		while (1)
			;
	}
	Port[portnumber].SetFinishStatus(&Port[portnumber], status);
}
void PortSetErrorStatus(u8 portnumber)
{
	if (portnumber < 0 || portnumber >= 20)
	{
		printf("port number is invalid\n");
		while (1)
			;
	}
	Port[portnumber].SetErrorStatus(&Port[portnumber]);
}
void PortSetUseTime(u8 portnumber, float time)
{
	if (portnumber < 0 || portnumber >= 20)
	{
		printf("port number is invalid\n");
		while (1)
			;
	}
	Port[portnumber].SetUseTime(&Port[portnumber], time);
}

void PortClearUseStatus(u8 portnumber)
{
	if (portnumber < 0 || portnumber >= 20)
	{
		printf("port number is invalid\n");
		while (1)
			;
	}
	Port[portnumber].ClearUseStatus(&Port[portnumber]);
}
void PortClearFinishStatus(u8 portnumber)
{
	if (portnumber < 0 || portnumber >= 20)
	{
		printf("port number is invalid\n");
		while (1)
			;
	}
	Port[portnumber].ClearFinishStatus(&Port[portnumber]);
}
void PortClearUseTime(u8 portnumber)
{
	if (portnumber < 0 || portnumber >= 20)
	{
		printf("port number is invalid\n");
		while (1)
			;
	}
	Port[portnumber].ClearUseTime(&Port[portnumber]);
}


int GetPortUseTime(u8 portnumber)
{
	if (portnumber < 0 || portnumber >= 20)
	{
		printf("port number is invalid\n");
		return -1;
	}
	return Port[portnumber].GetUseTime(&Port[portnumber]);
	
}
void PortUpdateUseTime(u8 portnumber)
{
	if (portnumber < 0 || portnumber >= 20)
	{
		printf("port number is invalid\n");
		//	return -1;
			while(1)
				;
	}
	Port[portnumber].UpdateUseTime(&Port[portnumber]);
	
}
void PortUpdateUseStatus(u8 portnumber)      //从Flash更新状态，操作IO
{
	if (portnumber < 0 || portnumber >= 20)
	{
		printf("port number is invalid\n");
		//	return -1;
			while(1)
				;
	}
	Port[portnumber].UpdateUseStatus(&Port[portnumber]);
}
void PortUpdateFinishStatus(u8 portnumber)      //从Flash更新状态 
{
	if (portnumber < 0 || portnumber >= 20)
	{
		printf("port number is invalid\n");
		//	return -1;
			while(1)
				;
	}
	Port[portnumber].UpdateFinishStatus(&Port[portnumber]);
}
void PortUpdateErrorStatus(u8 portnumber)      //从Flash更新状态，操作IO
{
	if (portnumber < 0 || portnumber >= 20)
	{
		printf("port number is invalid\n");
		//	return -1;
			while(1)
				;
	}
	Port[portnumber].UpdateErrorStatus(&Port[portnumber]);
}
int PortGetAdcValue(u8 portnumber)
{
	if (portnumber < 0 || portnumber >= 20)
	{
		while (1)
			printf("port number is invalid\n");
	}
	int a1 = 0, a2 = 0, a3 = 0, a4 = 0, a5 = 0, a6 = 0, a7 = 0, a8 = 0, a9 = 0, a10 = 0;
    a1=  Port[portnumber].GetAdcValue(&Port[portnumber]);
	a2 = Port[portnumber].GetAdcValue(&Port[portnumber]);
	a3 = Port[portnumber].GetAdcValue(&Port[portnumber]);
	a4 = Port[portnumber].GetAdcValue(&Port[portnumber]);
	a5 = Port[portnumber].GetAdcValue(&Port[portnumber]);
	a6 = Port[portnumber].GetAdcValue(&Port[portnumber]);
	a7 = Port[portnumber].GetAdcValue(&Port[portnumber]);
	a8 = Port[portnumber].GetAdcValue(&Port[portnumber]);
	a9 = Port[portnumber].GetAdcValue(&Port[portnumber]);
	a10 = Port[portnumber].GetAdcValue(&Port[portnumber]);
	return (a1+a2+a3+a4+a5+a6+a7+a8+a9+a10)/10;
}
int PortGetUseStatus(u8 portnumber)
{
	if (portnumber < 0 || portnumber >= 20)
	{
		printf("port number is invalid\n");
		while (1)
			;
	}
	return Port[portnumber].GetUseStatus(&Port[portnumber]);
}
float PortGetUseTime(u8 portnumber)
{
	if (portnumber < 0 || portnumber >= 20)
	{
		printf("port number is invalid\n");
		while (1)
			;
	}
	return Port[portnumber].GetUseTime(&Port[portnumber]);
}
int PortGetFinishStatus(u8 portnumber)
{
	if (portnumber < 0 || portnumber >= 20)
	{
		printf("port number is invalid\n");
		while (1)
			;
	}
	return Port[portnumber].GetFinishStatus(&Port[portnumber]);
}
int PortGetErrorStatus(u8 portnumber)
{
	if (portnumber < 0 || portnumber >= 20)
	{
		printf("port number is invalid\n");
		return 0;
	}
	return Port[portnumber].GetErrorStatus(&Port[portnumber]);
}

void PortConfig(u8 portnumber)
{
	if (portnumber < 0 || portnumber >= 20)
	{
		while (1)
			printf("port number is invalid\n");
	}
	Port[portnumber].PortInit(&Port[portnumber]);
	HAL_GPIO_WritePin(Port[portnumber].displayLedPinGPIOX, Port[portnumber].displayLedPin, CHARGELEDOFF);
	HAL_GPIO_WritePin(Port[portnumber].controlPinGPIOX, Port[portnumber].controlPin, CHARGEPOWERON);
}
void PortSubUseTime(u8 portnumber)
{
	
	static int stopCheck[20] = { 0 };

	if (PortGetUseTime(portnumber) < 1)              //费用不足  充电停止
		{
			PortChargeFinish(portnumber, MONEY_OVER);
			return ;
		}
	else if (PortGetAdcValue(portnumber) >= CURRENT_MIN + 10)  //充电正常，继续扣费
		{
			static u8 costCount = 0;
			costCount++;
			if (costCount >= 60)  //3min扣一次费用
			{
				costCount = 0;
				float p = GetPrice();
				printf("price is %d\r\n", (int)p);
				float cur =  0;//GetCurrentValue(PortGetAdcValue(portnumber));
				printf("currten is %f\r\n", cur);
	         	float cost = (p)*(cur *0.22)*(0.05);
				printf("cost %f\r\n", cost);
				Port[portnumber].useTime = Port[portnumber].useTime - cost;
				char m[20] = { 0};
				sprintf(m, "%f", Port[portnumber].useTime);
            	printf("money %s\r\n",m);
				return ;
			}
		}
//	else if (PortGetAdcValue(portnumber) <= CURRENT_MIN)    //充电电流减小
//		{
//			stopCheck[portnumber]++;
//			if (stopCheck[portnumber] >= 3)
//			{
//				stopCheck[portnumber] = 0;
//				PortChargeFinish(portnumber, CHARGE_OVER);      //连续3次减小，则充电完成
//			}
//		}
}

void StartCharge(u8 *buf)
{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
	int port = GetChargePortNumber(buf);
	long int money = GetChargeMoney(buf);
	printf("Port %d charges\r\n", port);
	PortSetUseStatus(port - 1);
	PortSetUseTime(port - 1, money);
	//ChargeStartCheckCallBack(2, port-1);
}

void StopCharge(u8 *buf)
{
	int port = GetStopPortNumber(buf);
	printf("Port %d stoped\r\n", port);
	PortClearUseStatus(port - 1);
//	ChargeStopCheckCallBack(2, port-1);
}
void PortSleep(u8 port)
{
	PortSetErrorStatus(port);
//	PortErrorCheckCallBack(1, port+1);
}

int GetChargePortNumber(u8 *buf)
{
#define  STARTPORTNUMBERSTAT "charges"
#define  STARTPORTNUMBEREND  "charged"	
	u8 portNumber = 0;
	char * tmp;
	if (NULL != strstr((const char *)buf, (const char *)STARTPORTNUMBEREND))
	{
		tmp = strstr((const char *)buf, (const char *)STARTPORTNUMBERSTAT);
		tmp += strlen(STARTPORTNUMBERSTAT);
		portNumber = ((*tmp) - '0') * 10 + (*(tmp + 1) - '0');
	}
	if ((portNumber <= 20)&&(portNumber > 0))
	{
		return portNumber;
	}
	else
	{
		return 0;
	}
}
int GetChargeMoney(u8 *buf)
{
#define  STARTPORTNUMBERSTAT "charges"
#define  STARTPORTNUMBEREND  "charged"	
	long int money = 0;
	char * tmp;
	if (NULL != strstr((const char *)buf, (const char *)STARTPORTNUMBEREND))
	{
	  tmp = strstr((const char *)buf, (const char *)STARTPORTNUMBERSTAT);
		tmp += strlen(STARTPORTNUMBERSTAT) + 2;
		money = ((*tmp) - '0') * 10000 + (*(tmp + 1) - '0') * 1000 + (*(tmp + 2) - '0') * 100 + (*(tmp + 3) - '0') * 10+(*(tmp + 4) - '0');
	}
	if ((money <= 100000)&&(money > 0))
	{
		return money;
	}
	else
	{
		return 0;
	}
}

int GetStopPortNumber(u8 *buf)
{
#define  STOPPORTNUMBERSTAT "stopport"
#define  STOPPORTNUMBEREND  "stopportd"	
	u8 portNumber = 0;
	char * tmp;
	if (NULL != strstr((const char *)buf, (const char *)STOPPORTNUMBEREND))
	{
		tmp = strstr((const char *)buf, (const char *)STOPPORTNUMBERSTAT);
		tmp += strlen(STOPPORTNUMBERSTAT);
		portNumber = ((*tmp) - '0') * 10 + (*(tmp + 1) - '0');
	}
	if ((portNumber <= 20)&&(portNumber > 0))
	{
		return portNumber;
	}
	else
	{
		return 0;
	}
}
int GetErrorPortNumber(u8 *buf)
{
#define  ERRORPORTNUMBERSTAT "porterrors"
#define  ERRORPORTNUMBEREND  "porterrord"	

	char * tmp;
	u8 portNumber = 0;
	if (NULL != strstr((const char *)buf, (const char *)ERRORPORTNUMBEREND))
	{
		tmp = strstr((const char *)buf, (const char *)ERRORPORTNUMBERSTAT);
		tmp += strlen(ERRORPORTNUMBERSTAT);
		portNumber = ((*tmp) - '0') * 10 + (*(tmp + 1) - '0');
	}
	if ((portNumber <= 20)&&(portNumber > 0))
	{
		return portNumber;
	}
	else
	{
		return 0;
	}
}
int CheckPortErrorCmd(u8 * buf)
{
	if (NULL != strstr((const char *)buf, (const char *)"porterrors"))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void PortChargeFinish(u8 portnumber, u8 status)
{
	printf("charge finish port %d \r\n", portnumber + 1);
//	FinishRepCheck(portnumber, status);
	PortClearUseStatus(portnumber);
	PortClearFinishStatus(portnumber);
	PortClearUseTime(portnumber);
}