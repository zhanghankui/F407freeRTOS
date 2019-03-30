/*
*********************************************************************************************************
* @file    appdata.h 
* @author  smilingfrog 
* @company Dafanshu Innovations Co. Ltd
* @version 1.00
* @date    30-Mar-2019 2:02
* @brief   appdata

* Update record
* version   date           author          description
* V1.00     30-Mar-2019    smilingfrog     first release
*********************************************************************************************************
*/



#ifndef _APPDATA_H_
#define _APPDATA_H_

// **************************************************************************	
// the includes
#include "stm32f4xx.h" 

//!
//!
//! \defgroup APPDATA_H
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
extern const uint8_t tab_time[];

extern const int32_t tab_velocity[];

extern const int32_t tab_position[];

// **************************************************************************
// the function prototypes	
	
	
#ifdef __cplusplus
}
#endif // extern "C"

//@} // ingroup
#endif // end of _APPDATA_H_ definition	
