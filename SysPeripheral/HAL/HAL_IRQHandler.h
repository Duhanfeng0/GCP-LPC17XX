#ifndef  __HAL_IRQ_HANDLER_H
#define  __HAL_IRQ_HANDLER_H

//DMA通道
#define LPC_DMA_CH_COUNT            (8)

//IO中断组
#define LPC_GPIOINT_PORT_COUNT      (2)
#define LPC_GPIOINT_PIN_COUNT       (32)

typedef struct 
{
    void (*pf_SysTick_Update        )(void);
    void (*pf_SysTick_Handler       )(void);
    void (*pf_WDT_IRQHandler        )(void);
    void (*pf_TIMER0_IRQHandler     )(void);
    void (*pf_TIMER1_IRQHandler     )(void);
    void (*pf_TIMER2_IRQHandler     )(void);
    void (*pf_TIMER3_IRQHandler     )(void);
    void (*pf_UART0_IRQHandler      )(void);
    void (*pf_UART1_IRQHandler      )(void);
    void (*pf_UART2_IRQHandler      )(void);
    void (*pf_UART3_IRQHandler      )(void);
    void (*pf_PWM1_IRQHandler       )(void);
    void (*pf_I2C0_IRQHandler       )(void);
    void (*pf_I2C1_IRQHandler       )(void);
    void (*pf_I2C2_IRQHandler       )(void);
    void (*pf_SPI_IRQHandler        )(void);
    void (*pf_SSP0_IRQHandler       )(void);
    void (*pf_SSP1_IRQHandler       )(void);
    void (*pf_PLL0_IRQHandler       )(void);
    void (*pf_RTC_IRQHandler        )(void);
    void (*pf_EINT0_IRQHandler      )(void);
    void (*pf_EINT1_IRQHandler      )(void);
    void (*pf_EINT2_IRQHandler      )(void);
    void (*pf_ADC_IRQHandler        )(void);
    void (*pf_BOD_IRQHandler        )(void);
    void (*pf_USB_IRQHandler        )(void);
    void (*pf_CAN_IRQHandler        )(void);
    void (*pf_I2S_IRQHandler        )(void);
    void (*pf_ENET_IRQHandler       )(void);
    void (*pf_RIT_IRQHandler        )(void);
    void (*pf_MCPWM_IRQHandler      )(void);
    void (*pf_QEI_IRQHandler        )(void);
    void (*pf_PLL1_IRQHandler       )(void);
    void (*pf_USBActivity_IRQHandler)(void);
    void (*pf_CANActivity_IRQHandler)(void);
    
    void (*pf_DMA_CHx_IRQHandler[LPC_DMA_CH_COUNT])(void);
    void (*pf_GPIO_IRQHandler[LPC_GPIOINT_PORT_COUNT][LPC_GPIOINT_PIN_COUNT])(void);
    
}LPC17XX_IRQ_INTERFACE;

extern LPC17XX_IRQ_INTERFACE  g_IRQInterface;

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


#ifdef __cplusplus
}
#endif

#endif /* __HAL_IRQ_HANDLER_H */
