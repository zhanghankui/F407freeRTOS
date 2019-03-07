/*
*********************************************************************************************************
* @file    usartdma.c 
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
	
// **************************************************************************	
// the includes
#include "usartdma.h"
// **************************************************************************
// the defines


// **************************************************************************
// the typedefs	


// **************************************************************************
//declaration static function prototypes

static bool Is_DMA_USART_TX_busy = false;
//static bool Is_DMA_USART_RX_busy = false;
#ifdef HALF_DUPLEX	
static void switchdelay(void);
#endif
static void Handle_FIFO(void);
static void DMA_Config(void);
static void DMA_SetMemoryBaseAddr(DMA_Stream_TypeDef* DMAy_Streamx, u32 cmar);
static void DMA_Transfer_Config(DMA_Stream_TypeDef* DMAy_Streamx,u32 cmar,uint16_t cndtr);


// **************************************************************************
// the globals
uint8_t USARTDMA_FIFObuf[RxFIFObufsize];
uint16_t USARTDMA_FIFOframe[RxFIFOframesize];
USARTDMA_FIFO_Handle USARTDMA_FIFOhandle_intr;
// **************************************************************************
// the functions	

#if 0
//add those code,support printf function,and needn't choose use MicroLIB
#pragma import(__use_no_semihosting)
//support functions required by the standard library                
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//define _sys_exit() for avoid using semi-host mode
void _sys_exit(int x) 
{ 
	x = x; 
} 
//redefine fputc function to USARTx
int fputc(int ch, FILE *f)
{
#if defined COMM_USE_USART1		
	while((USART1->SR&0X40)==0);//cycle send,until send over  
    USART1->DR = (uint8_t) ch;    

#elif defined COMM_USE_USART2
	while((USART2->SR&0X40)==0);//cycle send,until send over  
    USART2->DR = (uint8_t) ch;  
#endif	
	return ch;
}
#endif 


#if 1
//use microLib method

//redefine fputc function to USARTx
int fputc(int ch, FILE *f)
{
#if defined COMM_USE_USART1	   	
	while((USART1->SR&0x40)==0)
	{}//cycle send,until send over
	USART1->DR = (uint8_t) ch;	
#elif defined COMM_USE_USART2	
	while((USART2->SR&0x40)==0)
	{}//cycle send,until send over
	USART2->DR = (uint8_t) ch;   	
#endif		
	return ch;
}

//redefine fgetc function to USARTx
int fgetc(FILE *f)
{
#if defined COMM_USE_USART1		
  while((USART1->SR&0x20)==0)
  {}
  return (int)USART1->DR;
#elif defined COMM_USE_USART2
  while((USART2->SR&0x20)==0)
  {}
  return (int)USART2->DR;
#endif		
}
#endif

USARTDMA_FIFO_Handle USARTDMA_FIFO_init(void const*pMemory,const size_t numBytes)
{
  USARTDMA_FIFO_Handle USARTDMA_FIFOHandle;


  if(numBytes < sizeof(USARTDMA_FIFO_Obj))
    return((USARTDMA_FIFO_Handle)NULL);

  // assign the handle
  USARTDMA_FIFOHandle = (USARTDMA_FIFO_Handle)pMemory;

  return(USARTDMA_FIFOHandle);
} // end of USARTDMA_FIFO_init() function

void MC_setupUSARTDMA_FIFIO(USARTDMA_FIFO_Handle handle)
{
	USARTDMA_FIFO_Obj *obj = (USARTDMA_FIFO_Obj *)handle;
	obj->buffersize     = RxFIFObufsize;
	obj->framesize      = RxFIFOframesize;
	obj->pbuf           = USARTDMA_FIFObuf;
	obj->pframe         = USARTDMA_FIFOframe;
	
	obj->frameonhand    = 0;
	obj->frameindex     = 0;
	obj->unreadframenum = 0;
	obj->overstockcnt   = 0;
}

void USARTDMA_Config(uint32_t bound)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef  GPIO_InitStructure;  

	DMA_Config();//DMA configuration
	
  /* Enable GPIO clock */	
  RCC_AHB1PeriphClockCmd(USART_TX_GPIO_CLK | USART_RX_GPIO_CLK,ENABLE); //enable usart pin GPIO clock
#ifdef HALF_DUPLEX
  RCC_AHB1PeriphClockCmd(HALF_DUPLEX_GPIO_CLK,ENABLE);//enable half duplex W/R pin clock
#endif
  /* Enable USARTx APB clock */ 
