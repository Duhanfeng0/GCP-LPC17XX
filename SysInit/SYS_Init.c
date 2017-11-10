/**
  ******************************************************************************
  * @file    SYS_Init.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.08
  * @brief   System some model initialize
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "SYS_Init.h"
#include "../DataType/DataType.h"
#include "../CMU/CMU_Interface.h"
#include "../CMU/CMU_DataStructDef.h"

#include "../SysPeripheral/SysTick/SysTick.h"
#include "../SysPeripheral/SysTick/SysTimer.h"
#include "../SysPeripheral/SysCtrl/SysCtrl.h"
#include "../SysPeripheral/IRQ/IRQ_Man.h"
#include "../SysPeripheral/UART/UartInterface.h"
#include "../SysPeripheral/UART/UART.h"
#include "../SysPeripheral/GPIO/GPIO_Man.h"

#include <stdlib.h>


/**
  * @brief  系统外设初始化
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_InitPerl(void)
{
    //初始化中断接口
    IRQ_Init();
    
    //初始化系统滴答
    SysTick_Init();
    
    //初始化串口
    UART_Init(0, 115200);
    
    return 0;
}


/**
  * @brief  通信管理单元初始化
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_InitCMU(void)
{
    //设置通信模块数据缓冲区地址
    uBit8 *pCmuBufAddr = (uBit8 *)malloc(3*1024);
    if (pCmuBufAddr == NULL) return 1;
    CMU_SetRSBufAddr((uBit32)pCmuBufAddr, 3*1024);
    
    //设置外部功能函数列表
    CMU_EXTERNAL_FUN_TEBLE CMUFunTable = {0};   //定义外部函数列表
    CMU_SetExternFun(CMUFunTable);
    
    //初始化串口接口
    UART_INTERFACE UartInterface = {0};
    UART_ExportInterface(&UartInterface);
    CMU_SetUartInterface(0, UartInterface);
    
    //初始化CMU
    CMU_Init(COM_TYPE_UART);
    
    return 0;
}


/**
  * @brief  系统初始化
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_Init(void)
{
    //上电延时
    SysTime_SimpleDelay(60000);
    
    //初始化外设
    SYS_InitPerl();
    
    //初始化通信管理单元
    SYS_InitCMU();
    
    return 0;
}


/**
  * @brief  系统主任务处理
  * @param  None
  * @retval None
  */
void SYS_MainTaskHandler(void)
{
    //通信控制
    CMU_MainProc();
    
    //IO状态更新处理
    GPIO_MAN_UpdateProc();
    
    //系统复位管理
    SysCtrl_ResetHandler();
    
}

