/*
*********************************************************************************************************
* @file    systimer.h 
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



#ifndef _SYSTIMER_H_
#define _SYSTIMER_H_

// **************************************************************************	
// the includes
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "semphr.h"
#include "event_groups.h"

#include  <stdbool.h>
//!
//!
//! \defgroup SYSTIMER_H
//!
//@{


#ifdef __cplusplus
extern "C" {
#endif
	
	
// **************************************************************************
// the defines	
	
	
// **************************************************************************
// the typedefs		
	
	
	
// **************************************************************************
// the globals declaration	
extern uint8_t COMMovertimer;

// **************************************************************************
// the function prototypes	
void systimerforovertimeCreate(void);
void Startsystimerforovertime(void);
	
#ifdef __cplusplus
}
#endif // extern "C"

//@} // ingroup
#endif // end of _SYSTIMER_H_ definition	
