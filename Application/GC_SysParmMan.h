#ifndef __GC_SYS_PARM_MAN_H
#define __GC_SYS_PARM_MAN_H

#include "../DataType/DataType.h"

typedef struct 
{
    uBit32 ulCabNumber;     //当前柜号
    uBit32 ulPwrDownTime;   //门锁通电保持时间
    uBit32 ulPwrOnTime;     //门锁断电保持时间
    uBit32 ulCRC16;         //CRC值(为了数据对齐,用32位数据存储)
    
}GC_SYS_PARM;


#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * 系统参数管理相关接口
 ****************************************************************************/
    
/**
  * @brief  系统参数初始化
  * @param  None
  * @retval 0-成功  1-失败
  */
uBit8 GC_SysParmInit(void);


/**
  * @brief  系统参数存储
  * @param  None
  * @retval 0-成功  1-失败
  */
uBit8 GC_StoreSysParm(void);


/**
  * @brief  系统参数地址获取
  * @param  None
  * @retval GC_SYS_PARM* 系统参数地址
  */
GC_SYS_PARM *GC_GetSysParmAddr(void);


#ifdef __cplusplus
}
#endif


#endif /* __GC_SYS_PARM_MAN_H */
