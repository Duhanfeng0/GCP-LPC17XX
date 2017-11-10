#ifndef __GC_LOCK_CTRL_H
#define __GC_LOCK_CTRL_H

#include "../DataType/DataType.h"

#ifdef __cplusplus
extern "C" {
#endif


/*****************************************************************************
 * 门锁时序控制相关接口
 ****************************************************************************/

/**
  * @brief  门锁时序设置
  * @param  ulPwrOnTime   书柜门电磁锁通电保持时间
  * @param  ulPwrDownTime 书柜门电磁锁断电保持时间
  * @retval None
  */
void GC_SetLockOptTiming(uBit32 ulPwrOnTime, uBit32 ulPwrDownTime);


/*****************************************************************************
 * 门锁控制处理相关接口
 ****************************************************************************/

/**
  * @brief  所有的IO信号关闭
  * @param  None
  * @retval None
  */
void GC_CloseAllLock(void);


/**
  * @brief  控制指定门锁打开
  * @param  nRowNumber 行数
  * @param  nColNumber 列数
  * @retval None
  */
void GC_OpenLock(uBit16 nRowNumber, uBit16 nColNumber);


/**
  * @brief  控制所有的门锁打开
  * @param  None
  * @retval None
  */
void GC_OpenAllLock(void);

/**
  * @brief  门锁处理
  * @param  None
  * @retval None
  * @Note   本函数需要放在while循环里轮询
  */
void GC_LockHandler(void);

    
    
#ifdef __cplusplus
}
#endif


#endif /* __GC_LOCK_CTRL_H */
