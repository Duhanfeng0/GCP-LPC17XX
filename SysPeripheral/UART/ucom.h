#ifndef  __UCOM_H
#define  __UCOM_H

#include "../../DataType/DataType.h"

/**
  * @brief  数据解包函数
  * @param  pRcvBuf 接收数据缓冲区
  * @param  pRcvLen 接收到的数据(出参)
  * @retval 0 -- 成功  1 -- 失败
  */
uBit32 UCOM_GetPack(uBit8* pRcvBuf, uBit32* pRcvLen);


#endif
