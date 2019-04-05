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

//#define DEBUG_WAR_CONSOLE_ON
//#define DEBUG_ERR_CONSOLE_ON
#include "stm32f4xx_can.h"
#include "can_STM32.h"
#include "canfestival.h"
#include "misc.h"


CAN_InitTypeDef        CAN_InitStructure;
CAN_FilterInitTypeDef  CAN_FilterInitStructure;

void can_irq_handler(void);

unsigned char canInit(CAN_TypeDef* CANx,unsigned int bitrate)
/******************************************************************************
Initialize the hardware to receive CAN messages and start the timer for the
CANopen stack.
INPUT	
OUTPUT	
******************************************************************************/
{
	  GPIO_InitTypeDef  GPIO_InitStructure;
	  NVIC_InitTypeDef NVIC_InitStructure;	          //NVIC �ṹ
 /* CAN GPIOs configuration **************************************************/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;                // ��ռʽ���ȼ�Ϊ1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  if(CANx == CAN1)
  {
		  /* Enable GPIO clock */
		  /*ʹ��CANʹ�õ�IO��ʱ�ӣ�IO����config.h�ж���*/
		  RCC_AHB1PeriphClockCmd(CAN1_GPIO_CLK, ENABLE);
		
		  /* Connect CAN pins to AF9 */
		  GPIO_PinAFConfig(CAN1_GPIO_PORT, CAN1_RX_SOURCE, CAN1_AF_PORT);
		  GPIO_PinAFConfig(CAN1_GPIO_PORT, CAN1_TX_SOURCE, CAN1_AF_PORT); 
		  
		  /* Configure CAN RX and TX pins */
		  GPIO_InitStructure.GPIO_Pin = CAN1_RX_PIN | CAN1_TX_PIN;
		  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
		  GPIO_Init(CAN1_GPIO_PORT, &GPIO_InitStructure);
		
		  /* CAN configuration ********************************************************/  
		  /* Enable CAN clock */
		  /*ʹ��CAN1ʱ��*/
		  RCC_APB1PeriphClockCmd(CAN1_CLK, ENABLE);
		  
		  /* CAN register init */
		  CAN_DeInit(CANx);
		  CAN_StructInit(&CAN_InitStructure);
		
		  /* CAN cell init */
		  CAN_InitStructure.CAN_TTCM = DISABLE;     //��ֹʱ�䴥��ͨ��ģʽ
		  CAN_InitStructure.CAN_ABOM = ENABLE;     //�Զ����߹���ʹ��
		  CAN_InitStructure.CAN_AWUM = DISABLE;		//�Զ�����ģʽ��ʹ��
		  CAN_InitStructure.CAN_NART = DISABLE;		//���Զ��ش�ģʽ��ʹ�ܣ����Զ��ش�ֱ���ɹ�
		  CAN_InitStructure.CAN_RFLM = DISABLE;		//FIFO����ģʽ��ʹ�ܣ����ʱ�ɱ��ĸ����±���
		  CAN_InitStructure.CAN_TXFP = DISABLE;		//FIFO���ȼ�ȷ�������ȼ��ɱ��ı�ʶ��ȷ��
		  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;	 //CAN����������ģʽ

		    
		  /* CAN Baudrate */
		  /* CAN ʹ��APB1ʱ�ӣ�APB1ʱ�ӷ�Ƶϵ����4��,42M����system_stm32f2xx.c   PCLK1 = HCLK / 4      (APB1Periph)   */
		  /* CAN Ƶ�� = APB1ʱ��/(CAN��Ƶϵ��*(CAN_SJW+CAN_BS1+CAN_BS2))*/
		  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;  //����ͬ����Ծ���1��ʱ�䵥λ			
		  CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
		  CAN_InitStructure.CAN_BS2 = CAN_BS2_7tq;
		  CAN_InitStructure.CAN_Prescaler = bitrate;    //Ԥ��Ƶϵ��
		  CAN_Init(CANx, &CAN_InitStructure);
		
		  CAN_FilterInitStructure.CAN_FilterNumber = 0;	  //ָ���˴���ʼ���Ĺ�����0

		 // CAN_FilterInitStructure.CAN_FilterNumber = 14;

		  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;    //ָ���˹�����������ʼ������ģʽΪ��ʶ������λģʽ
		  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
		  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
		  CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
		  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
		  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
		  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;            //�趨��ָ���������FIFO0
		  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;           //ʹ�ܹ�����
		  CAN_FilterInit(&CAN_FilterInitStructure);
		  
		  /* Transmit Structure preparation */
//		  TxMessage.StdId = 0x321;
//		  TxMessage.ExtId = 0x01;
//		  TxMessage.RTR = CAN_RTR_DATA;
//		  TxMessage.IDE = CAN_ID_STD;                            //CAN 2.0A
//		  TxMessage.DLC = 1;
		  
		  /* Enable FIFO 0 message pending Interrupt */
		  CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);	              //��FIFO 0 �ж�����

		  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;	  //����NVIC
		  NVIC_Init(&NVIC_InitStructure);                         //��CAN1�ж�

  }
  else
  {
		  /* Enable GPIO clock */
		  /*ʹ��CANʹ�õ�IO��ʱ�ӣ�IO����config.h�ж���*/
		  RCC_AHB1PeriphClockCmd(CAN2_GPIO_CLK, ENABLE);
		
		  /* Connect CAN pins to AF9 */
		  GPIO_PinAFConfig(CAN2_GPIO_PORT, CAN2_RX_SOURCE, CAN2_AF_PORT);
		  GPIO_PinAFConfig(CAN2_GPIO_PORT, CAN2_TX_SOURCE, CAN2_AF_PORT); 
		  
		  /* Configure CAN RX and TX pins */
		  GPIO_InitStructure.GPIO_Pin = CAN2_RX_PIN | CAN2_TX_PIN;
		  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
		  GPIO_Init(CAN2_GPIO_PORT, &GPIO_InitStructure);
		
		  /* CAN configuration ********************************************************/  
		  /* Enable CAN clock */
		  /*ʹ��CAN1ʱ��*/
		  RCC_APB1PeriphClockCmd(CAN2_CLK, ENABLE);
		  
		  /* CAN register init */
		  CAN_DeInit(CANx);
		  CAN_StructInit(&CAN_InitStructure);
		
		  /* CAN cell init */
		  CAN_InitStructure.CAN_TTCM = DISABLE;     //��ֹʱ�䴥��ͨ��ģʽ
		  CAN_InitStructure.CAN_ABOM = DISABLE;     //�Զ����߹���ʹ��
		  CAN_InitStructure.CAN_AWUM = DISABLE;		//�Զ�����ģʽ��ʹ��
		  CAN_InitStructure.CAN_NART = DISABLE;		//���Զ��ش�ģʽ��ʹ�ܣ����Զ��ش�ֱ���ɹ�
		  CAN_InitStructure.CAN_RFLM = DISABLE;		//FIFO����ģʽ��ʹ�ܣ����ʱ�ɱ��ĸ����±���
		  CAN_InitStructure.CAN_TXFP = DISABLE;		//FIFO���ȼ�ȷ�������ȼ��ɱ��ı�ʶ��ȷ��
		  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;	 //CAN����������ģʽ
		    
		  /* CAN Baudrate */
		  /* CAN ʹ��APB1ʱ�ӣ�APB1ʱ�ӷ�Ƶϵ����4��,42M����system_stm32f2xx.c   PCLK1 = HCLK / 4      (APB1Periph)   */
		  /* CAN Ƶ�� = APB1ʱ��/(CAN��Ƶϵ��*(CAN_SJW+CAN_BS1+CAN_BS2))*/
		  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;  //����ͬ����Ծ���1��ʱ�䵥λ			
		  CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
		  CAN_InitStructure.CAN_BS2 = CAN_BS2_7tq;
		  CAN_InitStructure.CAN_Prescaler = bitrate;    //Ԥ��Ƶϵ��
		  CAN_Init(CANx, &CAN_InitStructure);
		

		  CAN_SlaveStartBank(14);                              //���ô�CAN����ʼFilter CAN2
		  CAN_FilterInitStructure.CAN_FilterNumber = 14;	  //ָ���˴���ʼ���Ĺ�����14

		 // CAN_FilterInitStructure.CAN_FilterNumber = 14;
		  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;    //ָ���˹�����������ʼ������ģʽΪ��ʶ������λģʽ
		  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
		  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
		  CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
		  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
		  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
		  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;            //�趨��ָ���������FIFO0
		  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;           //ʹ�ܹ�����

		  CAN_FilterInit(&CAN_FilterInitStructure);
		  
		  /* Transmit Structure preparation */
//		  TxMessage.StdId = 0x321;
//		  TxMessage.ExtId = 0x01;
//		  TxMessage.RTR = CAN_RTR_DATA;
//		  TxMessage.IDE = CAN_ID_STD;                            //CAN 2.0A
//		  TxMessage.DLC = 1;
//        CAN_Transmit(CANx, &TxMessage);
		  
		  /* Enable FIFO 0 message pending Interrupt */
		  CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);	              //��FIFO 0 �ж�����
		  NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;	  //����NVIC
		  NVIC_Init(&NVIC_InitStructure);                         //��CAN2�ж�	
  }
  return 1;

}

unsigned char canSend(CAN_PORT CANx, Message *m)	                 //CAN����
/******************************************************************************
The driver send a CAN message passed from the CANopen stack
INPUT	CAN_PORT is not used (only 1 avaiable)
	Message *m pointer to message to send
OUTPUT	1 if  hardware -> CAN frame
******************************************************************************/
{
	unsigned char ret;
	unsigned char i;
	CanTxMsg TxMessage;
	TxMessage.StdId = (uint32_t)(m->cob_id);
	TxMessage.ExtId = 0x00;
	TxMessage.RTR = m->rtr;								   //�Ƿ�ΪԶ��֡
	TxMessage.IDE = CAN_ID_STD;                            //CAN 2.0A ����B�����
	TxMessage.DLC = m->len;                                //���ݳ���
	for(i=0;i<m->len;i++)                                  //Ϊ���ݸ�ֵ
	{
		TxMessage.Data[i] = m->data[i];
	}
    ret = CAN_Transmit(CANx, &TxMessage);
	if(ret == CAN_TxStatus_NoMailBox)
		return 0xFF;			   //����ʧ�ܣ�����������
	else 
	    return 0;              //���ͳɹ�
}
