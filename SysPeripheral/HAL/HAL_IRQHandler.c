/**
  ******************************************************************************
  * @file    HAL_IRQHandler.c
  * @author  Duhanfneg
  * @version V1.0 
  * @date    2017.08.08
  * @brief   LPC17xx IRQ Callback Funtion interface
  ******************************************************************************
  * @attention
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "chip.h"
#include "HAL_IRQHandler.h"


#ifdef CHIP_LPC175X_6X

/* On the LPC1769, the GPIO interrupts share the EINT3 vector. */
#define GPIO_IRQHandler             EINT3_IRQHandler/* GPIO interrupt IRQ function name */
#define GPIO_INTERRUPT_NVIC_NAME    EINT3_IRQn    /* GPIO interrupt NVIC interrupt name */

#elif defined(CHIP_LPC177X_8X) || defined(CHIP_LPC40XX)

#define GPIO_IRQHandler             GPIO_IRQHandler/* GPIO interrupt IRQ function name */
#define GPIO_INTERRUPT_NVIC_NAME    GPIO_IRQn    /* GPIO interrupt NVIC interrupt name */

#endif

LPC17XX_IRQ_INTERFACE  g_IRQInterface;  //全局中断回调指针接口


/**
  * @brief  空指针入口
  * @param  None
  * @retval None
  */
void HAL_IRQ_NullEntry(void){}


/**
  * @brief  中断回调指针初始化
  * @param  None
  * @retval None
  */
void HAL_IRQ_Init(void)
{
    g_IRQInterface.pf_SysTick_Update         = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_SysTick_Handler        = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_WDT_IRQHandler         = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_TIMER0_IRQHandler      = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_TIMER1_IRQHandler      = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_TIMER2_IRQHandler      = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_TIMER3_IRQHandler      = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_UART0_IRQHandler       = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_UART1_IRQHandler       = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_UART2_IRQHandler       = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_UART3_IRQHandler       = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_PWM1_IRQHandler        = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_I2C0_IRQHandler        = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_I2C1_IRQHandler        = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_I2C2_IRQHandler        = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_SPI_IRQHandler         = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_SSP0_IRQHandler        = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_SSP1_IRQHandler        = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_PLL0_IRQHandler        = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_RTC_IRQHandler         = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_EINT0_IRQHandler       = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_EINT1_IRQHandler       = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_EINT2_IRQHandler       = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_ADC_IRQHandler         = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_BOD_IRQHandler         = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_USB_IRQHandler         = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_CAN_IRQHandler         = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_I2S_IRQHandler         = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_ENET_IRQHandler        = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_RIT_IRQHandler         = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_MCPWM_IRQHandler       = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_QEI_IRQHandler         = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_PLL1_IRQHandler        = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_USBActivity_IRQHandler = HAL_IRQ_NullEntry;
    g_IRQInterface.pf_CANActivity_IRQHandler = HAL_IRQ_NullEntry;
    
    for (int i = 0; i < LPC_DMA_CH_COUNT; i++)
    {
        g_IRQInterface.pf_DMA_CHx_IRQHandler[i]  = HAL_IRQ_NullEntry;
    }
    
    for (int i = 0; i < LPC_GPIOINT_PORT_COUNT; i++)
    {
        for (int j = 0; j < LPC_GPIOINT_PIN_COUNT; j++)
        {
            g_IRQInterface.pf_GPIO_IRQHandler[i][j] = HAL_IRQ_NullEntry;
        }
    }
    
}



/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    g_IRQInterface.pf_SysTick_Update();
    g_IRQInterface.pf_SysTick_Handler();
    
}


/**
  * @brief  This function handles WDT Handler.
  * @param  None
  * @retval None
  */
void WDT_IRQHandler(void)
{
    g_IRQInterface.pf_WDT_IRQHandler();

}


/**
  * @brief  This function handles TIMER0 Handler.
  * @param  None
  * @retval None
  */
void TIMER0_IRQHandler(void)
{
    g_IRQInterface.pf_TIMER0_IRQHandler();

}


/**
  * @brief  This function handles TIMER1 Handler.
  * @param  None
  * @retval None
  */
void TIMER1_IRQHandler(void)
{
    g_IRQInterface.pf_TIMER1_IRQHandler();

}


