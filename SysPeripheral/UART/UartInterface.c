/**
  ******************************************************************************
  * @file    UART_DMA.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.09.14
  * @brief   UART3 DMA管理函数
  ******************************************************************************
  * @attention
  * 
  * 
  *  
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "./UartInterface.h"
#include "./UartDataStructDef.h"
#include "./UART.h"
#include "./ucom.h"
#include "../../DataType/DataType.h"
#include "../HAL/HAL_Uart.h"
#include <string.h>


#define UART_MAX_SNED_LEN       (64)    //最大发送数据的长度

typedef struct 
{
    uBit32 ulID;
    uBit32 ulDataLen;
    uBit8  uDataBuff[UART_MAX_SNED_LEN];
}UART_DATA_PACK;

static UART_DATA_PACK m_UartSendPack = {0};
static UART_DATA_PACK m_UartRecvPack = {0};
static uBit8 m_uRecvBuff[128] = {0};
static uBit8 m_uSendBuff[128] = {0};
static uBit8 m_uRecvPackValidFlag = 0;




/**
  * @brief  数据接收处理
  * @param  uUartNode 串口节点号
  * @retval None
  */
void UART_CMU_RecvHandler(uBit8 uUartNode)
{
    uBit32 ulPackLen = 0;
    
    //获取数据包
    if (UCOM_GetPack(m_uRecvBuff, &ulPackLen) == 0)
    {
        memcpy(&m_UartRecvPack, m_uRecvBuff, ulPackLen);
        m_uRecvPackValidFlag = 1;
    }
    
}

/**
  * @brief  数据包获取
  * @param  uUartNode 串口节点号
  * @param  pDataPack 数据包变量指针(出参)
  * @retval 0-成功  非0-失败
  */
uBit32 UART_CMU_GetPack(uBit8 uUartNode, uBit32 *pID,  uBit32* pRcvLen, uBit8** pRcvBuf)
{
    if (m_uRecvPackValidFlag)
    {
        m_uRecvPackValidFlag = 0;
        
        *pID = m_UartRecvPack.ulID;
        *pRcvLen = m_UartRecvPack.ulDataLen;
        *pRcvBuf = m_UartRecvPack.uDataBuff;
        
        return 0;
    }
    
    return 1;
}


#define FRAME_HAND              ("HCZN")
#define FRAME_HAND_SIZE         (4)
#define FRAME_LEN_SIZE          (2)
#define FRAME_CHECK_SIZE        (2)

#define FRAME_HAND_INDEX        (0)
#define FRAME_LAN_INDEX         (4)
#define FRAME_PACK_INDEX        (6)

/**
  * @brief  数据包发送
  * @param  uUartNode 串口节点号
  * @param  ulID ID
  * @param  pDataBuf 要发送的数据缓冲区
  * @param  ulDataBufLen 华灿红曲长度
  * @retval 0-成功  非0-失败
  */
uBit32 UART_CMU_SendPack(uBit8 uUartNode, uBit32 ulID, uBit8* pDataBuff, uBit32 ulBuffLen)
{
    uBit16 uFrameLen = 0;
    uBit16 nFrameCheckValue = 0;
    
    //清空缓冲区
    memset(&m_UartSendPack, 0, sizeof(m_UartSendPack));
    memset(&m_uSendBuff, 0, sizeof(m_uSendBuff));
    
    //填充数据区
    m_UartSendPack.ulID = ulID;
    m_UartSendPack.ulDataLen = ulBuffLen;
    memcpy(m_UartSendPack.uDataBuff, pDataBuff, ulBuffLen); //打包数据区
    
    //打包数据
    uFrameLen = FRAME_HAND_SIZE + FRAME_LEN_SIZE + sizeof(ulID) + sizeof(ulBuffLen) + ulBuffLen + FRAME_CHECK_SIZE; //计算帧长
    memcpy(&m_uSendBuff[FRAME_HAND_INDEX], FRAME_HAND, FRAME_HAND_SIZE);    //帧头
    memcpy(&m_uSendBuff[FRAME_LAN_INDEX], &uFrameLen, sizeof(uFrameLen));   //数据区长度
    memcpy(&m_uSendBuff[FRAME_PACK_INDEX], &m_UartSendPack, uFrameLen - (FRAME_HAND_SIZE + FRAME_LEN_SIZE + FRAME_CHECK_SIZE)); //数据区
    
    for (int i = 0; i < uFrameLen; i++)
    {
        nFrameCheckValue += m_uSendBuff[i];
    }
    
    memcpy(&m_uSendBuff[uFrameLen-FRAME_CHECK_SIZE], &nFrameCheckValue, FRAME_CHECK_SIZE);  //校验和
    
    //发送数据
    HAL_UART_DmaSendBuff(uUartNode, (uBit8 *)&m_uSendBuff, uFrameLen);
    
    return 0;
}


/**
  * @brief  单次能发送的最大数据长度获取
  * @param  uUartNode 串口节点号
  * @retval 数据长度
  */
uBit32 UART_CMU_GetMaxSendLen(uBit8 uUartNode)
{
    
    return UART_MAX_SNED_LEN;
}


/*****************************************************************************
 * 串口接口输出
 ****************************************************************************/

/**
  * @brief  串口接口输出
  * @param  uUartNode 串口节点号
  * @param  ulID ID
  * @retval None
  */
void UART_ExportInterface(UART_INTERFACE *pUartInterface)
{
    pUartInterface->pf_UART_Create          = UART_Init       ;
    pUartInterface->pf_UART_Open            = UART_Open         ;
    pUartInterface->pf_UART_Close           = UART_Close        ;
//    pUartInterface->pf_UART_RecvHandler     = UART_RecvHandler  ;
    pUartInterface->pf_UART_SendBuff        = UART_SendBuff     ;
    pUartInterface->pf_UART_RecvBuff        = UART_RecvBuff     ;
    
    pUartInterface->pf_UART_RecvHandler     = UART_CMU_RecvHandler  ;
    pUartInterface->pf_UART_GetPack         = UART_CMU_GetPack      ;
    pUartInterface->pf_UART_SendPack        = UART_CMU_SendPack     ;
    pUartInterface->pf_UART_GetMaxSendLen   = UART_CMU_GetMaxSendLen;

        
}

