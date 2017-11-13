/**
  ******************************************************************************
  * @file    GCIOCtrl.c
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
#include "GC_HwCtrl.h"
#include "GC_CtrlIOTable.h"
#include "GC_SysParmMan.h"
#include "GC_LockCtrl.h"
#include "GC_HardwareDef.h"

#include "../DataType/DataType.h"
#include "../SysPeripheral/GPIO/GPIO_Man.h"
#include "../SysPeripheral/KEY/KEY.h"
#include "../SysPeripheral/SysTick/SysTimer.h"
#include "../SysPeripheral/SysCtrl/SysCtrl.h"
#include "../SysPeripheral/UART/UART.h"
#include "../SysPeripheral/EXTI/EXTI.h"
#include "../SysPeripheral/IRQ/IRQ_Man.h"

#include "../PeriDrivers/NixieTube/NixieTube.h"
#include "../PeriDrivers/Eeprom/Eeprom.h"


/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/
#define GC_LED_TOGGLE_TIME          (100)       //LED翻转时间(MS)
static SYS_TIME_DATA m_LedCtrlTimer  = {1};     //LED控定时器

#define GC_KEY_SCAN_INTERVAL        (50)        //按键扫描间隔(MS)
static SYS_TIME_DATA m_ScanTimer  = {1};        //扫描定时器


/**
  * @brief  IO初始化
  * @param  None
  * @retval None
  */
static void GC_IOConfig(void)
{
    //设置控制IO资源表
    GPIO_SetCtrlTable((GPIO_CTRL_TABLE *)&GC_CtrlIOTable);
    
    //配置IO输出使能脚
    GPIO_InitIOGroup(&GC_CtrlIOTable.pOutputGroup[GC_OUTPUT_IO_OE_N], 1, GPIO_MODE_OUPUT);  //模式配置
    GPIO_MAN_SetOutputPinLogicToggle(GC_OUTPUT_IO_OE_N, 1); //设置逻辑翻转(true为使能柜锁IO,false为禁止柜锁IO)
    GPIO_MAN_SetOutputPinState(GC_OUTPUT_IO_OE_N, 0);       //关闭柜锁IO输出,防止在初始化过程中通电开锁
    
    //初始化资源表内的IO
    GPIO_InitIOTable((GPIO_CTRL_TABLE *)&GC_CtrlIOTable);
    
    //关闭所有柜锁
    GC_CloseAllLock();
    
    //配置I2C引脚为开漏
    GPIO_InitIOGroup(&GC_CtrlIOTable.pOutputGroup[GC_OUTPUT_IO_I2C_SDA], 2, GPIO_MODE_OD);
    
    GPIO_MAN_SetOutputPinState(GC_OUTPUT_IO_OE_N, 1);   //使能IO输出
    
}


/*****************************************************************************
 * 硬件配置接口
 ****************************************************************************/

/**
  * @brief  系统硬件初始化
  * @param  None
  * @retval None
  */
void GC_HwInit(void)
{
    //初始化IO
    GC_IOConfig();
    
    //初始化串口
    UART_Init(0, 115200);
    
    //初始化蜂鸣器
    GPIO_MAN_SetOutputPinLogicToggle(GC_OUTPUT_IO_BUZZER, 1);//设置蜂鸣器逻辑翻转,false为关闭,true为开启
    GPIO_MAN_SetOutputPinState(GC_OUTPUT_IO_BUZZER, 0);      //关闭蜂鸣器
    
    //初始化按键
    uBit32 ulKeyPinGourp[1] = {GC_INPUT_IO_KEY1};
    KEY_SetScanPinGroup(ulKeyPinGourp, 1);
    
    //初始化EEPROM
    EEPROM_Init(GC_OUTPUT_IO_I2C_SDA, GC_OUTPUT_IO_I2C_SCL);
    
    //初始化数码管
    NixieTube_SetCtrlPinNO(GC_OUTPUT_IO_NIXIE_TUBE_DIO, GC_OUTPUT_IO_NIXIE_TUBE_CLK, GC_OUTPUT_IO_NIXIE_TUBE_STB);
    
}


/*****************************************************************************
 * LED显示线程接口
 ****************************************************************************/

/**
  * @brief  LED 流水灯循环
  * @param  None
  * @retval None
  */
void GC_MainWorkLedShow(void)
{
    if (SysTime_CheckExpiredState(&m_LedCtrlTimer))
    {
        SysTime_StartOneShot(&m_LedCtrlTimer, GC_LED_TOGGLE_TIME); //设置下一次执行的时间
        
        GPIO_ToggleOutputState(GC_OUTPUT_IO_LED);
    }

}


/*****************************************************************************
 * 按键处理线程接口
 ****************************************************************************/

/**
  * @brief  按键处理
  * @param  None
  * @retval None
  */
void GC_KeyProc(void)
{
    if (SysTime_CheckExpiredState(&m_ScanTimer))
    {
        SysTime_StartOneShot(&m_ScanTimer, GC_KEY_SCAN_INTERVAL);   //设置下一次执行的时间
        
        uBit32 ulCurTrg = KEY_Scan(NULL);
        
        if (ulCurTrg)
        {
            //获取系统控制参数
            GC_SYS_PARM *pSysParm = GC_GetSysParmAddr();
            
            //柜号自增
            pSysParm->ulCabNumber = (pSysParm->ulCabNumber + 1) & 0x0F;
            
            //存储参数
            GC_StoreSysParm();
            
            //数码管显示
            NixieTube_DisNumber(pSysParm->ulCabNumber);
            
            //系统复位
            SysCtrl_ResetSystemDelay(1000);
            
        }
    }
      
}


/*****************************************************************************
 * 入书检测相关线程接口
 ****************************************************************************/

static uBit32 m_ulInBookResult = 0;  //入书结果,1为入书成功

/**
  * @brief  入书检测器中断处理
  * @param  None
  * @retval None
  */
static void GC_InBookExtiHandler(void)
{
    m_ulInBookResult = 1;
    
}

/**
  * @brief  入书检测器初始化
  * @param  None
  * @retval None
  */
void GC_InBookDetectorInit(void)
{
    EXTI_Init(GC_INPUT_IO_INC0, EXTI_TRG_RISING);   //上升沿作为触发条件
    
}


/**
  * @brief  入书检测器使能
  * @param  ulIsEnable 使能标志
  * @retval None
  */
void GC_InBookDetectorEnable(uBit32 ulIsEnable)
{
    if (ulIsEnable)
    {
        m_ulInBookResult = 0;
        IRQ_SetTrgCallback(GC_InBookExtiHandler, GC_INBOOK_DETECTOR_EXTI_SOURCE);
    }
    else 
    {
        IRQ_ReleaseTrgCallback(GC_INBOOK_DETECTOR_EXTI_SOURCE);
    }
    
    
}


/**
  * @brief  入书结果获取
  * @param  None
  * @retval 0-未入书 1-入书成功
  */
uBit32 GC_GetInBookResult(void)
{
    
    return m_ulInBookResult;
}











