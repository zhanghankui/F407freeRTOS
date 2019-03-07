/*
This file is part of CanFestival, a library implementing CanOpen Stack.

Copyright (C): Edouard TISSERANT and Francis DUPIN
AT91 Port: Peter CHRISTEN

See COPYING file for copyrights details.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

// Includes for the Canfestival driver
#include "canfestival.h"
#include "timer.h"


void timer_can_irq_handler(void);
//ʱ��Ƭ����Ĺ��Ӻ�����ÿ��ʱ��Ƭ����һ��
void vApplicationTickHook( void );

//Ϊ���㲻ͬSTM32֮�����ֲ������ucos SYS_TICkʱ����Ϊ��ʱ����1msһ���ж�
// Define the timer registers

UNS32 TimerAlarm_CAN;                      // ���嶨ʱ��     
UNS32 TimerCounter_CAN;    				   //���嶨ʱ��
 

/************************** Modul variables **********************************/
// Store the last timer value to calculate the elapsed time
UNS32 last_time_set = 0; //TIMEVAL_MAX;

void initTimer(void)
/******************************************************************************
Initializes the timer, turn on the interrupt and put the interrupt time to zero
INPUT	void
OUTPUT	void
******************************************************************************/
{
 	  TimerCounter_CAN = 0;              //��ʼ����ʱ��Ϊ0
	  TimerAlarm_CAN   = 0;
}

void setTimer(TIMEVAL value)
/******************************************************************************
Set the timer for the next alarm.
INPUT	value TIMEVAL (unsigned long)
OUTPUT	void
******************************************************************************/
{
  TimerAlarm_CAN += value;	// Add the desired time to timer interrupt time
}

TIMEVAL getElapsedTime(void)
/******************************************************************************
Return the elapsed time to tell the stack how much time is spent since last call.
INPUT	void
OUTPUT	value TIMEVAL (unsigned long) the elapsed time
******************************************************************************/
{
  unsigned long timer = TimerCounter_CAN;	// Copy the value of the running timer
  // Calculate the time difference
  return timer > last_time_set ? timer - last_time_set : last_time_set - timer;
}
/*����ʱ���ĺ���*/
void  vApplicationTickHook(void)
{
    TimerCounter_CAN++;                       //CANopen ��ʱ��+1
    if(TimerCounter_CAN == TimerAlarm_CAN)    //����
    {
  		last_time_set = TimerCounter_CAN;
			TimeDispatch();	// Call the time handler of the stack to adapt the elapsed time	
    }
}
