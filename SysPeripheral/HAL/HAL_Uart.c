/**
  ******************************************************************************
  * @file    HAL_Uart.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.07.18
  * @brief   UART drivers
  ******************************************************************************
  * @attention
  * 
  * UART和UART: 串口0,2,3 可以实现同步模式下的传输,而串口1仅能工作在异步模式下.
  * 
  * 引脚,
  *     U0_TXD  --- P0_2(Fun1),
  *     U0_RXD  --- P0_3(Fun1),
  *     
  *     U1_TXD  --- P0_15(Fun1),  P2_0(Fun2),  
  *     U1_RXD  --- P0_16(Fun1),  P2_1(Fun2),  
  *     
  *     U2_TXD  --- P0_10(Fun1),  P2_8(Fun2), 
  *     U2_RXD  --- P0_11(Fun1),  P2_9(Fun2), 
  *     
  *     U3_TXD  --- P0_1(Fun2),   P0_25(Fun3),  P4_28(Fun3),  
  *     U3_RXD  --- P0_0(Fun2),   P0_26(Fun3),  P4_29(Fun3),  
  * 
  * 
  * 注1,
  *     本文件不对入参 LPC_USART_T *pUART 进行错误判断,请调用者确保其 pUART 为
  *     正确的外设指针.
  * 
  * 
  * 注2,
  *     如果要使用DMA模式,则需要对UART的FIFO开启DMA支持(UART_FCR_DMAMODE_SEL)
  * 
  * V1.1------------
  * 修改描述: 修改串口实现机制,接收使用中断接收机制,而发送使用DMA+链表方式实现
  * 链表管理: 假如外部函数调用DMA发送函数,而入参的缓冲区是栈上的局部变量,那么在
  *           外部函数结束后栈上的空间被回收(此时UART还在发送中).如果此时在别的
  *           函数中修改了栈上的数据,串口便发送了被修改的数据.故而修改其发送机制,
  *           在发送函数中,将栈上临时存储的数据复制到堆上的空间中,用链表的方式链
  *           接多个发送缓冲区,当DMA发送完成后(DMA发送完成中断),释放其申请的空间.
  *           只要堆上可用的空间足够,CPU的占用时间仅为从栈到堆的搬运时间
  * 修改作者: Duhanfeng
  * 当前版本: V1.1
  * 修改日期: 2017.08.08
  *   
  * V1.2------------
  * 修改描述: 修复使能多个串口时DMA发送引起断言报警的问题(对NULL进行free)
  * 错误原因: 在每次调用HAL_UART_Init函数对多个串口进行初始化配置的时候,都会调用一次
  *           Chip_GPDMA_Init(LPC_GPDMA)函数去对DMA进行初始化.而DMA的再初始化会
  *           导致为先前申请的空闲DMA通道复位,多个串口复用同一个DMA通道,从而导致
  *           在触发DMA_TX中断时,对多个串口的链表进行操作(释放内存操作),而在空闲
  *           的串口之中,链表头指向NULL,从而导致对NULL进行free,引起断言报警.
  * 解决方法: 不在HAL_UART_Init中调用Chip_GPDMA_Init,而是将Chip_GPDMA_Init作为串口配
  *           置的前置条件,放在串口初始化之前.
  * 修改作者: Duhanfeng
  * 当前版本: V1.2
  * 修改日期: 2017.08.08
  * 
  * V1.3------------
  * 修改描述: 1.将提供给外接的接口抽象,不再使用LPC_USART_T* 类型的入参作为串口号的索引,
  *           而是依据具体入参的串口节点号,避免向外接暴露LPC平台的相关信息
  *           2.开放串口使能/禁止的接口
  * 修改作者: Duhanfeng
  * 当前版本: V1.3
  * 修改日期: 2017.11.07
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "HAL_Uart.h"
#include "HAL_IRQHandler.h"
#include "chip.h"
#include <string.h>
#include <stdlib.h>

#ifndef SL_ASSERT
#define SL_ASSERT(x)
#endif

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

//环形缓冲区定义
#define UART_RB_SIZE    (512)
static RINGBUFF_T m_RxRing[HAL_UART_NODE_NUM] = {0};
static uint8_t    m_uRxBuff[HAL_UART_NODE_NUM][UART_RB_SIZE] = {0};

//串口相关变量
static LPC_USART_T * const USART[HAL_UART_NODE_NUM] = {(LPC_USART_T *)LPC_UART0_BASE, (LPC_USART_T *)LPC_UART1_BASE, (LPC_USART_T *)LPC_UART2_BASE, (LPC_USART_T *)LPC_UART3_BASE};    //串口的基地址
static const IRQn_Type USART_IRQn[HAL_UART_NODE_NUM] = {UART0_IRQn, UART1_IRQn, UART2_IRQn, UART3_IRQn};   //中断向量

//DMA相关变量
static uint8_t m_uDmaFreeChannel[HAL_UART_NODE_NUM] = {0};    //串口的空闲通道
static const uint8_t m_uDmaUartTxMuxCode[HAL_UART_NODE_NUM] = {GPDMA_CONN_UART0_Tx, GPDMA_CONN_UART1_Tx, GPDMA_CONN_UART2_Tx, GPDMA_CONN_UART3_Tx};

//中断回调函数
static void HAL_UART0_RX_IRQHandler(void);
static void HAL_UART1_RX_IRQHandler(void);
static void HAL_UART2_RX_IRQHandler(void);
static void HAL_UART3_RX_IRQHandler(void);

static void HAL_UART0_DMATC_IRQHandler(void);
static void HAL_UART1_DMATC_IRQHandler(void);
static void HAL_UART2_DMATC_IRQHandler(void);
static void HAL_UART3_DMATC_IRQHandler(void);


/*****************************************************************************
 * UART IO 配置
 ****************************************************************************/

