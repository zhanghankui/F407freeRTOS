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

#define SDOovertimer 50
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

	/* ��ȡ�Ǹ���ʱ��ʱ�䵽 */
	ulTimerID = (uint32_t)pvTimerGetTimerID(pxTimer);
	
	/* ����ʱ��0���� */
	if(ulTimerID == 0)
	{
		COMMovertimer = true;
	}
}

void systimerforovertimeCreate(void)
{
	const TickType_t  xTimerPer = SDOovertimer;//50ms��ʱ
	
	/* 
	  1. ������ʱ���������RTOS���ȿ�ʼǰ��ʼ����ʱ������ôϵͳ������Ż�ִ�С�
	  2. ��ʼ����ʱ��
	*/

	xsysTimers = xTimerCreate("Timer",          /* ��ʱ������ */
									 xTimerPer,    /* ��ʱ������,��λʱ�ӽ��� */
									 pdFALSE,          /* �������� */
									 (void *) 0,      /* ��ʱ��ID */
									 vsysTimerforovertimeCallback); /* ��ʱ���ص����� */

	if(xsysTimers == NULL)
	{
		/* û�д����ɹ����û�������������봴��ʧ�ܵĴ������ */
	}
	else
	{
	}
}
void Startsystimerforovertime(void)
{
	COMMovertimer = false;
	/* ������ʱ����ϵͳ������ſ�ʼ���� */
	if(xTimerStart(xsysTimers, 100) != pdPASS)/*���ԭ�ȶ�ʱ��û�п�ʼ����������ǿ�ʼ�����ԭ�ȶ�ʱ���������У�������������൱��xTimerReset����*/
	{
	 /* ��ʱ����û�н��뼤��״̬ */
	}	
}

