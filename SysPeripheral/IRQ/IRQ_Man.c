/**
  ******************************************************************************
  * @file    IRQ_Man.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.07
  * @brief   Hardware IRQ manage
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "IRQ_Man.h"
#include "../HAL/HAL_IRQHandler.h"
#include <stdio.h>


/**
  * @brief  中断接口初始化
  * @param  None
  * @retval None
  * @note   本函数必须在所有硬件模块配置之前调用(重要)
  *         
  */
void IRQ_Init(void)
{
    HAL_IRQ_Init();
    
}


/**
  * @brief  设置中断触发回调
  * @param  ptr 回调指针
  * @param  TrgSource 触发源
  * @retval None
  */
void IRQ_SetTrgCallback(void (*ptr)(void), IRQ_TRIGGER_SOURCE TrgSource)
{
    if (ptr == NULL) 
    {
        return;
    }
    
    switch (TrgSource)
    {

    //系统滴答
    case IRQ_TRG_SYSTICK_UPDATE: g_IRQInterface.pf_SysTick_Update =  ptr; break;
    case IRQ_TRG_SYSTICK_OS:  g_IRQInterface.pf_SysTick_Handler =  ptr; break;
    
    //串口
    case IRQ_TRG_UART0: g_IRQInterface.pf_UART0_IRQHandler =  ptr; break;
    case IRQ_TRG_UART1: g_IRQInterface.pf_UART1_IRQHandler =  ptr; break;
    case IRQ_TRG_UART2: g_IRQInterface.pf_UART2_IRQHandler =  ptr; break;
    case IRQ_TRG_UART3: g_IRQInterface.pf_UART3_IRQHandler =  ptr; break;

    //DMA通道
    case IRQ_TRG_DMA_CH1: 
    case IRQ_TRG_DMA_CH2: 
    case IRQ_TRG_DMA_CH3: 
    case IRQ_TRG_DMA_CH4: 
    case IRQ_TRG_DMA_CH5: 
    case IRQ_TRG_DMA_CH6: 
    case IRQ_TRG_DMA_CH7: 
    case IRQ_TRG_DMA_CH8: g_IRQInterface.pf_DMA_CHx_IRQHandler[TrgSource - IRQ_TRG_DMA_CH1] =  ptr; break;
    
    //GPIO中断
    case IRQ_TRG_GPIOINT0_00:
    case IRQ_TRG_GPIOINT0_01:
    case IRQ_TRG_GPIOINT0_02:
    case IRQ_TRG_GPIOINT0_03:
    case IRQ_TRG_GPIOINT0_04:
    case IRQ_TRG_GPIOINT0_05:
    case IRQ_TRG_GPIOINT0_06:
    case IRQ_TRG_GPIOINT0_07:
    case IRQ_TRG_GPIOINT0_08:
    case IRQ_TRG_GPIOINT0_09:
    case IRQ_TRG_GPIOINT0_10:
    case IRQ_TRG_GPIOINT0_11:
    case IRQ_TRG_GPIOINT0_12:
    case IRQ_TRG_GPIOINT0_13:
    case IRQ_TRG_GPIOINT0_14:
    case IRQ_TRG_GPIOINT0_15:
    case IRQ_TRG_GPIOINT0_16:
    case IRQ_TRG_GPIOINT0_17:
    case IRQ_TRG_GPIOINT0_18:
    case IRQ_TRG_GPIOINT0_19:
    case IRQ_TRG_GPIOINT0_20:
    case IRQ_TRG_GPIOINT0_21:
    case IRQ_TRG_GPIOINT0_22:
    case IRQ_TRG_GPIOINT0_23:
    case IRQ_TRG_GPIOINT0_24:
    case IRQ_TRG_GPIOINT0_25:
    case IRQ_TRG_GPIOINT0_26:
    case IRQ_TRG_GPIOINT0_27:
    case IRQ_TRG_GPIOINT0_28:
    case IRQ_TRG_GPIOINT0_29:
    case IRQ_TRG_GPIOINT0_30:
    case IRQ_TRG_GPIOINT0_31:

    case IRQ_TRG_GPIOINT2_00:
    case IRQ_TRG_GPIOINT2_01:
    case IRQ_TRG_GPIOINT2_02:
    case IRQ_TRG_GPIOINT2_03:
    case IRQ_TRG_GPIOINT2_04:
    case IRQ_TRG_GPIOINT2_05:
    case IRQ_TRG_GPIOINT2_06:
    case IRQ_TRG_GPIOINT2_07:
    case IRQ_TRG_GPIOINT2_08:
    case IRQ_TRG_GPIOINT2_09:
    case IRQ_TRG_GPIOINT2_10:
    case IRQ_TRG_GPIOINT2_11:
    case IRQ_TRG_GPIOINT2_12:
    case IRQ_TRG_GPIOINT2_13:
    case IRQ_TRG_GPIOINT2_14:
    case IRQ_TRG_GPIOINT2_15:
    case IRQ_TRG_GPIOINT2_16:
    case IRQ_TRG_GPIOINT2_17:
    case IRQ_TRG_GPIOINT2_18:
    case IRQ_TRG_GPIOINT2_19:
    case IRQ_TRG_GPIOINT2_20:
    case IRQ_TRG_GPIOINT2_21:
    case IRQ_TRG_GPIOINT2_22:
    case IRQ_TRG_GPIOINT2_23:
    case IRQ_TRG_GPIOINT2_24:
    case IRQ_TRG_GPIOINT2_25:
    case IRQ_TRG_GPIOINT2_26:
    case IRQ_TRG_GPIOINT2_27:
    case IRQ_TRG_GPIOINT2_28:
    case IRQ_TRG_GPIOINT2_29:
    case IRQ_TRG_GPIOINT2_30:
    case IRQ_TRG_GPIOINT2_31: g_IRQInterface.pf_GPIO_IRQHandler[0][TrgSource - IRQ_TRG_GPIOINT0_00] = ptr; break;  //通过数组越界的方式去访问二维数组
    
    default: break;
    }
    
}


