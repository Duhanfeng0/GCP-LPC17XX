/**
  ******************************************************************************
  * @file    NixieTube.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.07
  * @brief   demo
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "NixieTube.h"
#include "TM1617.h"
#include "../../DataType/DataType.h"
#include "../../SysPeripheral/GPIO/GPIO_Man.h"


/*****************************************************************************
 * 数码管控制接口
 ****************************************************************************/

static uBit32 m_ulDataPinNO  = 0;
static uBit32 m_ulClockPinNO = 0;
static uBit32 m_ulStbPinNO   = 0;


/**
  * @brief  数码管DIO引脚设置
  * @param  bState 要设置的状态
  * @retval None
  */
static void SetDIO(bool bState)
{
    GPIO_SetOutputState(m_ulDataPinNO, bState);
    
}


/**
  * @brief  数码管CLK引脚设置
  * @param  bState 要设置的状态
  * @retval None
  */
static void SetCLK(bool bState)
{
    GPIO_SetOutputState(m_ulClockPinNO, bState);
    
}


/**
  * @brief  数码管STB引脚设置
  * @param  bState 要设置的状态
  * @retval None
  */
static void SetSTB(bool bState)
{
    GPIO_SetOutputState(m_ulStbPinNO, bState);
    
}


/**
  * @brief  TM1617控制引脚编号设置
  * @param  ulDataPinNO  数据引脚编号
  * @param  ulClockPinNO 时钟引脚编号
  * @param  ulStbPinNO   片选引脚编号
  * @retval None
  */
void NixieTube_SetCtrlPinNO(uBit32 ulDataPinNO, uBit32 ulClockPinNO, uBit32 ulStbPinNO)
{
    m_ulDataPinNO  = ulDataPinNO ;
    m_ulClockPinNO = ulClockPinNO;
    m_ulStbPinNO   = ulStbPinNO  ;
    
    TM1617_SetCtrlPin(SetDIO, SetCLK, SetSTB);
    
}


/**
  * @brief  数码管数据显示函数
  * @param  uHexData 要显示的数据
  * @retval 0-成功 非0-失败
  */
uBit32 NixieTube_DisNumber(uBit8 uHexData)
{
    
    return TM1617_DisNumber(uHexData);
}












