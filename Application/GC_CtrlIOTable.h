#ifndef  __GC_IO_CTRL_H
#define  __GC_IO_CTRL_H

#include "../SysPeripheral/GPIO/GPIO_Man.h"

//输出IO编号
typedef enum
{
    GC_OUTPUT_IO_I2C_SDA = 0,
    GC_OUTPUT_IO_I2C_SCL,
    
    GC_OUTPUT_IO_LED,
    GC_OUTPUT_IO_OE_N,
    
    GC_OUTPUT_IO_BUZZER,
    
    GC_OUTPUT_IO_NIXIE_TUBE_CLK,
    GC_OUTPUT_IO_NIXIE_TUBE_DIO,
    GC_OUTPUT_IO_NIXIE_TUBE_STB,
    
    GC_OUTPUT_IO_ROW_0,
    GC_OUTPUT_IO_ROW_1,
    GC_OUTPUT_IO_ROW_2,
    GC_OUTPUT_IO_ROW_3,
    GC_OUTPUT_IO_ROW_4,
    GC_OUTPUT_IO_ROW_5,
    
    GC_OUTPUT_IO_COL_0,
    GC_OUTPUT_IO_COL_1,
    GC_OUTPUT_IO_COL_2,
    GC_OUTPUT_IO_COL_3,
    GC_OUTPUT_IO_COL_4,
    GC_OUTPUT_IO_COL_5,
    GC_OUTPUT_IO_COL_6,
    GC_OUTPUT_IO_COL_7,
    GC_OUTPUT_IO_COL_8,
    GC_OUTPUT_IO_COL_9,
    GC_OUTPUT_IO_COL_10,
    GC_OUTPUT_IO_COL_11,
    GC_OUTPUT_IO_COL_12,
    GC_OUTPUT_IO_COL_13,
    GC_OUTPUT_IO_COL_14,
    GC_OUTPUT_IO_COL_15,
    GC_OUTPUT_IO_COL_16,
    GC_OUTPUT_IO_COL_17,
    GC_OUTPUT_IO_COL_18,
    GC_OUTPUT_IO_COL_19,
    GC_OUTPUT_IO_COL_20,
    GC_OUTPUT_IO_COL_21,
    GC_OUTPUT_IO_COL_22,
    GC_OUTPUT_IO_COL_23,
    GC_OUTPUT_IO_COL_24,
    GC_OUTPUT_IO_COL_25,
    GC_OUTPUT_IO_COL_26,
    GC_OUTPUT_IO_COL_27,
    GC_OUTPUT_IO_COL_28,
    GC_OUTPUT_IO_COL_29,
    
    GC_OUTPUT_IO_INPUT_DOOR_LOCK,
    GC_OUTPUT_IO_INPUT_RESERVER,
    
}GC_OUTPUT_IO_TABLE;


//输入IO编号
typedef enum
{
    GC_INPUT_IO_KEY1 = 0,
    GC_INPUT_IO_INC0,

}GC_INPUT_IO_TABLE;




//格子柜控制IO资源表
extern const GPIO_CTRL_TABLE GC_CtrlIOTable;



#endif /* __GC_IO_CTRL_H */
