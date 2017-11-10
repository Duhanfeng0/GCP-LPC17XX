#ifndef  __HAL_IRQ_HANDLER_H
#define  __HAL_IRQ_HANDLER_H

#include "chip.h"

typedef enum
{
    //系统滴答
    IRQ_TRG_SYSTICK_UPDATE = 0,
    IRQ_TRG_SYSTICK_OS,
    
    //串口
    IRQ_TRG_UART0,
    IRQ_TRG_UART1,
    IRQ_TRG_UART2,
    IRQ_TRG_UART3,

    //DMA通道
    IRQ_TRG_DMA_CH1,
    IRQ_TRG_DMA_CH2,
    IRQ_TRG_DMA_CH3,
    IRQ_TRG_DMA_CH4,
    IRQ_TRG_DMA_CH5,
    IRQ_TRG_DMA_CH6,
    IRQ_TRG_DMA_CH7,
    IRQ_TRG_DMA_CH8,
    
    //GPIO中断
    IRQ_TRG_GPIOINT0_00,
    IRQ_TRG_GPIOINT0_01,
    IRQ_TRG_GPIOINT0_02,
    IRQ_TRG_GPIOINT0_03,
    IRQ_TRG_GPIOINT0_04,
    IRQ_TRG_GPIOINT0_05,
    IRQ_TRG_GPIOINT0_06,
    IRQ_TRG_GPIOINT0_07,
    IRQ_TRG_GPIOINT0_08,
    IRQ_TRG_GPIOINT0_09,
    IRQ_TRG_GPIOINT0_10,
    IRQ_TRG_GPIOINT0_11,
    IRQ_TRG_GPIOINT0_12,
    IRQ_TRG_GPIOINT0_13,
    IRQ_TRG_GPIOINT0_14,
    IRQ_TRG_GPIOINT0_15,
    IRQ_TRG_GPIOINT0_16,
    IRQ_TRG_GPIOINT0_17,
    IRQ_TRG_GPIOINT0_18,
    IRQ_TRG_GPIOINT0_19,
    IRQ_TRG_GPIOINT0_20,
    IRQ_TRG_GPIOINT0_21,
    IRQ_TRG_GPIOINT0_22,
    IRQ_TRG_GPIOINT0_23,
    IRQ_TRG_GPIOINT0_24,
    IRQ_TRG_GPIOINT0_25,
    IRQ_TRG_GPIOINT0_26,
    IRQ_TRG_GPIOINT0_27,
    IRQ_TRG_GPIOINT0_28,
    IRQ_TRG_GPIOINT0_29,
    IRQ_TRG_GPIOINT0_30,
    IRQ_TRG_GPIOINT0_31,

    IRQ_TRG_GPIOINT2_00,
    IRQ_TRG_GPIOINT2_01,
    IRQ_TRG_GPIOINT2_02,
    IRQ_TRG_GPIOINT2_03,
    IRQ_TRG_GPIOINT2_04,
    IRQ_TRG_GPIOINT2_05,
    IRQ_TRG_GPIOINT2_06,
    IRQ_TRG_GPIOINT2_07,
    IRQ_TRG_GPIOINT2_08,
    IRQ_TRG_GPIOINT2_09,
    IRQ_TRG_GPIOINT2_10,
    IRQ_TRG_GPIOINT2_11,
    IRQ_TRG_GPIOINT2_12,
    IRQ_TRG_GPIOINT2_13,
    IRQ_TRG_GPIOINT2_14,
    IRQ_TRG_GPIOINT2_15,
    IRQ_TRG_GPIOINT2_16,
    IRQ_TRG_GPIOINT2_17,
    IRQ_TRG_GPIOINT2_18,
    IRQ_TRG_GPIOINT2_19,
    IRQ_TRG_GPIOINT2_20,
    IRQ_TRG_GPIOINT2_21,
    IRQ_TRG_GPIOINT2_22,
    IRQ_TRG_GPIOINT2_23,
    IRQ_TRG_GPIOINT2_24,
    IRQ_TRG_GPIOINT2_25,
    IRQ_TRG_GPIOINT2_26,
    IRQ_TRG_GPIOINT2_27,
    IRQ_TRG_GPIOINT2_28,
    IRQ_TRG_GPIOINT2_29,
    IRQ_TRG_GPIOINT2_30,
    IRQ_TRG_GPIOINT2_31,
    
}IRQ_TRIGGER_SOURCE;



#ifdef __cplusplus
extern "C" {
#endif


/**
  * @brief  空指针入口
  * @param  None
  * @retval None
  */
void HAL_IRQ_NullEntry(void);


/**
  * @brief  中断回调指针初始化
  * @param  None
  * @retval None
  */
void HAL_IRQ_Init(void);
      

/**
  * @brief  设置中断触发回调
  * @param  ptr 回调指针
  * @param  ulTrgSource 触发源
  * @retval None
  */
void HAL_IRQ_SetTrgCallback(void (*ptr)(void), uint32_t ulTrgSource);


/**
  * @brief  释放中断触发回调
  * @param  ulTrgSource 触发源
  * @retval None
  */
void HAL_IRQ_ReleaseTrgCallback(uint32_t ulTrgSource);


#ifdef __cplusplus
}
#endif

#endif /* __HAL_IRQ_HANDLER_H */
