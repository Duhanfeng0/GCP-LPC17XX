/**
  ******************************************************************************
  * @file    GCSysParmMan.c
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
#include "GC_SysParmMan.h"
#include "GC_LockCtrl.h"
#include "GC_CtrlIOTable.h"
#include "GC_HardwareDef.h"

#include "../DataType/DataType.h"
#include "../PeriDrivers/Eeprom/Eeprom.h"
      
#include <string.h>

extern unsigned short int GetCRC16(unsigned char *pBuf, unsigned long int iCount);

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/
#define GC_EEPROM_STORE_ADDR            (64)    //EEPROM中数据存储的地址
static GC_SYS_PARM  m_GCSysParm = {0};      //图书机系统控制参数


/*****************************************************************************
 * 系统参数管理相关接口
 ****************************************************************************/

/**
  * @brief  系统参数初始化
  * @param  None
  * @retval 0-成功  1-失败
  */
uBit8 GC_SysParmInit(void)
{
    uBit16 nCRC16 = 0;
    
    //从EEPROM中读取出存储的数据
    EEPROM_ReadBuff(GC_EEPROM_STORE_ADDR, (uBit8 *)&m_GCSysParm, sizeof(GC_SYS_PARM));
    
    //CRC校验
    nCRC16 = GetCRC16((uBit8 *)&m_GCSysParm, sizeof(GC_SYS_PARM) - sizeof(uBit32));
    
    //假如校验不正确,则将EEPROM中的数据设置为默认的配置
    if (nCRC16 != m_GCSysParm.ulCRC16)
    {
        //设置数据
        m_GCSysParm.ulCabNumber = 0;
        m_GCSysParm.ulPwrDownTime = GC_LOCK_DEF_PWR_DOWN_TIME;  //默认的门锁关闭(断电)保持时间
        m_GCSysParm.ulPwrOnTime = GC_LOCK_DEF_PWR_ON_TIME;      //默认的门锁打开(通电)保持时间
        
        //计算校验值
        m_GCSysParm.ulCRC16 = GetCRC16((uBit8 *)&m_GCSysParm, sizeof(GC_SYS_PARM) - sizeof(uBit32));
        
        //存储数据
        EEPROM_WriteBuff(GC_EEPROM_STORE_ADDR, (uBit8 *)&m_GCSysParm, sizeof(GC_SYS_PARM));
        
        return 1;
    }
    
    return 0;
}


/**
  * @brief  系统参数存储
  * @param  None
  * @retval 0-成功  1-失败
  */
uBit8 GC_StoreSysParm(void)
{
    GC_SYS_PARM TempSysParam = {0};
    
    //计算CRC校验值
    m_GCSysParm.ulCRC16 = GetCRC16((uBit8 *)&m_GCSysParm, sizeof(GC_SYS_PARM) - sizeof(uBit32));
    
    //存储进EEPROM
    EEPROM_WriteBuff(GC_EEPROM_STORE_ADDR, (uBit8 *)&m_GCSysParm, sizeof(GC_SYS_PARM));
    
    //回读EEPROM
    EEPROM_ReadBuff(GC_EEPROM_STORE_ADDR, (uBit8 *)&TempSysParam, sizeof(GC_SYS_PARM));
    
    int iCmpRet = memcmp(&m_GCSysParm, &TempSysParam, sizeof(m_GCSysParm));
    
    return (iCmpRet != 0);
}


/**
  * @brief  系统参数地址获取
  * @param  None
  * @retval GC_SYS_PARM* 系统参数地址
  */
GC_SYS_PARM *GC_GetSysParmAddr(void)
{
    
    return &m_GCSysParm;
}