#if defined COMM_USE_USART1	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//enable USART1 clock
#elif defined COMM_USE_USART2
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,ENABLE);//enable USART2 clock
#endif
  /* USARTx Pins configuration ************************************************/ 
  /* Connect pin to Periph */
  GPIO_PinAFConfig(USART_TX_GPIO_PORT,USART_TX_GPIO_PinSource,GPIO_AF_USART); //alternate USARTx_TX
  GPIO_PinAFConfig(USART_RX_GPIO_PORT,USART_RX_GPIO_PinSource,GPIO_AF_USART); //alternate USARTx_RX
	
#ifdef HALF_DUPLEX
	/* Configure half duplex W/R pins */	
  GPIO_InitStructure.GPIO_Pin   = HALF_DUPLEX_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(HALF_DUPLEX_GPIO_PORT, &GPIO_InitStructure);	
	
	half_duplex_Read();	
#endif		
	
  /* Configure pins as AF pushpull */	
  GPIO_InitStructure.GPIO_Pin = USART_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//pullup
  GPIO_Init(USART_TX_GPIO_PORT, &GPIO_InitStructure); 
	
  GPIO_InitStructure.GPIO_Pin = USART_RX_PIN;	
  GPIO_Init(USART_RX_GPIO_PORT, &GPIO_InitStructure);	


  /* USARTx configured as follow:
  - BaudRate =  baud  
  - Word Length = 8 Bits
  - Stop Bit = 1 Stop Bit
  - Parity = No Parity
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled
  */ 	
	
  USART_DeInit(COMM_USART); 
  USART_InitStructure.USART_BaudRate = bound;//set baudrate
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;//set word length:8 bits
  USART_InitStructure.USART_StopBits = USART_StopBits_1;// 1 stop bit
  USART_InitStructure.USART_Parity = USART_Parity_No;//no parity
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//Hardware flow control disabled
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //Receive and transmit enabled
  USART_Init(COMM_USART, &USART_InitStructure); 

  //Usart NVIC configuration
  NVIC_InitTypeDef   NVIC_InitStructure;        
  NVIC_InitStructure.NVIC_IRQChannel = USART_IRQ_CHANNEL;//USARTx interrupt channel
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQ channel enable
  NVIC_Init(&NVIC_InitStructure);//initialize
	
#if EN_USART_RX        
	USART_ITConfig(COMM_USART, USART_IT_RXNE, ENABLE);//enable tx interrupt
#endif   

#if EN_USART_TX        
	USART_ITConfig(COMM_USART, USART_IT_TXE, ENABLE);//enable rx interrupt
#endif	

//auto baudrate identify code 
//  /* Configure the AutoBaudRate method */
//  USART_AutoBaudRateConfig(COMM_USART, USART_AutoBaudRate_StartBit);  
//  
//  /* Enable AutoBaudRate feature */
//  USART_AutoBaudRateCmd(COMM_USART, ENABLE);
//  
//  /* Wait until Receive enable acknowledge flag is set */
//  while(USART_GetFlagStatus(COMM_USART, USART_FLAG_REACK) == RESET)
//  {}  
//  
//  /* Wait until Transmit enable acknowledge flag is set */  
//  while(USART_GetFlagStatus(COMM_USART, USART_FLAG_TEACK) == RESET)
//  {}  
//  
//  /* Loop until the end of Autobaudrate phase */
//  while(USART_GetFlagStatus(COMM_USART, USART_FLAG_ABRF) == RESET)
//  {} 

//  USART_ITConfig(COMM_USART, USART_IT_IDLE, ENABLE);//enable receive idle interrupt
  /* USART enable */
  USART_Cmd(COMM_USART, ENABLE); 
  USART_ClearITPendingBit(COMM_USART,USART_IT_IDLE);  

#if DMA_USART_RX
   /* Enable USARTx DMA Rxrequest */  
  USART_DMACmd(COMM_USART, USART_DMAReq_Rx, ENABLE);
#endif
#if DMA_USART_TX  
   /* Enable USARTx DMA Txrequest */  
  USART_DMACmd(COMM_USART, USART_DMAReq_Tx, ENABLE);  
#endif 

/* clear send completion flag, prevent for first sending byte abnormal */
  USART_ClearFlag(COMM_USART, USART_FLAG_TC);	
}

