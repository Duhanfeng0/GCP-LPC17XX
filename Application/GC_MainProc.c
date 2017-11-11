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
#include "GC_ComMan.h"
#include "../DataType/DataType.h"


/**
  * @brief  格子柜相关资源初始化
  * @param  None
  * @retval None
  */
void GC_Init(void)
{
    //初始化硬件
    GC_HwInit();
    
    //初始化系统参数
    GC_SysParmInit();
    
    //初始化格子柜系统
    GC_SysInit();
    
    //初始化通信管理单元
    GC_CMUInit();
    
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
    
    //通信处理
    GC_ComHandler();
    
}

