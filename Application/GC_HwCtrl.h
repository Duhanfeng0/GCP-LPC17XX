#ifndef  __GC_HW_CFG_H
#define  __GC_HW_CFG_H


#include "../DataType/DataType.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * 硬件配置接口
 ****************************************************************************/

/**
  * @brief  系统硬件初始化
  * @param  None
  * @retval None
  */
void GC_HwInit(void);


/*****************************************************************************
 * 主线程LED显示接口
 ****************************************************************************/

/**
  * @brief  LED 流水灯循环
  * @param  None
  * @retval None
  */
void GC_MainWorkLedShow(void);
    

/*****************************************************************************
 * 按键处理线程接口
 ****************************************************************************/

/**
  * @brief  按键处理
  * @param  None
  * @retval None
  */
void GC_KeyProc(void);


/*****************************************************************************
 * 入书检测相关线程接口
 ****************************************************************************/

/**
  * @brief  入书检测器初始化
  * @param  None
  * @retval None
  */
void GC_InBookDetectorInit(void);


/**
  * @brief  入书检测器使能
  * @param  ulIsEnable 使能标志
  * @retval None
  */
void GC_InBookDetectorEnable(uBit32 ulIsEnable);


/**
  * @brief  入书结果获取
  * @param  None
  * @retval 0-未入书 1-入书成功
  */
uBit32 GC_GetInBookResult(void);


#ifdef __cplusplus
}
#endif


#endif /* __GC_HW_CFG_H */