static void DMA_Config(void)
{
  DMA_InitTypeDef     DMA_InitStruct;
  NVIC_InitTypeDef    NVIC_InitStructure;	
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

#ifdef DMA_USART_TX
	DMA_DeInit(USART_TX_DMA_Stream);
  DMA_InitStruct.DMA_Channel = USART_DMA_CH;	
	DMA_InitStruct.DMA_PeripheralBaseAddr   = USART_TX_cpar;//DMA peripheral base address
//	DMA_InitStruct.DMA_Memory0BaseAddr;                   //DMA memory base daddress
	DMA_InitStruct.DMA_DIR                  = DMA_DIR_MemoryToPeripheral;//data transfer direction,Read from memory
//	DMA_InitStruct.DMA_BufferSize           //DMA channel buffer size
	DMA_InitStruct.DMA_PeripheralInc        = DMA_PeripheralInc_Disable;   //peripheral increment mode disabled
	DMA_InitStruct.DMA_MemoryInc            = DMA_MemoryInc_Enable;        //memory increment mode enabled
	DMA_InitStruct.DMA_PeripheralDataSize   = DMA_PeripheralDataSize_Byte; //peripheral size 8-bits
	DMA_InitStruct.DMA_MemoryDataSize       = DMA_MemoryDataSize_Byte;     //memory size 8-bits
	DMA_InitStruct.DMA_Mode                 = DMA_Mode_Normal;             //circular mode disabled
	DMA_InitStruct.DMA_Priority             = DMA_Priority_Medium;         //dma channel priority level medium

  DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
  DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
	DMA_Init(USART_TX_DMA_Stream,&DMA_InitStruct);	
	
	//interrupt configuration
	NVIC_InitStructure.NVIC_IRQChannel                     = USART_TX_DMA_IRQn;  //interrupt channel
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority   = 4; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority          = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd                  = ENABLE;
	NVIC_Init(&NVIC_InitStructure);   		
	
	DMA_ITConfig(USART_TX_DMA_Stream,DMA_IT_TC,ENABLE);    //enables the specified DMAy Channelx interrupts
	DMA_ClearFlag(USART_TX_DMA_Stream,USART_TX_DMA_FLAG_TC);                       //clear flag
//enable dma channel start transfer
//	DMA_Cmd(USART_TX_DMA_Stream,ENABLE);
#endif	

#ifdef DMA_USART_RX
	DMA_DeInit(USART_RX_DMA_Stream);
	DMA_InitStruct.DMA_Channel = USART_DMA_CH;	
	DMA_InitStruct.DMA_PeripheralBaseAddr   = USART_RX_cpar; //DMA peripheral base address
//	DMA_InitStruct.DMA_Memory0BaseAddr;                    //DMA memory base daddress
	DMA_InitStruct.DMA_DIR                  = DMA_DIR_PeripheralToMemory;//data transfer direction,Read from peripheral
//	DMA_InitStruct.DMA_BufferSize                         //DMA channel buffer size
	DMA_InitStruct.DMA_PeripheralInc        = DMA_PeripheralInc_Disable;   //peripheral increment mode disabled
	DMA_InitStruct.DMA_MemoryInc            = DMA_MemoryInc_Enable;        //memory increment mode enabled
	DMA_InitStruct.DMA_PeripheralDataSize   = DMA_PeripheralDataSize_Byte; //peripheral size 8-bits
	DMA_InitStruct.DMA_MemoryDataSize       = DMA_MemoryDataSize_Byte;     //memory size 8-bits
	DMA_InitStruct.DMA_Mode                 = DMA_Mode_Circular;           //circular mode enabled, circle FIFO
	DMA_InitStruct.DMA_Priority             = DMA_Priority_High;           //dma channel priority level high

  DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
  DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
	
	DMA_Init(USART_RX_DMA_Stream,&DMA_InitStruct);	 

	//中断配置
//		NVIC_InitStructure.NVIC_IRQChannel                     = USART_RX_DMA_IRQn;  //中断通道
//		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority   = 4;     
//		NVIC_InitStructure.NVIC_IRQChannelSubPriority          = 0;		
//		NVIC_InitStructure.NVIC_IRQChannelCmd                  = ENABLE;             //使能中断
//		NVIC_Init(&NVIC_InitStructure);   		

	DMA_ITConfig(USART_RX_DMA_Stream,DMA_IT_TC,DISABLE);    //disables the specified DMAy Channelx interrupts
	DMA_ClearFlag(USART_RX_DMA_Stream,USART_RX_DMA_FLAG_TC);                //clear flag
//enable dma channel start transfer
//	DMA_Cmd(USART_RX_DMA_Stream,ENABLE);   //开启一次传输
#endif
}



