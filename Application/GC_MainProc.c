/**
  ******************************************************************************
  * @file    GCMachine.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.10.23
  * @brief   
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
   
   
/***********************************<INCLUDES>**********************************/
#include "GC_MainProc.h"
#include "GC_HwCtrl.h"
#include "GC_SysParmMan.h"
#include "GC_LockCtrl.h"
#include "GC_CtrlIOTable.h"
#include "GC_SysCtrl.h"
#include "GC_HardwareDef.h"

#include "../DataType/DataType.h"


/**
  * @brief  格子柜相关资源初始化
  * @param  None
  * @retval None
  */
void GC_Init(void)
{
    //硬件初始化
    GC_HwInit();
    
    //系统参数初始化
    GC_SysParmInit();
    
}



/**
  * @brief  格子柜主任务处理
  * @param  None
  * @retval None
  */
void GC_MainProc(void)
{
    //LED显示
    GC_MainWorkLedShow();
    
    //柜锁控制
    GC_LockHandler();
    
    //按键处理
    GC_KeyProc();
    
}

