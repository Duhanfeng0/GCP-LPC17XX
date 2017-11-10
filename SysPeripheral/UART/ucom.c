 /**
   ******************************************************************************
   * @file    ucom.h
   * @author  杜公子寒枫
   * @version V1.0 寄存器版本
   * @date    2017.08.08
   * @brief   串口指令解析函数
   ******************************************************************************
   * @attention
   * 
   * 
   * 
   ******************************************************************************
   */
   
   
/***********************************<INCLUDES>**********************************/
#include "ucom.h"
#include <stdlib.h>
#include <string.h>
#include "../../DataType/DataType.h"
#include "UART.h"


//帧数据长度定义(字节)
#define UCOM_HEAD_STR               "HCZN"  //帧头
#define UCOM_SIZE_HEAD              (4)     //帧头长度
#define UCOM_SIZE_LENSEG            (2)     //帧长度字段长度
#define UCOM_SIZE_CHECKNUM          (2)     //校验字长度

#define UCOM_PACK_MAX_LEN           (64)    //单次发送的最大总长度
//单次发送允许最大的发送数据区长度
#define UCOM_DATA_MAX_LENGTH        (UCOM_PACK_MAX_LEN-UCOM_SIZE_HEAD-UCOM_SIZE_LENSEG-UCOM_SIZE_CHECKNUM)

//状态机工作步骤
#define UCOM_GET_PACK_STEP_HEAD     (0)     //获取包头
#define UCOM_GET_PACK_STEP_LEN      (1)     //获取帧长度
#define UCOM_GET_PACK_STEP_DATA     (2)     //获取数据区
#define UCOM_GET_PACK_STEP_CHECKNUM (3)     //获取校验字


static uBit8    *m_pHead = UCOM_HEAD_STR;     //指向帧头
static uBit8     m_bUcomGetPackStep=UCOM_GET_PACK_STEP_HEAD;  //当前状态机的步数
static uBit16    m_nUcomReadPos=0;            //已经处理的数据位置 从接收缓冲区头开始计数
static uBit16    m_nUcomWritePos=0;           //已经接收到的数据位置 从接收缓冲区头开始计数
static uBit16    m_nUcomDataLen=0;            //数据区长度
static uBit8     m_pUcomRcvBuf[UCOM_PACK_MAX_LEN] = {0};      //接收数据缓冲区


uBit32 HW_UART_RecvBuff(uBit8 *pRBuff, uBit32 ulSize)
{
    
    return UART_RecvBuff(0, pRBuff, ulSize);
}


/**
  * @brief  数据解包函数
  * @param  pRcvBuf 接收数据缓冲区
  * @param  pRcvLen 接收到的数据(出参)
  * @retval 0 -- 成功  1 -- 失败
  */