/* USART send, for string */
void USART_SendString(USART_TypeDef* USARTx,const char *str)
{
  uint16_t len;
  uint8_t  i;
  len = strlen(str);
#ifdef HALF_DUPLEX	
  half_duplex_Write();
	switchdelay();	
#endif
  for(i=0;i<len;i++)
  {
    USART_SendData(USARTx,str[i]);
    while((USART_GetFlagStatus(USARTx,USART_FLAG_TXE))==RESET);     
  } 
	while((USART_GetFlagStatus(USARTx,USART_FLAG_TC))==RESET){};	
#ifdef HALF_DUPLEX
	switchdelay();			
  half_duplex_Read();
#endif	
}

//USART send, for array
void USART_SendMultiData(USART_TypeDef* USARTx,uint8_t *str,uint16_t len)
{
  uint16_t i;
#ifdef HALF_DUPLEX	
  half_duplex_Write();
	switchdelay();		
#endif
  for(i=0;i<len;i++)
  {
    USART_SendData(USARTx,str[i]);
    while((USART_GetFlagStatus(USARTx,USART_FLAG_TXE))==RESET); //reset means data is not transferred to the shift register   
  }
	while((USART_GetFlagStatus(USARTx,USART_FLAG_TC))==RESET){};//reset means transmission is not complete
#ifdef HALF_DUPLEX	
	switchdelay();			
  half_duplex_Read();
#endif	
}

static void Handle_FIFO(void)
{
	uint16_t NDTR = USART_RX_DMA_Stream->NDTR;
	if(USARTDMA_FIFOhandle_intr->buffersize>=NDTR)
	{
		USARTDMA_FIFOhandle_intr->unreadframenum++;
		USARTDMA_FIFOhandle_intr->frameindex++;
		if(USARTDMA_FIFOhandle_intr->frameindex == USARTDMA_FIFOhandle_intr->framesize)
		{
			USARTDMA_FIFOhandle_intr->frameindex = 0;
		}
		USARTDMA_FIFOhandle_intr->pframe[USARTDMA_FIFOhandle_intr->frameindex] = 
			USARTDMA_FIFOhandle_intr->buffersize - NDTR;
		
		//system can't deal too many data,overstock
		if(USARTDMA_FIFOhandle_intr->unreadframenum > USARTDMA_FIFOhandle_intr->framesize)
		{
			
			USARTDMA_FIFOhandle_intr->frameonhand    = 0;
			USARTDMA_FIFOhandle_intr->frameindex     = 0;
			USARTDMA_FIFOhandle_intr->unreadframenum = 0;
			USARTDMA_FIFOhandle_intr->overstockcnt++;		
		}
	}
}

//maxlen加入，防止边界溢出
uint16_t Deal_FIFO_data(USARTDMA_FIFO_Handle handle,uint8_t *pData,uint16_t maxlen)
{

	uint16_t len = 0;
	uint16_t frametail;
	uint16_t i,temp;
	USARTDMA_FIFO_Obj *obj = (USARTDMA_FIFO_Obj *)handle;
	
	if(obj->unreadframenum)
	{
		obj->unreadframenum --;
		if(obj->frameonhand == (obj->framesize - 1))
		{
			frametail = 0;
		}
		else
		{
			frametail = obj->frameonhand + 1;
		}

		if(obj->pframe[obj->frameonhand]>obj->pframe[frametail])
		{

			len = obj->pframe[frametail] + obj->buffersize - obj->pframe[obj->frameonhand];	
			if(len>maxlen)
			{
				len = 0;
			}
			for(i=0;i<len;i++)
			{
				temp = (obj->pframe[obj->frameonhand]+i)%obj->buffersize;
				pData[i]=obj->pbuf[temp];
			}
		}
		else
		{
			len = obj->pframe[frametail]-obj->pframe[obj->frameonhand];
			if(len>maxlen)
			{
				len = 0;
			}
			for(i=0;i<len;i++)
			{
				pData[i]=obj->pbuf[obj->pframe[obj->frameonhand]+i];
			}   		
		}

		obj->frameonhand++;
		if(obj->frameonhand == obj->framesize)
		{
			obj->frameonhand = 0;
		}
	}
	return len;
}

uint16_t ReadOVERSTOCK(USARTDMA_FIFO_Handle handle)
{
	USARTDMA_FIFO_Obj *obj = (USARTDMA_FIFO_Obj *)handle;	
	return obj->overstockcnt;
}