/**
  * @brief  UART_IO_MUX初始化(选定某组引脚作为UART的引脚).
  * @param  uUartNode 串口节点号
  * @retval None
  */
static void UART_IO_Init(uint8_t uUartNode)
{
    switch (uUartNode)
    {
    case HAL_UART_NODE0:
        Chip_IOCON_PinMux(LPC_IOCON, 0x00,  2, IOCON_MODE_INACT, IOCON_FUNC1);
        Chip_IOCON_PinMux(LPC_IOCON, 0x00,  3, IOCON_MODE_INACT, IOCON_FUNC1);
        break;                      
    case HAL_UART_NODE1:
#if 1
        Chip_IOCON_PinMux(LPC_IOCON, 0x00, 15, IOCON_MODE_INACT, IOCON_FUNC1);
        Chip_IOCON_PinMux(LPC_IOCON, 0x00, 16, IOCON_MODE_INACT, IOCON_FUNC1);
#else
                                    
        Chip_IOCON_PinMux(LPC_IOCON, 0x02,  0, IOCON_MODE_INACT, IOCON_FUNC2);
        Chip_IOCON_PinMux(LPC_IOCON, 0x02,  1, IOCON_MODE_INACT, IOCON_FUNC2);
#endif
        break;
    case HAL_UART_NODE2:
#if 1
        Chip_IOCON_PinMux(LPC_IOCON, 0x00, 10, IOCON_MODE_INACT, IOCON_FUNC1);
        Chip_IOCON_PinMux(LPC_IOCON, 0x00, 11, IOCON_MODE_INACT, IOCON_FUNC1);
#else
        Chip_IOCON_PinMux(LPC_IOCON, 0x02,  8, IOCON_MODE_INACT, IOCON_FUNC2);
        Chip_IOCON_PinMux(LPC_IOCON, 0x02,  9, IOCON_MODE_INACT, IOCON_FUNC2);
#endif
        break;
    case HAL_UART_NODE3:
#if 1
        Chip_IOCON_PinMux(LPC_IOCON, 0x00,  1, IOCON_MODE_INACT, IOCON_FUNC2);
        Chip_IOCON_PinMux(LPC_IOCON, 0x00,  0, IOCON_MODE_INACT, IOCON_FUNC2);
#elif 1
        Chip_IOCON_PinMux(LPC_IOCON, 0x00, 25, IOCON_MODE_INACT, IOCON_FUNC3);
        Chip_IOCON_PinMux(LPC_IOCON, 0x00, 26, IOCON_MODE_INACT, IOCON_FUNC3);
#else
        Chip_IOCON_PinMux(LPC_IOCON, 0x04, 28, IOCON_MODE_INACT, IOCON_FUNC3);
        Chip_IOCON_PinMux(LPC_IOCON, 0x04, 29, IOCON_MODE_INACT, IOCON_FUNC3);
#endif
        break;
    default : break;
    }
    
}


/*****************************************************************************
 * 串口功能配置(私有)
 ****************************************************************************/

/**
  * @brief  串口中断接收功能初始化
  * @param  uUartNode 串口节点号
  * @retval None
  */
