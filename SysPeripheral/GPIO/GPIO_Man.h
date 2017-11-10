#ifndef  __GPIO_MAN_H
#define  __GPIO_MAN_H

#include "../../DataType/DataType.h"

//IO引脚数据定义
typedef struct
{
    uBit16 nPort;   //端口号
    uBit16 nPin;    //引脚号
    
}GPIO_DATA;

//IO资源控制表定义
typedef struct
{
    GPIO_DATA *pOutputGroup;    //输出IO组
    GPIO_DATA *pInputGroup;     //输入IO组
    uBit32 ulOutputGroupLen;    //输出IO数
    uBit32 ulInputGroupLen;     //输入IO数
    
}GPIO_CTRL_TABLE;

//GPIO配置模式
typedef enum
{
    GOIO_MODE_INPUT,
    GPIO_MODE_OUPUT,
    GPIO_MODE_OD   ,
    
}GPIO_CFG_MODE;


#ifdef __cplusplus
extern "C"
{
#endif


/*****************************************************************************
 * IO配置相关接口
 ****************************************************************************/

/**
  * @brief  IO资源配置表设置
  * @param  pTable IO资源配置表指针
  * @retval 0-成功  非0-失败
  */
uBit32 GPIO_SetCtrlTable(GPIO_CTRL_TABLE *pTable);


/**
  * @brief  IO组初始化
  * @param  pIOGroup IO组指针
  * @param  ulGroupLen 组长度
  * @param  CfgMode  要配置的IO模式
  * @retval 0-成功  非0-失败
  */
uBit32 GPIO_InitIOGroup(GPIO_DATA *pIOGroup, uBit32 ulGroupLen, GPIO_CFG_MODE CfgMode);


/**
  * @brief  配置表IO初始化
  * @param  pTable IO资源配置表指针
  * @retval 0-成功  非0-失败
  * @note   此函数会把输出组的IO设置成输出模式,输入组的IO设置为输入模式;若要配置
  *         成开漏模式,需在执行本函数后另行配置(调用GPIO_InitIOGroup);
  */
uBit32 GPIO_InitIOTable(GPIO_CTRL_TABLE *pTable);


/*****************************************************************************
 * 基本IO控制接口
 ****************************************************************************/

/**
  * @brief  IO输出状态设置
  * @param  ulOutputNO 输出IO编号,从0算起
  * @param  bState     要设置的状态
  * @retval None
  */
void GPIO_SetOutputState(uBit32 ulOutputNO, bool bState);


/**
  * @brief  IO输出状态翻转
  * @param  ulOutputNO 输出IO编号,从0算起
  * @retval None
  */
void GPIO_ToggleOutputState(uBit32 ulOutputNO);


/**
  * @brief  IO 输出端口状态获取
  * @param  ulOutputNO 输出IO编号,从0算起
  * @retval IO状态
  */
bool GPIO_GetOutputState(uBit32 ulOutputNO);


/**
  * @brief  IO 输入端口状态获取
  * @param  ulOutputNO 输出IO编号,从0算起
  * @retval IO状态
  */
bool GPIO_GetInputState(uBit32 ulInputNO);


/*****************************************************************************
 * 输入IO管理接口
 ****************************************************************************/

/**
  * @brief  输入引脚翻转状态设置
  * @param  uInputNO 输入IO编号(从0算起)
  * @param  bState (true-对应的IO进行状态翻转, false-不处理)
  * @retval None
  * @note   IO翻转: 翻转IO的电平逻辑,以true为低电平,false为高电平.在实际的应用中,
  *                 有些IO设备为高电平使能,有些设备为低电平使能,我们可以将其中一
  *                 类设备进行IO逻辑翻转,这样应用层就可以统一设定true为使能的电平,
  *                 false为禁止的电平,不需要关心实际上所控制的电平是什么
  */
void GPIO_MAN_SetInputPinLogicToggle(uBit32 ulIntputNO, bool bState);


/**
  * @brief  输入引脚状态获取
  * @param  uInputNO 输入IO编号(从0算起)
  * @retval 引脚状态(true为有效电平, false为无效电平)
  */
bool GPIO_MAN_GetInputPinState(uBit32 ulIntputNO);


/*****************************************************************************
 * 输出IO管理接口
 ****************************************************************************/

/**
  * @brief  输出引脚翻转状态设置
  * @param  uInputNO 输入IO编号(从0算起)
  * @param  bState 翻转状态(true-对应的IO进行状态翻转, false-不处理)
  * @retval None
  * @note   IO翻转: 翻转IO的电平逻辑,以true为低电平,false为高电平.在实际的应用中,
  *                 有些IO设备为高电平使能,有些设备为低电平使能,我们可以将其中一
  *                 类设备进行IO逻辑翻转,这样应用层就可以统一设定true为使能的电平,
  *                 false为禁止的电平,不需要关心实际上所控制的电平是什么
  */
void GPIO_MAN_SetOutputPinLogicToggle(uBit32 ulOutputNO, bool bState);


/**
  * @brief  输出引脚状态获取
  * @param  uOutputNO 输出IO编号(从0算起)
  * @retval 引脚状态(true为有效电平, false为无效电平)
  */
bool GPIO_MAN_GetOutputPinState(uBit32 ulIntputNO);


/**
  * @brief  输出引脚状态设置
  * @param  uOutputNO 输出IO编号(从0算起)
  * @param  uState    IO状态,true为有效电平,false是无效电平
  * @retval None
  */
void GPIO_MAN_SetOutputPinState(uBit32 uOutputNO, bool bState);


/*****************************************************************************
 * IO状态刷新线程
 ****************************************************************************/

/**
  * @brief  IO状态更新处理(放在while循环中执行)
  * @param  None
  * @retval None
  */
void GPIO_MAN_UpdateProc(void);
    
    

#ifdef __cplusplus
}
#endif

#endif /* __GPIO_MAN_H */
