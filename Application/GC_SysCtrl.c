/**
  ******************************************************************************
  * @file    GCSysCtrl.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.10.23
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
#include "GC_SysCtrl.h"
#include "GC_SysParmMan.h"

#include "../DataType/DataType.h"
#include "../SysPeripheral/SysCtrl/SysCtrl.h"

#include "../PeriDrivers/Eeprom/Eeprom.h"
#include "../PeriDrivers/NixieTube/NixieTube.h"

/**
  * @brief  系统相关接口初始化
  * @param  None
  * @retval None
  */
void GC_SysInit(void)
{
    //刷新数码管
    uBit16 nCabNum = GC_GetSysParmAddr()->ulCabNumber;
    NixieTube_DisNumber(nCabNum);
    
}





