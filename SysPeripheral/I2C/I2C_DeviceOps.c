/**
  ******************************************************************************
  * @file    I2C_DeviceOps.c
  * @author  Duhanfeng
  * @version V1.0 
  * @date    2017.03.17
  * @brief   I2C框架:内核实现文件
  ******************************************************************************
  * @attention
  * 
  * 对底层操作函数再一层的封装,以实现基本的协议通信时序.其操作的对象是 
  * I2C_BUS_BASE_OPS_TYPE
  * 
  * 
  ******************************************************************************
  */


/***********************************<INCLUDES>**********************************/
#include "I2C_Interface.h"
#include "I2C_DataStructDef.h"


//多字节发送函数
//对 I2C_BUS_BASE_OPS_TYPE的应用,实现字节发送的时序
static uBit32 I2C_SendBuff(I2C_BUS_BASE_OPS_TYPE *pBaseOps, I2C_CTRL_PARM *pCtrlParm, 
                           uBit16 nRegAddr, const uBit8 *pBuff, uBit32 ulCount)
{
    I2C_BIT_OPS_TYPE *pBitOps = &pBaseOps->BitOps;
    uBit16 nDevAddr = pCtrlParm->nDevAddr;
    uBit16 nFlags = pCtrlParm->nFlags;
    
    while (1)
    {
        pBaseOps->pf_I2C_Start(pBitOps);
        
        if (!(nFlags & I2C_FLAG_NO_DEV_ADDR))
        {
            //发送设备地址
            if (nFlags & I2C_FLAG_DEV_ADDR_16BIT)
            {
                if (pBaseOps->pf_I2C_SendByte(pBitOps, (nDevAddr>>8) & 0xFF) == 0) break;
            }
            if (pBaseOps->pf_I2C_SendByte(pBitOps, (nDevAddr & 0xFF) | I2C_FLAG_WR) == 0) break;
        }
        
        //发送寄存器地址
        if (!(nFlags & I2C_FLAG_NO_REG_ADDR))
        {
            if (nFlags & I2C_FLAG_REG_ADDR_16BIT)
            {
                if (pBaseOps->pf_I2C_SendByte(pBitOps, (nRegAddr>>8) & 0xFF) == 0) break;
            }
            if (pBaseOps->pf_I2C_SendByte(pBitOps, nRegAddr& 0xFF) == 0) break;
        }
        
        for (int i = 0; i < ulCount; i++)
        {
            if (pBaseOps->pf_I2C_SendByte(pBitOps, pBuff[i]) == 0) break;
        }
        
        pBaseOps->pf_I2C_Stop(pBitOps);
        return ulCount;
    }
    
    pBaseOps->pf_I2C_Stop(pBitOps);
    return 0;
}


//多字节接收函数
//对 I2C_BUS_BASE_OPS_TYPE的应用,实现字节接收的时序
static uBit32 I2C_RecvBuff(I2C_BUS_BASE_OPS_TYPE *pBaseOps, I2C_CTRL_PARM *pCtrlParm, 
                                  uBit16 nRegAddr, uBit8 *pBuff, uBit32 ulCount)
{
    I2C_BIT_OPS_TYPE *pBitOps = &pBaseOps->BitOps;
    uBit16 nDevAddr = pCtrlParm->nDevAddr;
    uBit16 nFlags = pCtrlParm->nFlags;
    
    while (1)
    {
        pBaseOps->pf_I2C_Start(pBitOps);
        
        //发送设备地址
        if (nFlags & I2C_FLAG_DEV_ADDR_16BIT)
        {
            if (pBaseOps->pf_I2C_SendByte(pBitOps, (nDevAddr>>8) & 0xFF) == 0) break;
        }
        if (pBaseOps->pf_I2C_SendByte(pBitOps, (nDevAddr & 0xFF) | I2C_FLAG_WR) == 0) break;
        
        //发送寄存器地址
        if (nFlags & I2C_FLAG_REG_ADDR_16BIT)
        {
            if (pBaseOps->pf_I2C_SendByte(pBitOps, (nRegAddr>>8) & 0xFF) == 0) break;
        }
        if (pBaseOps->pf_I2C_SendByte(pBitOps, nRegAddr& 0xFF) == 0) break;
        
        pBaseOps->pf_I2C_Start(pBitOps);
        
        //发送设备地址
        if (nFlags & I2C_FLAG_DEV_ADDR_16BIT)
        {
            if (pBaseOps->pf_I2C_SendByte(pBitOps, (nDevAddr>>8) & 0xFF) == 0) break;
        }
        if (pBaseOps->pf_I2C_SendByte(pBitOps, (nDevAddr & 0xFF) | I2C_FLAG_RD) == 0) break;
        
        //接收数据
        for (int i = 0; i < ulCount; i++)
        {
            pBuff[i] = pBaseOps->pf_I2C_RecvByte(pBitOps);
            pBaseOps->pf_I2C_SendAckOrNack(pBitOps, ulCount - (i+1));  //最后1个字节给非应答信号
        }
        
        pBaseOps->pf_I2C_Stop(pBitOps);
        
        return ulCount;
    }
    
    pBaseOps->pf_I2C_Stop(pBitOps);
    return 0;
}


