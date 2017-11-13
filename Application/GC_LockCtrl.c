/**
  ******************************************************************************
  * @file    GCLockCtrl.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.10.20
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
#include "GC_LockCtrl.h"
#include "GC_CtrlIOTable.h"
#include "GC_HardwareDef.h"
#include "GC_SysParmMan.h"
#include "../DataType/DataType.h"
#include "../SysPeripheral/GPIO/GPIO_Man.h"
#include "../SysPeripheral/SysTick/SysTimer.h"


/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/
static uBit32 m_ulLockCtrlMask[GC_MAX_ROW_COUNT] = {0}; //门锁控制掩码
static SYS_TIME_DATA m_LockCtrlTimer  = {0};            //锁控定时器


/*****************************************************************************
 * 门锁硬件控制接口
 ****************************************************************************/

/**
  * @brief  行信号控制
  * @param  nNumber 行编号
  * @param  bState  IO状态
  * @retval None
  */
static void GC_SetLockRowIO(uBit16 nNumber, bool bState)
{
    GPIO_MAN_SetOutputPinState(GC_OUTPUT_IO_ROW_0 + nNumber, bState);
    
}


/**
  * @brief  列信号控制
  * @param  nNumber 列编号
  * @param  bState  IO状态
  * @retval None
  */
static void GC_SetLockColIO(uBit16 nNumber, bool bState)
{
    GPIO_MAN_SetOutputPinState(GC_OUTPUT_IO_COL_0 + nNumber, bState);
    
}


/**
  * @brief  行状态获取
  * @param  nNumber 行编号
  * @retval bool IO状态
  */
static bool GC_GetLockRowIOState(uBit16 nNumber)
{
    
    return GPIO_MAN_GetOutputPinState(GC_OUTPUT_IO_ROW_0 + nNumber);
}


/**
  * @brief  列状态获取
  * @param  nNumber 列编号
  * @retval bool IO状态
  */
static bool GC_GetLockColIOState(uBit16 nNumber)
{
    
    return GPIO_MAN_GetOutputPinState(GC_OUTPUT_IO_COL_0 + nNumber);
}


/**
  * @brief  IO信号是否全部关闭检测
  * @param  None
  * @retval true-全部IO都关闭  false-存在未关闭的IO
  */
static bool GC_CheckAllLockClose(void)
{
    
    for (int i = 0; i < GC_MAX_ROW_COUNT; i++)
    {
        if (GC_GetLockRowIOState(i) == GC_LOCK_ROW_OPEN_STATE)
        {
            return false;
        }
    }
    
    for (int i = 0; i < GC_MAX_COL_COUNT; i++)
    {
        if (GC_GetLockColIOState(i) == GC_LOCK_COL_OPEN_STATE)
        {
            return false;
        }
    }
    
    return true;
}


/**
  * @brief  门锁控制
  * @param  nRowNumber 行数
  * @param  nColNumber 列数
  * @param  bIsOpen true-打开对应的锁  false-关闭对应的锁
  * @retval None
  */
static void GC_CtrlLock(uBit16 nRowNumber, uBit16 nColNumber, bool bIsOpen)
{
    GC_SetLockRowIO(nRowNumber, bIsOpen ? GC_LOCK_ROW_OPEN_STATE : (!GC_LOCK_ROW_OPEN_STATE));
    GC_SetLockColIO(nColNumber, bIsOpen ? GC_LOCK_COL_OPEN_STATE : (!GC_LOCK_COL_OPEN_STATE));
    
}


/*****************************************************************************
 * 门锁时序控制相关接口
 ****************************************************************************/

/**
  * @brief  书柜门锁打开保持时间获取
  * @param  None
  * @retval ulOpenKeepTime  书柜门电磁锁通电保持时间
  */
static uBit32 GC_GetLockPowerOnTime(void)
{
    
    return GC_GetSysParmAddr()->ulPwrOnTime;
}


/**
  * @brief  书柜门锁关闭保持时间获取
  * @param  None
  * @retval ulCloseKeepTime 书柜门电磁锁断电保持时间
  */
static uBit32 GC_GetLockPowerDownTime(void)
{
    
    return GC_GetSysParmAddr()->ulPwrDownTime;
}


