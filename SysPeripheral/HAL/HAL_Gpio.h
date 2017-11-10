#ifndef  __HAL_GPIO_H
#define  __HAL_GPIO_H

#include "chip.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
  * @brief  输出IO状态设置
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @param  bState IO状态
  * @retval None
  */
void HAL_GPIO_SetOutputState(uint16_t nPort, uint16_t nPin, bool bState);


/**
  * @brief  IO状态获取
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @retval bool IO状态
  */
bool HAL_GPIO_GetIOState(uint16_t nPort, uint16_t nPin);


/**
  * @brief  开漏IO配置
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @retval None
  */
void HAL_GPIO_ConfigOD(uint16_t nPort, uint16_t nPin);


/**
  * @brief  输入IO配置
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @retval None
  */
void HAL_GPIO_ConfigInput(uint16_t nPort, uint16_t nPin);


/**
  * @brief  输出IO配置
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @retval None
  */
void HAL_GPIO_ConfigOutput(uint16_t nPort, uint16_t nPin);
    
    
#ifdef __cplusplus
}
#endif

#endif /* __HAL_GPIO_H */