static void HAL_UART_RxItInit(uint8_t uUartNode)
{
    //初始化环形FIFO
    RingBuffer_Init(&m_RxRing[uUartNode], m_uRxBuff[uUartNode], 1, UART_RB_SIZE);
    
    
    //使能外设中断
    Chip_UART_IntEnable(USART[uUartNode], UART_IER_RBRINT);
    
    //使能内核中断
    NVIC_SetPriority(USART_IRQn[uUartNode], 1);
    NVIC_EnableIRQ(USART_IRQn[uUartNode]);

    //设置中断回调
    switch (uUartNode)
    {
    case HAL_UART_NODE0: 
        {
            HAL_IRQ_SetTrgCallback(HAL_UART0_RX_IRQHandler, IRQ_TRG_UART0);
            break;
        }
    case HAL_UART_NODE1: 
        {
            HAL_IRQ_SetTrgCallback(HAL_UART1_RX_IRQHandler, IRQ_TRG_UART1);
            break;
        }
    case HAL_UART_NODE2: 
        {
            HAL_IRQ_SetTrgCallback(HAL_UART2_RX_IRQHandler, IRQ_TRG_UART2);
            break;
        }
    case HAL_UART_NODE3: 
        {
            HAL_IRQ_SetTrgCallback(HAL_UART3_RX_IRQHandler, IRQ_TRG_UART3);
            break;
        }
    default: break; //不应该出现在这里
    }
    
}


/**
  * @brief  串口DMA发送功能初始化
  * @param  uUartNode 串口节点号
  * @retval None
  */
static void HAL_UART_DmaTxInit(uint8_t uUartNode)
{
    //获取空闲通道
    m_uDmaFreeChannel[uUartNode] = Chip_GPDMA_GetFreeChannel(LPC_GPDMA, 0);
    
    //使能中断
    NVIC_DisableIRQ(DMA_IRQn);
    NVIC_SetPriority(DMA_IRQn, ((0x01 << 3) | 0x01));
    NVIC_EnableIRQ(DMA_IRQn);
    
    //配置中断
    switch (uUartNode)
    {
    case HAL_UART_NODE0: 
        {
            //设置中断回调
            HAL_IRQ_SetTrgCallback(HAL_UART0_DMATC_IRQHandler, IRQ_TRG_DMA_CH1 + m_uDmaFreeChannel[0]);
            break;
        }
    case HAL_UART_NODE1: 
        {
            //设置中断回调
            HAL_IRQ_SetTrgCallback(HAL_UART1_DMATC_IRQHandler, IRQ_TRG_DMA_CH1 + m_uDmaFreeChannel[1]);
            break;
        }
    case HAL_UART_NODE2: 
        {
            //设置中断回调
            HAL_IRQ_SetTrgCallback(HAL_UART2_DMATC_IRQHandler, IRQ_TRG_DMA_CH1 + m_uDmaFreeChannel[2]);
            break;
        }
    case HAL_UART_NODE3: 
        {
            //设置中断回调
            HAL_IRQ_SetTrgCallback(HAL_UART3_DMATC_IRQHandler, IRQ_TRG_DMA_CH1 + m_uDmaFreeChannel[3]);
            break;
        }
    default: break; //不应该出现在这里
    }
    
}


/*****************************************************************************
 * UART 基本功能接口
 ****************************************************************************/

/**
  * @brief  串口初始化
  * @param  uUartNode 串口节点号
  * @param  ulBaudRate 波特率
  * @retval None
  * Note    当调用初始化函数后,无需再调用使能函数去使能串口
  */
void HAL_UART_Init(uint8_t uUartNode, uint32_t ulBaudRate)
{
    //初始化IO
    UART_IO_Init(uUartNode);
    
    //初始化串口基本配置
    Chip_UART_Init(USART[uUartNode]);

    //初始化波特率
    Chip_UART_SetBaud(USART[uUartNode], ulBaudRate);
    
    //配置工作模式(8-1-N)
    Chip_UART_ConfigData(USART[uUartNode], UART_LCR_WLEN8 | UART_LCR_SBS_1BIT | UART_LCR_PARITY_DIS);
    
    //配置FIFO(使能并支持DMA模式)
    Chip_UART_SetupFIFOS(USART[uUartNode], (UART_FCR_FIFO_EN | UART_FCR_RX_RS | UART_FCR_TX_RS | UART_FCR_DMAMODE_SEL | UART_FCR_TRG_LEV3));
    
    //初始化中断配置(Rx)
    HAL_UART_RxItInit(uUartNode);
    
    //初始化DMA配置(Tx)
    HAL_UART_DmaTxInit(uUartNode);
    
    //使能发送
    Chip_UART_TXEnable(USART[uUartNode]);
    
}


