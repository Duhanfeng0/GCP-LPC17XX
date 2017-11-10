#ifndef  __HAL_WATCHDOG_H
#define  __HAL_WATCHDOG_H

#include "chip.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
  * @brief  看门狗初始化
  * @param  ulTimeOutMs 超时时间
  * @retval None
  */
void HAL_WWDT_Init(uint32_t ulTimeOutMs);


/**
  * @brief  喂狗
  * @param  None
  * @retval None
  */
void WWDT_Feed(void);

    
#ifdef __cplusplus
}
#endif



#endif /* __HAL_WATCHDOG_H */