/**
  * @brief  释放中断触发回调
  * @param  TrgSource 触发源
  * @retval None
  */
void IRQ_ReleaseTrgCallback(IRQ_TRIGGER_SOURCE TrgSource)
{
    switch (TrgSource)
    {

    //系统滴答
    case IRQ_TRG_SYSTICK_UPDATE: g_IRQInterface.pf_SysTick_Update =  HAL_IRQ_NullEntry; break;
    case IRQ_TRG_SYSTICK_OS:  g_IRQInterface.pf_SysTick_Handler =  HAL_IRQ_NullEntry; break;
    
    //串口
    case IRQ_TRG_UART0: g_IRQInterface.pf_UART0_IRQHandler =  HAL_IRQ_NullEntry; break;
    case IRQ_TRG_UART1: g_IRQInterface.pf_UART1_IRQHandler =  HAL_IRQ_NullEntry; break;
    case IRQ_TRG_UART2: g_IRQInterface.pf_UART2_IRQHandler =  HAL_IRQ_NullEntry; break;
    case IRQ_TRG_UART3: g_IRQInterface.pf_UART3_IRQHandler =  HAL_IRQ_NullEntry; break;

    //DMA通道
    case IRQ_TRG_DMA_CH1: 
    case IRQ_TRG_DMA_CH2: 
    case IRQ_TRG_DMA_CH3: 
    case IRQ_TRG_DMA_CH4: 
    case IRQ_TRG_DMA_CH5: 
    case IRQ_TRG_DMA_CH6: 
    case IRQ_TRG_DMA_CH7: 
    case IRQ_TRG_DMA_CH8: g_IRQInterface.pf_DMA_CHx_IRQHandler[TrgSource - IRQ_TRG_DMA_CH1] =  HAL_IRQ_NullEntry; break;
    
    //GPIO中断
    case IRQ_TRG_GPIOINT0_00:
    case IRQ_TRG_GPIOINT0_01:
    case IRQ_TRG_GPIOINT0_02:
    case IRQ_TRG_GPIOINT0_03:
    case IRQ_TRG_GPIOINT0_04:
    case IRQ_TRG_GPIOINT0_05:
    case IRQ_TRG_GPIOINT0_06:
    case IRQ_TRG_GPIOINT0_07:
    case IRQ_TRG_GPIOINT0_08:
    case IRQ_TRG_GPIOINT0_09:
    case IRQ_TRG_GPIOINT0_10:
    case IRQ_TRG_GPIOINT0_11:
    case IRQ_TRG_GPIOINT0_12:
    case IRQ_TRG_GPIOINT0_13:
    case IRQ_TRG_GPIOINT0_14:
    case IRQ_TRG_GPIOINT0_15:
    case IRQ_TRG_GPIOINT0_16:
    case IRQ_TRG_GPIOINT0_17:
    case IRQ_TRG_GPIOINT0_18:
    case IRQ_TRG_GPIOINT0_19:
    case IRQ_TRG_GPIOINT0_20:
    case IRQ_TRG_GPIOINT0_21:
    case IRQ_TRG_GPIOINT0_22:
    case IRQ_TRG_GPIOINT0_23:
    case IRQ_TRG_GPIOINT0_24:
    case IRQ_TRG_GPIOINT0_25:
    case IRQ_TRG_GPIOINT0_26:
    case IRQ_TRG_GPIOINT0_27:
    case IRQ_TRG_GPIOINT0_28:
    case IRQ_TRG_GPIOINT0_29:
    case IRQ_TRG_GPIOINT0_30:
    case IRQ_TRG_GPIOINT0_31:

    case IRQ_TRG_GPIOINT2_00:
    case IRQ_TRG_GPIOINT2_01:
    case IRQ_TRG_GPIOINT2_02:
    case IRQ_TRG_GPIOINT2_03:
    case IRQ_TRG_GPIOINT2_04:
    case IRQ_TRG_GPIOINT2_05:
    case IRQ_TRG_GPIOINT2_06:
    case IRQ_TRG_GPIOINT2_07:
    case IRQ_TRG_GPIOINT2_08:
    case IRQ_TRG_GPIOINT2_09:
    case IRQ_TRG_GPIOINT2_10:
    case IRQ_TRG_GPIOINT2_11:
    case IRQ_TRG_GPIOINT2_12:
    case IRQ_TRG_GPIOINT2_13:
    case IRQ_TRG_GPIOINT2_14:
    case IRQ_TRG_GPIOINT2_15:
    case IRQ_TRG_GPIOINT2_16:
    case IRQ_TRG_GPIOINT2_17:
    case IRQ_TRG_GPIOINT2_18:
    case IRQ_TRG_GPIOINT2_19:
    case IRQ_TRG_GPIOINT2_20:
    case IRQ_TRG_GPIOINT2_21:
    case IRQ_TRG_GPIOINT2_22:
    case IRQ_TRG_GPIOINT2_23:
    case IRQ_TRG_GPIOINT2_24:
    case IRQ_TRG_GPIOINT2_25:
    case IRQ_TRG_GPIOINT2_26:
    case IRQ_TRG_GPIOINT2_27:
    case IRQ_TRG_GPIOINT2_28:
    case IRQ_TRG_GPIOINT2_29:
    case IRQ_TRG_GPIOINT2_30:
    case IRQ_TRG_GPIOINT2_31: g_IRQInterface.pf_GPIO_IRQHandler[0][TrgSource - IRQ_TRG_GPIOINT0_00] = HAL_IRQ_NullEntry; break;  //通过数组越界的方式去访问二维数组
    
    default: break;
    }
    
}