/**
  * @brief  Sets the MemoryBaseAddr in the current DMAy Channelx transfer.
  * @param  DMAy_Channelx: where y can be 1 or 2 to select the DMA and 
  *         x can be 1 to 7 for DMA1 and 1 to 5 for DMA2 to select the DMA Channel.
  * @param  cmar: The MemoryBaseAddr in the current DMAy Channelx transfer.
  * @note   This function can only be used when the DMAy_Channelx is disabled.                 
  * @retval None.
  */
static void DMA_SetMemoryBaseAddr(DMA_Stream_TypeDef* DMAy_Streamx, u32 cmar)
{
  /* Check the parameters */
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Streamx));
  
/*--------------------------- DMAy Channelx CMAR Configuration ---------------*/
  /* Write to DMAy Channelx CMAR */
  DMAy_Streamx->M0AR = cmar;  
}

//modify DMA configuration parameter
//DMAy_CHx : DMA channel CHx
//cmar: memory address
//cndtr:data transfer number
static void DMA_Transfer_Config(DMA_Stream_TypeDef* DMAy_Streamx,u32 mar,uint16_t ndtr)
{ 
	DMA_Cmd(DMAy_Streamx, DISABLE);            //disable DMAy_CHx channel
	DMA_SetMemoryBaseAddr(DMAy_Streamx,mar);	//set the memorybaseaddr
 	DMA_SetCurrDataCounter(DMAy_Streamx,ndtr); //dma channel buffer size
 	DMA_Cmd(DMAy_Streamx, ENABLE);              //enable DMAy_CHx channel
}	 

bool USART_DMA_Send(u32 cmar,uint16_t cndtr)
{
	if(Is_DMA_USART_TX_busy)
	{
		return true;// dma tx channel is busy
	}
	else
	{
#ifdef HALF_DUPLEX
		half_duplex_Write();
		switchdelay();			
#endif
		Is_DMA_USART_TX_busy = true;
		DMA_Transfer_Config(USART_TX_DMA_Stream,cmar,cndtr);
		return false; //send success
	}
}

void USART_DMA_Receive(u32 cmar,uint16_t cndtr)
{
//circle mode
    DMA_Transfer_Config(USART_RX_DMA_Stream,cmar,cndtr);  
}



/**
  * @brief  This function handles USART global interrupt request.
  * @param  None
  * @retval None
**/

void USART_IRQHandler(void)
{
#if	EN_USART_RX
  if(USART_GetITStatus(COMM_USART, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register */
    //= USART_ReceiveData(COMM_USART);
  }  
#endif   
	//总线空闲中断是在检测到接收数据后，数据总线上一个字节的时间内，没有再接到数据发生。也就是RXNE位被置位后，才开始检测，只被置位一次，除非再次检测到到RXNE被置位，然后才开始检测下一次的总线空闲
	//principle：
	//while(1)
	//{
	//if(RXNE == 1)
	//{
	//	start IDLE detect;
	//	wait 1 bits；
	//}
	//  if（RXNE == 0）
	//			IDLE = 1；
//  }

   if(USART_GetITStatus(COMM_USART, USART_IT_IDLE) != RESET) //Receive data register not empty flag, detect bus idle
	 {
		 
		 #ifdef DMA_USART_RX
     Handle_FIFO();
		 #endif
     USART_ClearITPendingBit(COMM_USART,USART_IT_IDLE);
	 } 
}

void USART_TX_DMA_IRQHandler(void)
{
	if(DMA_GetITStatus(USART_TX_DMA_Stream,USART_TX_DMA_IT_TC)!=RESET)//channel transfer complished
	{
		while((USART_GetFlagStatus(COMM_USART,USART_FLAG_TC))==RESET){};//usart tx send complished
		DMA_ClearITPendingBit(USART_TX_DMA_Stream,USART_TX_DMA_IT_TC);//clear channel transfer complish flag
#ifdef HALF_DUPLEX	
		switchdelay();			
		half_duplex_Read();
#endif	
		Is_DMA_USART_TX_busy = false;
	}	
}

void USART_RX_DMA_IRQHandler(void)
{
 	if(DMA_GetITStatus(USART_RX_DMA_Stream,USART_RX_DMA_IT_TC)!=RESET)//channel transfer complished
	{
		DMA_ClearITPendingBit(USART_RX_DMA_Stream,USART_RX_DMA_IT_TC);//clear channel transfer complish flag
//		Is_DMA_USART_RX_busy = false;
	} 	
}



#ifdef HALF_DUPLEX	
static void switchdelay(void)
{
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();	
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();	
}
#endif	






