/**
  ******************************************************************************
  * @file    Demo.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.07
  * @brief   demo
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "GC_ComMan.h"
#include "GC_HwCtrl.h"
#include "GC_LockCtrl.h"

#include "../DataType/DataType.h"
#include "../SysPeripheral/UART/UartInterface.h"

#include "../CMU/CMU_Interface.h"

#include <stdlib.h>

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/


//设置互斥型脉冲IO口产生单脉冲
uBit32 GPIO_SetMutexMonoPulse(uBit32 ulIONO)
{
    //打开所有
    if (ulIONO == 0xFFFFFFFF)
    {
        GC_OpenAllLock();
    }
    else 
    {
        GC_OpenLockByNO(ulIONO);
    }
    
    return 0;
}


//互斥型脉冲时序设置
uBit32 GPIO_SetMutexPulseTiming(uBit32 ulActiveTime, uBit32 ulPassiveTime)
{
    GC_SetLockOptTiming(ulActiveTime, ulPassiveTime);
    
    return 0;
}


//高速IO边沿触发使能
uBit32 GPIO_SetHSpdEdgeTrgState(uBit32 ulIONO, uBit8 uTrg, uBit32 ulIsEnable)
{
    GC_InBookDetectorEnable(ulIsEnable);
    
    return 0;
}


//获取高速IO边沿触发状态
uBit32 GPIO_GetHSpdEdgeTrgState(uBit32 ulIONO)
{
    
    return GC_GetInBookResult();
}



/*****************************************************************************
 * 通信管理相关控制接口
 ****************************************************************************/

/**
  * @brief  通信管理单元初始化
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 GC_CMUInit(void)
{
    //设置通信模块数据缓冲区地址
    uBit8 *pCmuBufAddr = (uBit8 *)malloc(3*1024);
    if (pCmuBufAddr == NULL) return 1;
    CMU_SetRSBufAddr((uBit32)pCmuBufAddr, 3*1024);
    
    //设置外部功能函数列表
    CMU_EXTERNAL_FUN_TEBLE CMUFunTable = {0};   //定义外部函数列表
    CMUFunTable.pf_GPIO_SetMutexMonoPulse = GPIO_SetMutexMonoPulse;
    CMUFunTable.pf_GPIO_SetMutexPulseTiming = GPIO_SetMutexPulseTiming;
    CMUFunTable.pf_GPIO_SetHSpdEdgeTrgState = GPIO_SetHSpdEdgeTrgState;
    CMUFunTable.pf_GPIO_GetHSpdEdgeTrgState = GPIO_GetHSpdEdgeTrgState;
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
  * @brief  通信处理
  * @param  None
  * @retval None
  */
void GC_ComHandler(void)
{
    //通信控制
    CMU_MainProc();
      
}