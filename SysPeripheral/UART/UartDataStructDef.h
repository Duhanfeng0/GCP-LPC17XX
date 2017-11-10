#ifndef  __UART_DATA_STRUCT_DEF_H
#define  __UART_DATA_STRUCT_DEF_H

#include "../../DataType/DataType.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    /**
      * @brief  串口创建(初始化)
      * @param  uUartNode 串口节点号
      * @retval 0-成功  非0-失败
      */
    uBit32 (*pf_UART_Create)(uBit8 uUartNode, uBit32 ulBaudRate);
    
    /**
      * @brief  端口打开
      * @param  uUartNode 串口节点号
      * @retval 0-成功  非0-失败
      */
    uBit32 (*pf_UART_Open)(uBit8 uUartNode);
    
    /**
      * @brief  端口关闭
      * @param  uUartNode 串口节点号
      * @retval 0-成功  非0-失败
      */
    uBit32 (*pf_UART_Close)(uBit8 uUartNode);
    
    /**
      * @brief  数据接收处理(接收线程)
      * @param  uUartNode 串口节点号
      * @retval None
      * @Note   本函数是应用于非中断接收的情况,单独一个线程做数据接收
      */
    void (*pf_UART_RecvHandler)(uBit8 uUartNode);
    
    /**
      * @brief  串口数据发送(非阻塞)
      * @param  uUartNode 串口节点号
      * @param  pSBuff 数据缓冲区地址
      * @param  nSize 发送的数量
      * @retval None
      */
    uBit32 (*pf_UART_SendBuff)(uBit8 uUartNode, uBit8 *pSBuff, uBit32 ulSize);
    
    /**
      * @brief  串口接收(非阻塞)
      * @param  uUartNode 串口节点号
      * @param  pRBuff 要接收的缓冲区
      * @param  ulSize 要接收的数据长度
      * @retval uint32_t 实际上接收到的数据长度
      */
    uBit32 (*pf_UART_RecvBuff)(uBit8 uUartNode, uBit8 *pRBuff, uBit32 ulSize);

    /**
      * @brief  数据包获取
      * @param  uUartNode 串口节点号
      * @param  pDataPack 数据包变量指针(出参)
      * @retval 0-成功  非0-失败
      */
    uBit32 (*pf_UART_GetPack)(uBit8 uUartNode, uBit32 *pID,  uBit32* pRcvLen, uBit8** pRcvBuf);
    
    /**
      * @brief  数据包发送
      * @param  uUartNode 串口节点号
      * @param  ulID ID
      * @param  pDataBuf 要发送的数据缓冲区
      * @param  ulDataBufLen 华灿红曲长度
      * @retval 0-成功  非0-失败
      */
    uBit32 (*pf_UART_SendPack)(uBit8 uUartNode, uBit32 ulID, uBit8* pDataBuf, uBit32 ulDataBufLen);

    /**
      * @brief  单次能发送的最大数据长度获取
      * @param  uUartNode 串口节点号
      * @retval 数据长度
      */
    uBit32 (*pf_UART_GetMaxSendLen)(uBit8 uUartNode);
    
    
}UART_INTERFACE;

#ifdef __cplusplus
}
#endif


#endif /* __UART_DATA_STRUCT_DEF_H */
