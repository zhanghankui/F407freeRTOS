/*
*********************************************************************************************************
* @file    bsp_key.h 
* @author  smilingfrog 
* @company Dafanshu Innovations Co. Ltd
* @version 1.00
* @date    10-July-2018 15:08
* @brief   key

* Update record
* version   date           author          description
* V1.00     10-July-2018   smilingfrog     first release
*********************************************************************************************************
*/	



#ifndef _BSP_KEY_H_
#define _BSP_KEY_H_

// **************************************************************************	
// the includes
#include "sys.h"

//!
//!
//! \defgroup BSP_KEY_H
//!
//@{


#ifdef __cplusplus
extern "C" {
#endif
	
	
// **************************************************************************
// the defines	
#define KEY_COUNT    3	   					/* 按键个数, 3个独立建*/
	
/* 根据应用程序的功能重命名按键宏 */
#define KEY_DOWN_K0		KEY_0_DOWN
#define KEY_UP_K0		KEY_0_UP
#define KEY_LONG_K0		KEY_0_LONG

#define KEY_DOWN_K1		KEY_1_DOWN
#define KEY_UP_K1		KEY_1_UP
#define KEY_LONG_K1		KEY_1_LONG

#define KEY_DOWN_KWU		KEY_WU_DOWN
#define KEY_UP_KWU		KEY_WU_UP
#define KEY_LONG_KWU		KEY_WU_LONG	
	
	
/*
	按键滤波时间50ms, 单位10ms。
	只有连续检测到50ms状态不变才认为有效，包括弹起和按下两种事件
	即使按键电路不做硬件滤波，该滤波机制也可以保证可靠地检测到按键事件
*/
#define KEY_FILTER_TIME   4       /* 单位2ms， 持续8毫秒，认为短按事件 */   
#define KEY_LONG_TIME     500			/* 单位2ms， 持续1秒，认为长按事件 */	
	
// **************************************************************************
// the typedefs		
	
/* 按键ID, 主要用于bsp_KeyState()函数的入口参数 */
typedef enum
{
	KID_K0 = 0,
	KID_K1,
	KID_KWU,
}KEY_ID_E;	
	
/*
	每个按键对应1个全局的结构体变量。
*/
typedef struct
{
	/* 下面是一个函数指针，指向判断按键手否按下的函数 */
	uint8_t (*IsKeyDownFunc)(void); /* 按键按下的判断函数,1表示按下 */

	uint8_t  Count;			/* 滤波器计数器 */
	uint16_t LongCount;		/* 长按计数器 */
	uint16_t LongTime;		/* 按键按下持续时间, 0表示不检测长按 */
	uint8_t  State;			/* 按键当前状态（按下还是弹起） */
	uint8_t  RepeatSpeed;	/* 连续按键周期 */
	uint8_t  RepeatCount;	/* 连续按键计数器 */
}KEY_T;

/*
	定义键值代码, 必须按如下次序定时每个键的按下、弹起和长按事件

	推荐使用enum, 不用#define，原因：
	(1) 便于新增键值,方便调整顺序，使代码看起来舒服点
	(2) 编译器可帮我们避免键值重复。
*/
typedef enum
{
	KEY_NONE = 0,			/* 0 表示按键事件 */

	KEY_0_DOWN,				/* 0键按下 */
	KEY_0_UP,				/* 0键弹起 */
	KEY_0_LONG,				/* 0键长按 */

	KEY_1_DOWN,				/* 1键按下 */
	KEY_1_UP,				/* 1键弹起 */
	KEY_1_LONG,				/* 1键长按 */

	KEY_WU_DOWN,				/* WU键按下 */
	KEY_WU_UP,				/* WU键弹起 */
	KEY_WU_LONG,				/* WU键长按 */
}KEY_ENUM;

/* 按键FIFO用到变量 */
#define KEY_FIFO_SIZE	10
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* 键值缓冲区 */
	uint8_t Read;					/* 缓冲区读指针1 */
	uint8_t Write;					/* 缓冲区写指针 */
	uint8_t Read2;					/* 缓冲区读指针2 */
}KEY_FIFO_T;


// **************************************************************************
// the globals declaration	


// **************************************************************************
// the function prototypes	
/* 供外部调用的函数声明 */
void bsp_InitKey(void);
void bsp_KeyScan(void);
void bsp_PutKey(uint8_t _KeyCode);
uint8_t bsp_GetKey(void);
uint8_t bsp_GetKey2(void);
uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID);
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed);
void bsp_ClearKey(void);	
	
#ifdef __cplusplus
}
#endif // extern "C"

//@} // ingroup
#endif // end of _BSP_KEY_H_ definition	
