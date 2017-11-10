#ifndef __GC_HARDWARE_DEF_H
#define __GC_HARDWARE_DEF_H

/*****************************************************************************
 * 系统硬件资源参数
 ****************************************************************************/

#define GC_MAX_RES_DATA_LEN             (64)    //最大数据区长度(包含控制类型、控制字及数据区)

//格子柜结构参数定义
#define GC_MAX_ROW_COUNT                (4)     //货道最大行数
#define GC_MAX_COL_COUNT                (30)    //货道最大列数
#define GC_ROW_MASK                     (0x0F)
#define GC_COL_MASK                     (0x3FFFFFFF)
#define GC_MAX_LOCK_COUNT               (GC_MAX_ROW_COUNT * GC_MAX_COL_COUNT)   //柜门的数量

//门锁控制逻辑定义
#define GC_LOCK_DEF_PWR_DOWN_TIME       (1000)  //门锁关闭保持时间(ms)
#define GC_LOCK_DEF_PWR_ON_TIME         ( 250)  //门锁打开保持时间(ms)
#define GC_LOCK_ROW_OPEN_STATE          (false) //门锁行信号有效电平
#define GC_LOCK_COL_OPEN_STATE          (true)  //门锁列信号有效电平

#endif /* __GC_HARDWARE_DEF_H */
