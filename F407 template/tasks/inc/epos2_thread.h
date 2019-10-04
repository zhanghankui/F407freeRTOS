/*
*********************************************************************************************************
* @file    epos2_thread.h 
* @author  smilingfrog 
* @company Dafanshu Innovations Co. Ltd
* @version 1.00
* @date    21-Mar-2019 22:39
* @brief   epos2 thread

* Update record
* version   date           author          description
* V1.00     21-Mar-2019    smilingfrog     first release
*********************************************************************************************************
*/



#ifndef _EPOS2_THREAD_H_
#define _EPOS2_THREAD_H_

// **************************************************************************	
// the includes

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "can_STM32.h"
#include "canfestival.h"
#include "data.h"

//!
//!
//! \defgroup EPOS2_THREAD_H
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


// **************************************************************************
// the function prototypes	
void EPOS2_init(void);	
void EPOS2_setzero(void);
void EPOS2_ipm(void);
void EPOS2_returnzero(void);
#ifdef __cplusplus
}
#endif // extern "C"

//@} // ingroup
#endif // end of _EPOS2_THREAD_H_ definition	