/**
  * @brief  This function handles TIMER2 Handler.
  * @param  None
  * @retval None
  */
void TIMER2_IRQHandler(void)
{
    g_IRQInterface.pf_TIMER2_IRQHandler();
    
}


/**
  * @brief  This function handles TIMER3 Handler.
  * @param  None
  * @retval None
  */
void TIMER3_IRQHandler(void)
{
    g_IRQInterface.pf_TIMER3_IRQHandler();
    
}


/**
  * @brief  This function handles UART0 Handler.
  * @param  None
  * @retval None
  */
void UART0_IRQHandler(void)
{
    g_IRQInterface.pf_UART0_IRQHandler();

}


/**
  * @brief  This function handles UART1 Handler.
  * @param  None
  * @retval None
  */
void UART1_IRQHandler(void)
{
    g_IRQInterface.pf_UART1_IRQHandler();
    
}


/**
  * @brief  This function handles UART2 Handler.
  * @param  None
  * @retval None
  */
void UART2_IRQHandler(void)
{
    g_IRQInterface.pf_UART2_IRQHandler();

}


/**
  * @brief  This function handles UART3 Handler.
  * @param  None
  * @retval None
  */
void UART3_IRQHandler(void)
{
    g_IRQInterface.pf_UART3_IRQHandler();

}


/**
  * @brief  This function handles PWM1 Handler.
  * @param  None
  * @retval None
  */
void PWM1_IRQHandler(void)
{
    g_IRQInterface.pf_PWM1_IRQHandler();

}


/**
  * @brief  This function handles I2C0 Handler.
  * @param  None
  * @retval None
  */
void I2C0_IRQHandler(void)
{
    g_IRQInterface.pf_I2C0_IRQHandler();

}


/**
  * @brief  This function handles I2C1 Handler.
  * @param  None
  * @retval None
  */
void I2C1_IRQHandler(void)
{
    g_IRQInterface.pf_I2C1_IRQHandler();

}


/**
  * @brief  This function handles I2C2 Handler.
  * @param  None
  * @retval None
  */
void I2C2_IRQHandler(void)
{
    g_IRQInterface.pf_I2C2_IRQHandler();

}


/**
  * @brief  This function handles SPI Handler.
  * @param  None
  * @retval None
  */
void SPI_IRQHandler(void)
{
    g_IRQInterface.pf_SPI_IRQHandler();

}


/**
  * @brief  This function handles SSP0 Handler.
  * @param  None
  * @retval None
  */
void SSP0_IRQHandler(void)
{
    g_IRQInterface.pf_SSP0_IRQHandler();

}


/**
  * @brief  This function handles SSP1 Handler.
  * @param  None
  * @retval None
  */
void SSP1_IRQHandler(void)
{
    g_IRQInterface.pf_SSP1_IRQHandler();

}


/**
  * @brief  This function handles PLL0 Handler.
  * @param  None
  * @retval None
  */
void PLL0_IRQHandler(void)
{
    g_IRQInterface.pf_PLL0_IRQHandler();

}


/**
  * @brief  This function handles RTC Handler.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{
    g_IRQInterface.pf_RTC_IRQHandler();

}


/**
  * @brief  This function handles EINT0 Handler.
  * @param  None
  * @retval None
  */
void EINT0_IRQHandler(void)
{
    g_IRQInterface.pf_EINT0_IRQHandler();

}


/**
  * @brief  This function handles EINT1 Handler.
  * @param  None
  * @retval None
  */
void EINT1_IRQHandler(void)
{
    g_IRQInterface.pf_EINT1_IRQHandler();

}


/**
  * @brief  This function handles EINT2 Handler.
  * @param  None
  * @retval None
  */
void EINT2_IRQHandler(void)
{
    g_IRQInterface.pf_EINT2_IRQHandler();

}


/**
  * @brief  This function handles GPIO Handler.
  * @param  None
  * @retval None
  */
