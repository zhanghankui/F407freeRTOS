/*
*********************************************************************************************************
* @file    led.h 
* @author  smilingfrog 
* @company Dafanshu Innovations Co. Ltd
* @version 1.00
* @date    24-May-2018 22:09
* @brief   led

* Update record
* version   date           author          description
* V1.00     24-May-2018    smilingfrog     first release
*********************************************************************************************************
*/

#ifndef _LED_H_
#define _LED_H_
// **************************************************************************	
// the includes
#include "sys.h"


//!
//!
//! \defgroup LED
//!
//@{


#ifdef __cplusplus
extern "C" {
#endif

	
	
//LED�˿ڶ���
#define LED0 PAout(6)	// D2
#define LED1 PAout(7)	// D3	 

void LED_Init(void);//��ʼ��	


#ifdef __cplusplus
}
#endif // extern "C"

//@} // ingroup	
#endif // end of _LED_H_ definition	
