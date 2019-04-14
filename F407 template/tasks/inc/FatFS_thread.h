/*
*********************************************************************************************************
* @file    FatFS_thread.h 
* @author  smilingfrog 
* @company Dafanshu Innovations Co. Ltd
* @version 1.00
* @date    14-Apri-2019 17:38
* @brief   FatFS thread

* Update record
* version   date           author          description
* V1.00     14-Apri-2019     smilingfrog     first release
*********************************************************************************************************
*/



#ifndef _FATFS_THREAD_H_
#define _FATFS_THREAD_H_

// **************************************************************************	
// the includes
#include "ff.h"
#include "stm32f4xx.h"
//!
//!
//! \defgroup FATFS_THREAD
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
extern FRESULT result;
//extern FIL file;
extern FATFS fs;

// **************************************************************************
// the function prototypes	
u8 exf_getfree(u8 *drv,u32 *total,u32 *free);	
	
#ifdef __cplusplus
}
#endif // extern "C"

//@} // ingroup
#endif // end of _FATFS_THREAD_H_ definition	