/**
  * @brief  门锁时序设置
  * @param  ulPwrOnTime   书柜门电磁锁通电保持时间
  * @param  ulPwrDownTime 书柜门电磁锁断电保持时间
  * @retval None
  */
void GC_SetLockOptTiming(uBit32 ulPwrOnTime, uBit32 ulPwrDownTime)
{
    GC_GetSysParmAddr()->ulPwrOnTime = ulPwrOnTime;
    GC_GetSysParmAddr()->ulPwrDownTime = ulPwrDownTime;
    GC_StoreSysParm();
    
}

/*****************************************************************************
 * 门锁控制处理相关接口
 ****************************************************************************/

/**
  * @brief  所有的IO信号关闭
  * @param  None
  * @retval None
  */
void GC_CloseAllLock(void)
{
    //关闭所有的行信号
    for (int i = 0; i < GC_MAX_ROW_COUNT; i++)
    {
        GC_SetLockRowIO(i, !GC_LOCK_ROW_OPEN_STATE);
    }
    
    //关闭所有的列信号
    for (int i = 0; i < GC_MAX_COL_COUNT; i++)
    {
        GC_SetLockColIO(i, !GC_LOCK_COL_OPEN_STATE);
    }
    
}


/**
  * @brief  控制指定门锁打开(行列方式索引)
  * @param  nRowNumber 行数
  * @param  nColNumber 列数
  * @retval None
  */
void GC_OpenLockByPosition(uBit16 nRowNumber, uBit16 nColNumber)
{
    //数据范围校验
    if ((nRowNumber >= GC_MAX_ROW_COUNT) || (nColNumber >= GC_MAX_COL_COUNT))
    {
        return;
    }
    
    m_ulLockCtrlMask[nRowNumber] |= 0x1 << nColNumber;
    SysTime_StartOneShot(&m_LockCtrlTimer, GC_GetLockPowerOnTime());
    
}


/**
  * @brief  控制指定门锁打开(编号方式索引)
  * @param  ulLockNO 编号
  * @retval None
  */
void GC_OpenLockByNO(uBit32 ulLockNO)
{
    
    GC_OpenLockByPosition(ulLockNO/GC_MAX_COL_COUNT, ulLockNO%GC_MAX_COL_COUNT);
}


/**
  * @brief  控制所有的门锁打开
  * @param  None
  * @retval None
  */
void GC_OpenAllLock(void)
{
    for (int i = 0; i < GC_MAX_ROW_COUNT; i++)
    {
        m_ulLockCtrlMask[i] = GC_COL_MASK;
    }
    
    SysTime_StartOneShot(&m_LockCtrlTimer, GC_GetLockPowerOnTime());
    
}


/**
  * @brief  门锁处理
  * @param  None
  * @retval None
  * @Note   本函数需要放在while循环里轮询
  */
void GC_LockHandler(void)
{
    if (SysTime_CheckExpiredState(&m_LockCtrlTimer))
    {
        //如果所有门锁的已经关闭,则搜寻一个有效的门并打开
        if (GC_CheckAllLockClose())   
        {
            //搜寻需要打开的门
            for (int i = 0; i < GC_MAX_ROW_COUNT; i++)
            {
                if (m_ulLockCtrlMask[i] == 0)
                {
                    continue;
                }
                
                for (int j = 0; j < GC_MAX_COL_COUNT; j++)
                {
                    //找到了需要打开的门
                    if (m_ulLockCtrlMask[i] & (0x1<<j))
                    {
                        m_ulLockCtrlMask[i] &= ~(0x1<<j);       //清除此位标志
                        GC_CtrlLock(i, j, true);                //打开此门
                        SysTime_StartOneShot(&m_LockCtrlTimer, GC_GetLockPowerOnTime()); //设置下一次执行IO控制的时间
                        return;
                    }
                }
            }
        }
        //如果存在至少一个门被打开的情况,则关闭所有的门
        else
        {
           GC_CloseAllLock(); 
           SysTime_StartOneShot(&m_LockCtrlTimer, GC_GetLockPowerDownTime()); //设置下一次执行IO控制的时间
        }
    }
    
}
