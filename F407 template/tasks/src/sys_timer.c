/*
*********************************************************************************************************
* @file    systimer.c 
* @author  smilingfrog 
* @company Dafanshu Innovations Co. Ltd
* @version 1.00
* @date    23-Mar-2019 14:15
* @brief   sys timer

* Update record
* version   date           author          description
* V1.00     23-Mar-2019    smilingfrog     first release
*********************************************************************************************************
*/	
	
// **************************************************************************	
// the includes
#include "systimer.h"

// **************************************************************************
// the defines


// **************************************************************************
// the typedefs	


// **************************************************************************
//declaration static function prototypes
static TimerHandle_t xsysTimers = {NULL};


// **************************************************************************
// the globals
uint8_t COMMovertimer = false;

// **************************************************************************
// the functions

void vsysTimerforovertimeCallback(xTimerHandle pxTimer)
{
	uint32_t ulTimerID;

	configASSERT(pxTimer);

	/* 获取那个定时器时间到 */
	ulTimerID = (uint32_t)pvTimerGetTimerID(pxTimer);
	
	/* 处理定时器0任务 */
	if(ulTimerID == 0)
	{
		COMMovertimer = true;
	}
}

void systimerforovertimeCreate(void)
{
	const TickType_t  xTimerPer = 10;
	
	/* 
	  1. 创建定时器，如果在RTOS调度开始前初始化定时器，那么系统启动后才会执行。
	  2. 初始化定时器
	*/

	xsysTimers = xTimerCreate("Timer",          /* 定时器名字 */
									 xTimerPer,    /* 定时器周期,单位时钟节拍 */
									 pdFALSE,          /* 非周期性 */
									 (void *) 0,      /* 定时器ID */
									 vsysTimerforovertimeCallback); /* 定时器回调函数 */

	if(xsysTimers == NULL)
	{
		/* 没有创建成功，用户可以在这里加入创建失败的处理机制 */
	}
	else
	{
	}
}
void Startsystimerforovertime(void)
{
	COMMovertimer = false;
	/* 启动定时器，系统启动后才开始工作 */
	if(xTimerStart(xsysTimers, 100) != pdPASS)/*如果原先定时器没有开始，这个函数是开始，如果原先定时器就在运行，那这个函数就相当于xTimerReset函数*/
	{
	 /* 定时器还没有进入激活状态 */
	}	
}

