#ifndef  UART_INTERFACE_H
#define  UART_INTERFACE_H

#include "./UartDataStructDef.h"
#include "../../DataType/DataType.h"

#ifdef __cplusplus
extern "C" {
#endif



/*****************************************************************************
 * 串口接口输出
 ****************************************************************************/

/**
  * @brief  串口接口输出
  * @param  ulID ID
  * @retval None
  */
void UART_ExportInterface(UART_INTERFACE *pUartInterface);

#ifdef __cplusplus
}
#endif


#endif /* UART_INTERFACE_H */