/**
  * @brief  串口使能
  * @param  uUartNode 串口节点号
  * @param  bIsEnable 使能标志
  * @retval None
  */
void HAL_UART_Enable(uint8_t uUartNode, bool bIsEnable)
{
    if (bIsEnable)
    {
        //开接收中断
        Chip_UART_IntEnable(USART[uUartNode], UART_IER_RBRINT);
        
        //使能发送
        Chip_UART_TXEnable(USART[uUartNode]);
        
    }
    else 
    {
        //关接收中断
        Chip_UART_IntDisable(USART[uUartNode], UART_IER_RBRINT);
        
        //禁止发送
        Chip_UART_TXDisable(USART[uUartNode]);
        
    }
   
}


/**
  * @brief  串口发送状态获取
  * @param  uUartNode 串口节点号
  * @retval 0-发送中 非0-发送完成
  */
uint32_t HAL_UART_GetTransStatus(uint8_t uUartNode)
{
    
    ;
    
    return (Chip_UART_ReadLineStatus(USART[uUartNode]) & UART_LSR_TEMT) != 0;
}


/**
  * @brief  单字节发送(阻塞)
  * @param  uUartNode 串口节点号
  * @param  uData 要发送的字节数据
  * @retval None
  */
void HAL_UART_SendByte(uint8_t uUartNode, uint8_t uData)
{
    Chip_UART_SendByte(USART[uUartNode], uData);
    
}


/**
  * @brief  缓冲区发送(阻塞)
  * @param  uUartNode 串口节点号
  * @param  pSBuff 要发送的缓冲区
  * @param  ulSize 要发送的字节数
  * @retval None
  */
void HAL_UART_SendBuff(uint8_t uUartNode, void *pSBuff, uint32_t ulSize)
{
    Chip_UART_SendBlocking(USART[uUartNode], pSBuff, ulSize);

}


/**
  * @brief  字符串发送函数(阻塞性发送)
  * @param  uUartNode 串口节点号
  * @param  pSBuff 要发送的字符串指针
  * @retval None
  * @note   本函数是根据'\0'作为缓冲区长度依据,要保证入参的数据是一个字符串
  */
void HAL_UART_SendStr(uint8_t uUartNode, uint8_t *pSBuff)
{
    uint32_t i = 0;
    
    while (pSBuff[i] != '\0')
    {
        i++;
    }

    HAL_UART_SendBuff(uUartNode, pSBuff, i);
    
}


/**
  * @brief  串口接收(非阻塞,实际上就是读取出接收缓冲区中的数据)
  * @param  uUartNode 串口节点号
  * @param  pRBuff 要接收的缓冲区
  * @param  ulSize 要接收的数据长度
  * @retval uint32_t 实际上接收到的数据长度
  */
uint32_t HAL_UART_RecvBuff(uint8_t uUartNode, void *pRBuff, uint32_t ulSize)
{
    
    return Chip_UART_ReadRB(USART[uUartNode], &m_RxRing[uUartNode], pRBuff, ulSize);
}


/**
  * @brief  数据接收处理(非阻塞接收线程)
  * @param  uUartNode 串口节点号
  * @retval None
  * @retval 本函数是应用于非中断接收的情况,单独一个线程做数据接收
  */
void HAL_UART_RecvHandler(uint8_t uUartNode)
{
    uint8_t uData = 0;
    
    if (Chip_UART_Read(USART[uUartNode], &uData, 1))
    {
        RingBuffer_Insert(&m_RxRing[uUartNode], &uData);
    }
    
}


/*****************************************************************************
 * DMA链表传输接口
 ****************************************************************************/

//链表结构
typedef struct Usart_Tx_Node
{
      struct Usart_Tx_Node *pNext;  //下一个节点的地址(若为节点尾,则定义为NULL)
      uint32_t  ulLen;              //数据长度
      uint8_t   uBuff[];            //空数组
}UART_TX_NODE;


//链表首指针
static UART_TX_NODE *g_HeadNodePtr0 = NULL; //第0条链表的首节点指针
static UART_TX_NODE *g_HeadNodePtr1 = NULL; //第1条链表的首节点指针
static UART_TX_NODE *g_HeadNodePtr2 = NULL; //第2条链表的首节点指针
static UART_TX_NODE *g_HeadNodePtr3 = NULL; //第3条链表的首节点指针


