#ifndef  __AT24CXX_H
#define  __AT24CXX_H

#include "../../DataType/DataType.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    AT24C01_PAGE_SIZE =  8,
    AT24C02_PAGE_SIZE =  8,
    AT24C04_PAGE_SIZE = 16,
    AT24C08_PAGE_SIZE = 16,
    AT24C16_PAGE_SIZE = 16,
    AT24C32_PAGE_SIZE = 32,
    AT24C64_PAGE_SIZE = 32,
    
}AT24CXX_PAGE_SIZE;


void AT24Cxx_InitInterface(void   *Data,
                           void   (*SetSDA)  (void *Data, uBit8 State),
                           void   (*SetSCL)  (void *Data, uBit8 State),
                           uBit8  (*GetSDA)  (void *Data),
                           uBit8  (*GetSCL)  (void *Data),
                           void   (*BitDelay)(void *Data),
                           AT24CXX_PAGE_SIZE    PAGE_SIZE);

uBit8 AT24Cxx_RandomRead(uBit16 nAddr);
void AT24Cxx_WriteByte(uBit16 nAddr, uBit8 cWriteData);
void AT24Cxx_ReadMultiBytes(uBit16 nAddr, uBit8 *pReadBuff, uBit32 iNum);
void AT24Cxx_WriteMultiBytes(uBit16 nAddr, const uBit8 *pWriteBuff, uBit32 iNum);


#ifdef __cplusplus
}
#endif
                                  

#endif  /* __AT24CXX_DEV_H */
