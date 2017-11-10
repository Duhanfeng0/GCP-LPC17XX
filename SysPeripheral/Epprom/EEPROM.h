/*********************************************Copyright (c)***********************************************
**                                Guangzhou ZLG MCU Technology Co., Ltd.
**
**                                        http://www.zlgmcu.com
**
**      广州周立功单片机科技有限公司所提供的所有服务内容旨在协助客户加速产品的研发进度，在服务过程中所提供
**  的任何程序、文档、测试结果、方案、支持等资料和信息，都仅供参考，客户有权不使用或自行参考修改，本公司不
**  提供任何的完整性、可靠性等保证，若在客户使用过程中因任何原因造成的特别的、偶然的或间接的损失，本公司不
**  承担任何责任。
**                                                                          ——广州周立功单片机科技有限公司
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           EEPROM.H
** Last modified Date:  2013-10-08
** Last Version:        V1.00
** Descriptions:        EEPROM的操作定义
**
**--------------------------------------------------------------------------------------------------------
** Created by:          Zhengxiaocheng
** Created date:        2013-10-08
** Version:             V1.00
** Descriptions:        创建头文件
**
**--------------------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
** Version:             
** Descriptions:        
** Checked by:          
** Rechecked by:           
*********************************************************************************************************/
#include "chip.h"

typedef struct {
    uint32_t CMD;                                                       /* EEPROM command register      */
    uint32_t nothing;                                                   /* nothing,do not operate it    */
    uint32_t RWSTATE;                                                   /* EEPROM 读等待状态寄存器      */
    uint32_t AUTOPROG;                                                  /* EEPROM 自动烧写寄存器        */
    uint32_t WSTATE;                                                    /* EEPROM 等待状态寄存器        */
    uint32_t CLKDIV;                                                    /* EEPROM 时钟分频寄存器        */
    uint32_t PWRDWN;                                                    /* EEPROM 掉电寄存器            */
}EEPROM_REG_TYPE;

typedef struct {
    uint32_t INTENCLR;                                                  /* EEPROM中断使能清除寄存器     */
    uint32_t INTENSET;                                                  /* EEPROM中断使能置位寄存器     */
    uint32_t INTSTAT;                                                   /* EEPROM中断状态寄存器         */
    uint32_t INTEN;                                                     /* EEPROM中断使能寄存器         */
    uint32_t INTSTATCLR;                                                /* EEPROM中断状态清除寄存器     */
    uint32_t INTSTATSET;                                                /* EEPROM中断状态置位寄存器     */
}EEPROM_INT_TYPE;

#define EEPROM_REG_BASE     0x4000E000                                  /* EEPROM寄存器的基地址         */

#define EEPROM_REG ((EEPROM_REG_TYPE *)(EEPROM_REG_BASE))               /* EEPROM寄存器结构体指针       */

#define EEPROM_INT ((EEPROM_INT_TYPE *)(EEPROM_REG_BASE + 0xFD8))       /* EEPROM中断寄存器结构体指针   */

/*********************************************************************************************************
** 定义EEPROM字边界操作,注意不要超过范围
** page的范围为0-127,最后一个页不可以擦写
** offset的范围为0-31; 
*********************************************************************************************************/
#define EEPROM_WORD_Data(page, offset) (* ( (uint32_t *)(0x20040000 + page * 128 + offset * 4) ) )

/*********************************************************************************************************
** 定义EEPROM字节边界操作,这个定义不可以用来擦写，注意不要超过范围
** page的范围为0-127,
** addr的范围为0-127; 
*********************************************************************************************************/
#define EEPROM_Byte_Data(page,addr)    (* ( (uint32_t *)(0x20040000 + page * 128 +addr) ) )

/*********************************************************************************************************
  End Of File
*********************************************************************************************************/