/**
  * @brief  节点初始化
  * @param  pNode 要初始化的节点
  * @retval None
  */
static void List_Init(UART_TX_NODE *pNode)
{
    pNode->pNext = pNode;
  
}


/**
  * @brief  节点尾部嵌入
  * @param  pHeadNode 首节点地址
  * @param  pTailNode 要嵌入的尾节点
  * @retval None
  */
static void List_SinkTail(UART_TX_NODE *pHeadNode, UART_TX_NODE *pTailNode)
{
    SL_ASSERT(pHeadNode);
    SL_ASSERT(pTailNode);

    //指针偏移到链表尾部
    while (pHeadNode->pNext != pHeadNode)
    {
        pHeadNode = pHeadNode->pNext;
    }

    //嵌入尾部
    pHeadNode->pNext = pTailNode;
  
}


/**
  * @brief  节点释放
  * @param  pHeadNode 要释放的首节点地址
  * @retval 最新的首节点地址
  */
static UART_TX_NODE *List_FreeHead(UART_TX_NODE *pHeadNode)
{
    UART_TX_NODE *tmpNode = NULL;

    SL_ASSERT(pHeadNode);

    //假如链表并非只有单个节点,则返回最新的首节点的地址
    if (pHeadNode->pNext != pHeadNode)
    {
        tmpNode = pHeadNode->pNext;

    }
    memset(pHeadNode, 0, sizeof(UART_TX_NODE));
    free(pHeadNode);

    return tmpNode;
}


/**
  * @brief  首节点指针获取
  * @param  uUartNode 串口节点号
  * @retval None
  */
static UART_TX_NODE **List_GetHead(uint8_t uUartNode)
{
    UART_TX_NODE **pHeadNode = NULL;

    switch (uUartNode)
    {
    case HAL_UART_NODE0: pHeadNode = &g_HeadNodePtr0; break;
    case HAL_UART_NODE1: pHeadNode = &g_HeadNodePtr1; break;
    case HAL_UART_NODE2: pHeadNode = &g_HeadNodePtr2; break;
    case HAL_UART_NODE3: pHeadNode = &g_HeadNodePtr3; break;
    default: break;
    }

    return pHeadNode;
}


/**
  * @brief  数据DMA发送
  * @param  uUartNode 串口节点号
  * @param  pSBuff 数据缓冲区地址
  * @param  nSize 发送的数量
  * @retval None
  */
static void HAL_UART_DmaSend(uint8_t uUartNode, uint8_t *pSBuff, uint32_t ulSize)
{
    //调用DMA传输函数
    Chip_GPDMA_Transfer(LPC_GPDMA, m_uDmaFreeChannel[uUartNode],
                      (uint32_t) pSBuff,
                      m_uDmaUartTxMuxCode[uUartNode],
                      GPDMA_TRANSFERTYPE_M2P_CONTROLLER_DMA,
                      ulSize);
    
}


/**
  * @brief  当前链表首节点数据发送
  * @param  uUartNode 串口节点号
  * @retval None
  */
static void HAL_UART_SendHeadNodeBuff(uint8_t uUartNode)
{
    //获取节点指针
    UART_TX_NODE **pHeadNode = List_GetHead(uUartNode);
    SL_ASSERT(pHeadNode);
    SL_ASSERT(*pHeadNode);

    //发送节点中的数据
    HAL_UART_DmaSend(uUartNode, (*pHeadNode)->uBuff, (*pHeadNode)->ulLen);
  
}


/**
  * @brief  串口数据DMA发送(非阻塞)
  * @param  uUartNode 串口节点号
  * @param  pSBuff 数据缓冲区地址
  * @param  nSize 发送的数量
  * @retval None
  */
