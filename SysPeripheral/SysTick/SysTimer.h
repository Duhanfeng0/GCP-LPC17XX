#ifndef  __SYS_TIMER_H
#define  __SYS_TIMER_H

#include "../../DataType/DataType.h"

//系统定时器数据结构
typedef struct
{
    uBit32  ulEnable;     //使能标志
    uBit32  ulOneshot;    //单次计时标志
    uBit32  ulMsDelay;    //延时时间
    uBit32  ulMsStart;    //开始执行的时刻

}SYS_TIME_DATA;


#ifdef __cplusplus
extern "C"
{
#endif


/*****************************************************************************
 * 系统定时器实现
 ****************************************************************************/

/**
  * @brief  系统定时器启动
  * @param  pTimeData 系统定时器指针
  * @param  ulMsDelay  要延时的毫秒数
  * @retval None
  */
void SysTime_Start(SYS_TIME_DATA *pTimeData, uBit32 ulMsDelay);


/**
  * @brief  系统定时器启动(单次)
  * @param  pTimeData 系统定时器指针
  * @param  ulMsDelay  要延时的毫秒数
  * @retval None
  */
void SysTime_StartOneShot(SYS_TIME_DATA *pTimeData, uBit32 ulMsDelay);


/**
  * @brief  系统定时器取消
  * @param  pTimeData 系统定时器指针
  * @retval None
  */
void SysTime_Cancel(SYS_TIME_DATA *pTimeData);


/**
  * @brief  系统定时器超时状态检测
  * @param  pTimeData 系统定时器指针
  * @retval 0-未超时  1-超时
  */
uBit8 SysTime_CheckExpiredState(SYS_TIME_DATA *pTimeData);


/*****************************************************************************
 * 延时功能
 ****************************************************************************/

/**
  * @brief  简单的延时函数
  * @param  ulDelay 延时参数,无标准单位
  * @retval None
  * @note   在100MHz的系统中测试,入参100,延时大概是10us
  */
void SysTime_SimpleDelay(uBit32 ulDelay);


/**
  * @brief  毫秒级延时
  * @param  ulMsDelay 毫秒数 入参范围[1-0xFFFFFFFF](49.7天)
  * @retval None
  */
void SysTime_DelayMs(uBit32 ulMsDelay);


/*****************************************************************************
 * 计时功能
 ****************************************************************************/

/**
  * @brief  计时启动
  * @param  pTimeData 系统定时器指针
  * @retval None
  */
void SysTime_StartReckon(SYS_TIME_DATA *pTimeData);


/**
  * @brief  计时值获取
  * @param  pTimeData 系统定时器指针
  * @retval 计时数(毫秒)
  */
uBit32 SysTime_GetReckonValue(SYS_TIME_DATA *pTimeData);


#ifdef __cplusplus
}
#endif

#endif /* __SYS_TIMER_H */