/*----------------------------------------------------------------------------
对内核函数的封装,增加其校验重发的功能
其操作的对象是 I2C_Core_SendBuff 和 I2C_Core_RecvBuff
*----------------------------------------------------------------------------*/

//主机发送函数
static uBit32 I2C_MST_SendBuff(I2C_BUS_BASE_OPS_TYPE *pBaseOps, I2C_CTRL_PARM *pCtrlParm, 
                               uBit16 nRegAddr, const uBit8 *pBuff, uBit32 ulCount)
{
    uBit16 nRetries = (pCtrlParm->nFlags & I2C_FLAG_IGNORE_NACK) ? 1 : ((pCtrlParm->nRetries==0) ? 1 : pCtrlParm->nRetries);
    uBit32 ulSendCount = 0;
    uBit32 ulRet = I2C_ERR_IO;
    
    for (int i = 0; i < nRetries; i++)
    {
        ulSendCount = I2C_SendBuff(pBaseOps, pCtrlParm, nRegAddr, pBuff, ulCount);
        
        if (ulSendCount == ulCount) 
        {
            ulRet = I2C_ERR_OK;
            break;
        }
    }
    
    if (pCtrlParm->nFlags & I2C_FLAG_IGNORE_NACK)
    {
        ulRet = I2C_ERR_OK;
    }
    
    return ulRet;
}


//主机接收函数
static uBit32 I2C_MST_RecvBuff(I2C_BUS_BASE_OPS_TYPE *pBaseOps, I2C_CTRL_PARM *pCtrlParm, 
                               uBit16 nRegAddr, uBit8 *pBuff, uBit32 ulCount)
{
    uBit16 nRetries = (pCtrlParm->nFlags & I2C_FLAG_IGNORE_NACK) ? 1 : ((pCtrlParm->nRetries==0) ? 1 : pCtrlParm->nRetries);
    uBit32 ulRecvCount = 0;
    uBit32 ulRet = I2C_ERR_IO;
    
    for (int i = 0; i < nRetries; i++)
    {
        ulRecvCount = I2C_RecvBuff(pBaseOps, pCtrlParm, nRegAddr, pBuff, ulCount);
        
        if (ulRecvCount == ulCount) 
        {
            ulRet = I2C_ERR_OK;
            break;
        }
    }
    
    if (pCtrlParm->nFlags & I2C_FLAG_IGNORE_NACK)
    {
        ulRet = I2C_ERR_OK;
    }
    
    return ulRet;
}


/**
  * @brief  I2C设备操作接口输出
  * @param  pDeviceOpsInterface 操作接口指针
  * @retval None
  */
void I2C_DeviceOpsInterfaceExport(I2C_BUS_DEVICE_OPS_TYPE *pDeviceOpsInterface)
{
    pDeviceOpsInterface->pf_I2C_MST_SendBuff = I2C_MST_SendBuff;
    pDeviceOpsInterface->pf_I2C_MST_RecvBuff = I2C_MST_RecvBuff;
    
}