void GPIO_IRQHandler(void)
{
    LPC_GPIOINT_PORT_T GpioIntPortGroup[LPC_GPIOINT_PORT_COUNT] = {GPIOINT_PORT0, GPIOINT_PORT2};
    
    for (int iPortIndex = 0; iPortIndex < LPC_GPIOINT_PORT_COUNT; iPortIndex++)
    {
        if (Chip_GPIOINT_IsIntPending(LPC_GPIOINT, GpioIntPortGroup[iPortIndex]))
        {
            //获取中断置位状态
            uint32_t IntStatus = Chip_GPIOINT_GetStatusFalling(LPC_GPIOINT, GpioIntPortGroup[iPortIndex]) | 
                                 Chip_GPIOINT_GetStatusRising (LPC_GPIOINT, GpioIntPortGroup[iPortIndex]);
            
            //确认中断入口
            for (int iPinIndex = 0; iPinIndex < LPC_GPIOINT_PIN_COUNT; iPinIndex++)
            {
                if (IntStatus & (0x1<<iPinIndex))
                {
                    IntStatus &= ~(0x1<<iPinIndex);
                    Chip_GPIOINT_ClearIntStatus(LPC_GPIOINT, GpioIntPortGroup[iPortIndex], (0x1<<iPinIndex));    //清中断
                    
                    //执行中断回调
                    g_IRQInterface.pf_GPIO_IRQHandler[iPortIndex][iPinIndex]();
                    
                    if (IntStatus == 0)
                    {
                        break;
                    }
                }
            }
        }
    }
    
}


/**
  * @brief  This function handles ADC Handler.
  * @param  None
  * @retval None
  */
void ADC_IRQHandler(void)
{
    g_IRQInterface.pf_ADC_IRQHandler();

}


/**
  * @brief  This function handles BOD Handler.
  * @param  None
  * @retval None
  */
void BOD_IRQHandler(void)
{
    g_IRQInterface.pf_BOD_IRQHandler();

}


/**
  * @brief  This function handles USB Handler.
  * @param  None
  * @retval None
  */
void USB_IRQHandler(void)
{
    g_IRQInterface.pf_USB_IRQHandler();

}


/**
  * @brief  This function handles CAN Handler.
  * @param  None
  * @retval None
  */
void CAN_IRQHandler(void)
{
    g_IRQInterface.pf_CAN_IRQHandler();

}


/**
  * @brief  This function handles DMA Handler.
  * @param  None
  * @retval None
  */
void DMA_IRQHandler(void)
{
    for (int i = 0; i < LPC_DMA_CH_COUNT; i++)
    {
        //判断中断入口
        if (Chip_GPDMA_IntGetStatus(LPC_GPDMA, GPDMA_STAT_INTTC, i) == SET)
        {
            //清标志位
            Chip_GPDMA_ClearIntPending(LPC_GPDMA, GPDMA_STATCLR_INTTC, i);
            
            //执行回调
            g_IRQInterface.pf_DMA_CHx_IRQHandler[i]();
        }
    }
    
}


/**
  * @brief  This function handles I2S Handler.
  * @param  None
  * @retval None
  */
void I2S_IRQHandler(void)
{
    g_IRQInterface.pf_I2S_IRQHandler();

}


/**
  * @brief  This function handles ENET Handler.
  * @param  None
  * @retval None
  */
void ENET_IRQHandler(void)
{
    g_IRQInterface.pf_ENET_IRQHandler();

}


/**
  * @brief  This function handles RIT Handler.
  * @param  None
  * @retval None
  */
void RIT_IRQHandler(void)
{
    g_IRQInterface.pf_RIT_IRQHandler();

}


/**
  * @brief  This function handles MCPWM Handler.
  * @param  None
  * @retval None
  */
void MCPWM_IRQHandler(void)
{
    g_IRQInterface.pf_MCPWM_IRQHandler();

}


/**
  * @brief  This function handles QEI Handler.
  * @param  None
  * @retval None
  */
void QEI_IRQHandler(void)
{
    g_IRQInterface.pf_QEI_IRQHandler();

}


/**
  * @brief  This function handles PLL1 Handler.
  * @param  None
  * @retval None
  */
void PLL1_IRQHandler(void)
{
    g_IRQInterface.pf_PLL1_IRQHandler();

}


/**
  * @brief  This function handles USBActivity Handler.
  * @param  None
  * @retval None
  */
void USBActivity_IRQHandler(void)
{
    g_IRQInterface.pf_USBActivity_IRQHandler();

}


/**
  * @brief  This function handles CANActivity Handler.
  * @param  None
  * @retval None
  */
void CANActivity_IRQHandler(void)
{
    g_IRQInterface.pf_CANActivity_IRQHandler();

}

