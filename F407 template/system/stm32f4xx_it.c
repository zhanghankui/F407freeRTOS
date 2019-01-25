/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
	SVC_Handler PendSV_Handler SysTick_Handler 由操作系统实现
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"


/* Scheduler includes */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "canfestival.h"
#include "can_STM32.h"

extern xQueueHandle xQ_CAN_MSG;

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
//void SVC_Handler(void)
//{
//}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//void SysTick_Handler(void)
//{
//}

/**
  * @brief  This function handles CAN1 RX0 request.
  * @param  None
  * @retval None
  * *************CAN1 RX0 中断********************
  */


void CAN1_RX0_IRQHandler(void)
{  
  CANOpen_Message CAN1_Rx_m;
  portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;     //中断中唤醒新任务
  //printf("G\r\n");
  taskENTER_CRITICAL();                                 //进入中断
  CAN_Receive(CAN1, CAN_FIFO0, &(CAN1_Rx_m.m));	        //从CAN1 FIFO0接收数据
  CAN1_Rx_m.CANx = 1;
  if(NULL != xQ_CAN_MSG)         // 向队列发送数据包
  {
		xQueueSendFromISR( xQ_CAN_MSG, &CAN1_Rx_m, &xHigherPriorityTaskWoken );
  }
  taskEXIT_CRITICAL();                                 //退出临界区
  if( xHigherPriorityTaskWoken != pdFALSE )
  {
    portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
  }
}
/**
  * @}
  */ 
/**
  * @brief  This function handles CAN2 RX0 request.
  * @param  None
  * @retval None
  */

 /*
   在数据量不大的情况下，中断函数中处理数据没问题，若数据量
   较大，则需要建立缓冲区，防止数据被覆盖
   */
void CAN2_RX0_IRQHandler(void)
{
	  CANOpen_Message CAN2_Rx_m;
	  portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;     //中断中唤醒新任务
	  taskENTER_CRITICAL();                                  //进入中断
	  CAN_Receive(CAN2, CAN_FIFO0, &(CAN2_Rx_m.m));	    //从CAN2 FIFO0接收数据
	  CAN2_Rx_m.CANx = 1;
	  if(NULL != xQ_CAN_MSG)         // 向队列发送数据包
	  {
			xQueueSendFromISR( xQ_CAN_MSG, &CAN2_Rx_m, &xHigherPriorityTaskWoken );
	  }
	  taskEXIT_CRITICAL();
	  if( xHigherPriorityTaskWoken != pdFALSE )
	  {
	    portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );           //强制上下文切换
  	  }  
}


/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
