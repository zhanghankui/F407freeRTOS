/*
*********************************************************************************************************
* @file    usartdma.h 
* @author  smilingfrog 
* @company Dafanshu Innovations Co. Ltd
* @version 1.00
* @date    05-June-2018 01:20
* @brief   usart & dma

* Update record
* version   date           author          description
* V1.00     05-June-2018   smilingfrog     first release
*********************************************************************************************************
*/

#ifndef _USARTDMA_H_
#define _USARTDMA_H_

// **************************************************************************	
// the includes
#include "arm_math.h"		
#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
//!
//!
//! \defgroup USARTDMA
//!
//@{


#ifdef __cplusplus
extern "C" {
#endif
	
	
// **************************************************************************
// the defines	

#define COMM_USE_USART1	
	
#if defined COMM_USE_USART1
	#define COMM_USART              USART1
	
	#define USART_TX_PIN            GPIO_Pin_9
	#define USART_TX_GPIO_PORT      GPIOA
	#define USART_TX_GPIO_CLK       RCC_AHB1Periph_GPIOA
	#define USART_TX_GPIO_PinSource GPIO_PinSource9
	
	#define USART_RX_PIN            GPIO_Pin_10
	#define USART_RX_GPIO_PORT      GPIOA
	#define USART_RX_GPIO_CLK       RCC_AHB1Periph_GPIOA
	#define USART_RX_GPIO_PinSource GPIO_PinSource10
	
	#define GPIO_AF_USART           GPIO_AF_USART1	
	#define USART_IRQ_CHANNEL  		  USART1_IRQn
  #define USART_IRQHandler        USART1_IRQHandler
	
#elif defined COMM_USE_USART2
	#define COMM_USART              USART2
	
	#define USART_TX_PIN            GPIO_Pin_2
	#define USART_TX_GPIO_PORT      GPIOA
	#define USART_TX_GPIO_CLK       RCC_AHB1Periph_GPIOA
	#define USART_TX_GPIO_PinSource GPIO_PinSource2
	
	#define USART_RX_PIN            GPIO_Pin_3
	#define USART_RX_GPIO_PORT      GPIOA
	#define USART_RX_GPIO_CLK       RCC_AHB1Periph_GPIOA
	#define USART_RX_GPIO_PinSource GPIO_PinSource3

	#define GPIO_AF_USART           GPIO_AF_USART2		
	#define USART_IRQ_CHANNEL       USART2_IRQn	
  #define USART_IRQHandler        USART2_IRQHandler	
	
#endif	

#define EN_USART_RX 0
#define EN_USART_TX 0

#define DMA_USART_RX 1
#define DMA_USART_TX 1


//#define HALF_DUPLEX

#ifdef HALF_DUPLEX
	#define HALF_DUPLEX_PIN       GPIO_Pin_15
	#define HALF_DUPLEX_GPIO_PORT GPIOA
	#define HALF_DUPLEX_GPIO_CLK  RCC_AHB1Periph_GPIOA
	
	#define half_duplex_Write() GPIO_WriteBit(HALF_DUPLEX_GPIO_PORT,HALF_DUPLEX_PIN,Bit_SET)
	#define half_duplex_Read()  GPIO_WriteBit(HALF_DUPLEX_GPIO_PORT,HALF_DUPLEX_PIN,Bit_RESET)
#endif	

#if defined COMM_USE_USART2
//USART2 DMA
	#define USART_TX_DMA_Stream  DMA1_Channel7
	#define USART_RX_DMA_Stream  DMA1_Channel6

	#define USART_TX_DMA_IRQn  DMA1_Channel7_IRQn
	#define USART_RX_DMA_IRQn  DMA1_Channel6_IRQn

	#define USART_TX_DMA_FLAG_TC DMA1_FLAG_TC7
	#define USART_RX_DMA_FLAG_TC DMA1_FLAG_TC6

	#define USART_TX_DMA_IT_TC DMA1_IT_TC7
	#define USART_RX_DMA_IT_TC DMA1_IT_TC6

	#define USART_TX_DMA_IRQHandler     DMA1_Channel7_IRQHandler
	#define USART_RX_DMA_IRQHandler     DMA1_Channel6_IRQHandler

	#define USART_TX_cpar    (uint32_t)(&USART2->TDR) 
	#define USART_RX_cpar    (uint32_t)(&USART2->RDR) 
  
#elif defined COMM_USE_USART1

//USART1 DMA

	#define USART_DMA_CH         DMA_Channel_4
	#define USART_TX_DMA_Stream  DMA2_Stream7
	#define USART_RX_DMA_Stream  DMA2_Stream5

	#define USART_TX_DMA_IRQn  DMA2_Stream7_IRQn
	#define USART_RX_DMA_IRQn  DMA2_Stream5_IRQn

	#define USART_TX_DMA_FLAG_TC DMA_FLAG_TCIF7
	#define USART_RX_DMA_FLAG_TC DMA_FLAG_TCIF5

	#define USART_TX_DMA_IT_TC DMA_IT_TCIF7
	#define USART_RX_DMA_IT_TC DMA_IT_TCIF5

	#define USART_TX_DMA_IRQHandler     DMA2_Stream7_IRQHandler
	#define USART_RX_DMA_IRQHandler     DMA2_Stream5_IRQHandler

	#define USART_TX_cpar    (uint32_t)(&USART1->DR) 
	#define USART_RX_cpar    (uint32_t)(&USART1->DR)     
#else
#endif




#define RxFIFObufsize 1024
#define RxFIFOframesize 32


// **************************************************************************
// the typedefs		

//! \brief      Defines the usartdmafifo  object
//! \details    The controller usartdmafifo implements fifo circle queue           
//!

typedef struct _USARTDMA_FIFO_Obj_	
{
	uint16_t buffersize;//maxbuffersize 65535
	uint8_t  *pbuf;
	
	uint16_t framesize; //maxframesize
	uint16_t unreadframenum;
	uint16_t frameonhand;
	uint16_t frameindex;
	uint16_t *pframe;

	uint16_t overstockcnt;
}USARTDMA_FIFO_Obj;

//! \brief Defines the usartdmafifo handle
//!
typedef struct _USARTDMA_FIFO_Obj_ *USARTDMA_FIFO_Handle;
	
// **************************************************************************
// the globals declaration	

extern USARTDMA_FIFO_Handle USARTDMA_FIFOhandle_intr;

// **************************************************************************
// the function prototypes	
USARTDMA_FIFO_Handle USARTDMA_FIFO_init(void const*pMemory,const size_t numBytes);
void SetupUSARTDMA_FIFIO(USARTDMA_FIFO_Handle handle);
void USARTDMA_Config(uint32_t bound);
uint16_t ReadOVERSTOCK(USARTDMA_FIFO_Handle handle);
void USART_SendString(USART_TypeDef* USARTx,const char *str);
void USART_SendMultiData(USART_TypeDef* USARTx,uint8_t *str,uint16_t len);
void USART_DMA_Receive(u32 cmar,uint16_t cndtr);
bool USART_DMA_Send(u32 cmar,uint16_t cndtr);
uint16_t Deal_FIFO_data(USARTDMA_FIFO_Handle handle,uint8_t *pData,uint16_t maxlen);
#ifdef __cplusplus
}
#endif // extern "C"

//@} // ingroup
#endif // end of _USARTDMA_H_ definition	
