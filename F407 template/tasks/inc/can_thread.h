#ifndef __CANOPEN_THREAD_H__
#define __CANOPEN_THREAD_H__

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "can_STM32.h"
#include "canfestival.h"
#include "data.h"

//#define BIT_0_CAN1Dispatched	(1 << 0)


typedef struct _struct_co_data_2_can{
	CO_Data *CO_CAN1;
	CO_Data *CO_CAN2;
} CO_DATA_STRUCT;
extern CO_DATA_STRUCT  CO_D;

void canopen_init(void);
#endif