uBit32 UCOM_GetPack(uBit8* pRcvBuf, uBit32* pRcvLen)
{
    int iRecvCount;
    uBit16  nRestCount;
    uBit16  nRcvCheckSum = 0;                    //校验字(数据包中的校验字)
    uBit16  nCheckSum=0;                         //计数得到的校验字

    while(1)
    {
        switch(m_bUcomGetPackStep)
        {
            case UCOM_GET_PACK_STEP_HEAD:      //获取文件头(找头)
                {
                    if(m_nUcomReadPos!=0)
                    {
                        //剔除无效的指令头
                        memcpy(m_pUcomRcvBuf, &m_pUcomRcvBuf[m_nUcomReadPos],m_nUcomWritePos-m_nUcomReadPos);
                        m_nUcomWritePos -= m_nUcomReadPos;
                        m_nUcomReadPos=0;
                    }

                    //从串口外设中获取接收到的串口数据,并返回实际上接收到的字节数 -- 仅注释 杜寒枫 2017.06.28
                    iRecvCount = HW_UART_RecvBuff(&m_pUcomRcvBuf[m_nUcomWritePos] , UCOM_PACK_MAX_LEN-m_nUcomWritePos);
                    
                    if (iRecvCount<=0 && m_nUcomWritePos==0)
                        return 1;
    
                    m_nUcomWritePos += iRecvCount;

                    //查找包头
                    for (int i=0; i<m_nUcomWritePos; i++)
                    {
                        if (m_pUcomRcvBuf[i]==m_pHead[0])//找到可能是包头
                        {
                            nRestCount = m_nUcomWritePos-i; //nRestCount为当前未查找的数据包长度

                            if (nRestCount>=UCOM_SIZE_HEAD)//检查包头长度
                            {
                                if (memcmp(&m_pUcomRcvBuf[i],m_pHead,UCOM_SIZE_HEAD)==0)//找到文件头
                                {
                                    m_bUcomGetPackStep = UCOM_GET_PACK_STEP_LEN;    //将接下来的工作步骤设置为获取其数据帧长度 -- 仅注释 杜寒枫 2017.06.28
                                    m_nUcomReadPos = UCOM_SIZE_HEAD;
                                    break;
                                }
                            }
                            //包头长度不够,则将疑似包头及之后的数据移至缓冲区起始位置
                            else
                            {
                                //例如,将"xxxVxxxxx"--->"Vxxxxx"
                                //并将其m_nUcomWritePos位置变量移到相应的位置
                                // -- 仅注释 杜寒枫 2017.06.28
                                memcpy(m_pUcomRcvBuf, &m_pUcomRcvBuf[i], m_nUcomWritePos-i);
                                m_nUcomWritePos -= i;
                                return 1;
                            }
                        }
                    }

                    if (m_bUcomGetPackStep==UCOM_GET_PACK_STEP_HEAD)//包头接收不完整则继续接收数据
                    {
                        m_nUcomWritePos = 0; 
                        return 1;               
                    }
                }
            case UCOM_GET_PACK_STEP_LEN: //获取包长度
                {
                    if (m_nUcomWritePos-m_nUcomReadPos >= UCOM_SIZE_LENSEG)//已经读取到数据长度
                    {
                        m_nUcomDataLen = *(uBit16 *)&m_pUcomRcvBuf[UCOM_SIZE_HEAD];

                        //如果长度数据错误，则重新找包头，丢弃不正确数据包
                        if (m_nUcomDataLen < (UCOM_SIZE_HEAD+UCOM_SIZE_LENSEG+UCOM_SIZE_CHECKNUM) ||
                            m_nUcomDataLen > UCOM_DATA_MAX_LENGTH)
                        {
                            m_bUcomGetPackStep = UCOM_GET_PACK_STEP_HEAD;
                            memcpy(m_pUcomRcvBuf, &m_pUcomRcvBuf[m_nUcomReadPos],m_nUcomWritePos-m_nUcomReadPos);
                            m_nUcomWritePos -= m_nUcomReadPos;
                            m_nUcomReadPos=0;
                        }
                        //否则,则计算出接来下要获取的数据包的长度,并将其工作步骤设置为获取数据区
                        //同时,将其m_nUcomReadPos的位置进行偏移UCOM_SIZE_LENSEG位
                        // -- 仅注释 杜寒枫 2017.06.28
                        else
                        {
                            m_nUcomDataLen -= (UCOM_SIZE_HEAD+UCOM_SIZE_LENSEG+UCOM_SIZE_CHECKNUM);//获取数据包长度
                            m_bUcomGetPackStep = UCOM_GET_PACK_STEP_DATA; 
                            m_nUcomReadPos += UCOM_SIZE_LENSEG;
                        }
                    }
                    //未读到数据长度，继续读数据区
                    else
                    {
                        iRecvCount = HW_UART_RecvBuff(&m_pUcomRcvBuf[m_nUcomWritePos] , UCOM_PACK_MAX_LEN-m_nUcomWritePos);
                        
                        if (iRecvCount<=0)//未接收到数据
                            return 1;

                        m_nUcomWritePos += iRecvCount;
                    }

                    break;
                }

            case UCOM_GET_PACK_STEP_DATA: //获取数据区
                {
                    if (m_nUcomWritePos-m_nUcomReadPos >= m_nUcomDataLen)//数据区获取完毕
                    {
                        m_bUcomGetPackStep = UCOM_GET_PACK_STEP_CHECKNUM;  
                        m_nUcomReadPos += m_nUcomDataLen;   
                    }
                    //没有获取完整数据区数据,继续读数据区
                    else
                    {
                        iRecvCount = HW_UART_RecvBuff(&m_pUcomRcvBuf[m_nUcomWritePos] , UCOM_PACK_MAX_LEN-m_nUcomWritePos);
                        
                        if (iRecvCount<=0)//未接收到数据
                            return 1;

                        m_nUcomWritePos += iRecvCount;
                    }

                    break;
                }

            case UCOM_GET_PACK_STEP_CHECKNUM: //获取校验字
                {
                    if (m_nUcomWritePos-m_nUcomReadPos >= UCOM_SIZE_CHECKNUM)//校验字获取完毕
                    {
                        nRcvCheckSum = *(uBit16 *)&m_pUcomRcvBuf[m_nUcomReadPos];

                        //计数校验字
                        for (uBit16  i=0; i<m_nUcomReadPos; i++)
                        {
                            nCheckSum += m_pUcomRcvBuf[i];
                        }

                        //比较校验字
                        if (nRcvCheckSum==nCheckSum)//成功获取数据包
                        {
                            *pRcvLen = m_nUcomDataLen;

                            //剔除掉帧头和数据帧长度信息,将剩余的数据拷贝给pRcvBuf
                            //例如,"VDMACH12Axxxxxxx"--->"Axxxxxxx",不包含帧头,数据帧长度位以及校验位的数据在里面
                            //-- 仅注释 杜寒枫 2017.06.28
                            memcpy(pRcvBuf, &m_pUcomRcvBuf[UCOM_SIZE_HEAD+UCOM_SIZE_LENSEG],m_nUcomDataLen);
                            
                            //将多余接收到的数据包保存移动到起始位置
                            m_nUcomWritePos -= (m_nUcomReadPos + UCOM_SIZE_CHECKNUM);
                            memcpy(m_pUcomRcvBuf, &m_pUcomRcvBuf[m_nUcomReadPos+UCOM_SIZE_CHECKNUM],m_nUcomWritePos);
                            m_bUcomGetPackStep = UCOM_GET_PACK_STEP_HEAD; 
                            m_nUcomReadPos = 0; 
                            return 0;
                        }else//校验出错
                        {
                            //从包头后面重新开始找包头
                            m_nUcomWritePos -= UCOM_SIZE_HEAD;
                            memcpy(m_pUcomRcvBuf, &m_pUcomRcvBuf[UCOM_SIZE_HEAD],m_nUcomWritePos);
                            m_bUcomGetPackStep = UCOM_GET_PACK_STEP_HEAD;  
                            m_nUcomReadPos = 0; 
                            return 1;
                        }   
                    }
                    //没有获取完整校验字,继续读数据区
                    else
                    {
                        iRecvCount = HW_UART_RecvBuff(&m_pUcomRcvBuf[m_nUcomWritePos] , UCOM_PACK_MAX_LEN-m_nUcomWritePos);                      
                        
                        if (iRecvCount<=0)//未接收到数据
                            return 1;

                        m_nUcomWritePos += iRecvCount;
                    }
                    break;
                }

            default:break;
        }
        
        break;
    }

    return 1;   
}