void HAL_UART_DmaSendBuff(uint8_t uUartNode, void *pSBuff, uint32_t ulSize)
{
    UART_TX_NODE *pTxNode = NULL;
    UART_TX_NODE **pHeadNode = List_GetHead(uUartNode);

    //判断参数
    SL_ASSERT(ulSize);

    //分配空间(若分配不成功,则重复申请,以保证数据能发送出去)
    //如果要传输的数量量太大而导致内存空间不够,会阻塞在这里.由于此时内存空间是一边申请一边释放,
    //所以在超大数据量传输的情况中,超限数据的阻塞时间==超限数据的串口传输时间(依据波特率)
    //如果要增强其数据容量,需要增大其链表可以申请的内存空间的容量,即堆的容量
    while ((pTxNode = (UART_TX_NODE *)malloc(sizeof(UART_TX_NODE) + ulSize*sizeof(unsigned char))) == NULL);

    //初始化链表
    List_Init(pTxNode);

    //将堆上的数据存储到节点空间中
    memcpy(pTxNode->uBuff, pSBuff, ulSize);
    pTxNode->ulLen = ulSize;

    //嵌入链表
    if (*pHeadNode != NULL)  //假如当前链表非空(发送中)
    {
        //将节点嵌入链表
        List_SinkTail(*pHeadNode, pTxNode);
    }
    else //假如当前链表为空(串口空闲),启动传输
    {
        *pHeadNode = pTxNode; //将当前节点设置为链表头
        HAL_UART_SendHeadNodeBuff(uUartNode);  //发送链表头节点的数据
    }

}


/**
  * @brief  串口字符串DMA发送函数(非阻塞)
  * @param  uUartNode 串口节点号
  * @param  pSBuff 字符串指针
  * @retval None
  * @note   本函数是根据'\0'作为缓冲区长度依据,要保证入参的数据是一个字符串
  */
void HAL_UART_DmaSendStr(uint8_t uUartNode, uint8_t *pSBuff)
{
    uint32_t i = 0;
    
    while (pSBuff[i] != '\0') i++;

    HAL_UART_DmaSendBuff(uUartNode, pSBuff, i);
    
}



/*****************************************************************************
 * 中断回调函数
 ****************************************************************************/

/**
  * @brief  串口接收中断回调函数
  * @param  None
  * @retval None
  */
static void HAL_UART0_RX_IRQHandler(void)
{
    Chip_UART_RXIntHandlerRB(LPC_UART0, &m_RxRing[0]);
    
}


/**
  * @brief  串口接收中断回调函数
  * @param  None
  * @retval None
  */
static void HAL_UART1_RX_IRQHandler(void)
{
    Chip_UART_RXIntHandlerRB(LPC_UART1, &m_RxRing[1]);
    
}


/**
  * @brief  串口接收中断回调函数
  * @param  None
  * @retval None
  */
static void HAL_UART2_RX_IRQHandler(void)
{
    Chip_UART_RXIntHandlerRB(LPC_UART2, &m_RxRing[2]);
    
}


/**
  * @brief  串口接收中断回调函数
  * @param  None
  * @retval None
  */
static void HAL_UART3_RX_IRQHandler(void)
{
    Chip_UART_RXIntHandlerRB(LPC_UART3, &m_RxRing[3]);
    
}


/**
  * @brief  串口DMA发送完成中断中断回调函数
  * @param  None
  * @retval None
  */
static void HAL_UART0_DMATC_IRQHandler(void)
{
    g_HeadNodePtr0 = List_FreeHead(g_HeadNodePtr0);

    if (g_HeadNodePtr0 != NULL)  //若当前的首节点非空
    {
        HAL_UART_SendHeadNodeBuff(HAL_UART_NODE0);
    }
    
}


/**
  * @brief  串口DMA发送完成中断中断回调函数
  * @param  None
  * @retval None
  */
static void HAL_UART1_DMATC_IRQHandler(void)
{
    g_HeadNodePtr1 = List_FreeHead(g_HeadNodePtr1);

    if (g_HeadNodePtr1 != NULL)  //若当前的首节点非空
    {
        HAL_UART_SendHeadNodeBuff(HAL_UART_NODE1);
    }
    
}


/**
  * @brief  串口DMA发送完成中断中断回调函数
  * @param  None
  * @retval None
  */
static void HAL_UART2_DMATC_IRQHandler(void)
{
    g_HeadNodePtr2 = List_FreeHead(g_HeadNodePtr2);

    if (g_HeadNodePtr2 != NULL)  //若当前的首节点非空
    {
        HAL_UART_SendHeadNodeBuff(HAL_UART_NODE2);
    }

}


/**
  * @brief  串口DMA发送完成中断中断回调函数
  * @param  None
  * @retval None
  */
static void HAL_UART3_DMATC_IRQHandler(void)
{
    g_HeadNodePtr3 = List_FreeHead(g_HeadNodePtr3);

    if (g_HeadNodePtr3 != NULL)  //若当前的首节点非空
    {
        HAL_UART_SendHeadNodeBuff(HAL_UART_NODE3);
    }

}

