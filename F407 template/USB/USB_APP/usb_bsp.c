#include "usb_bsp.h" 
#include "sys.h" 
#include "bsp.h"

//#include "usart.h" 
//////////////////////////////////////////////////////////////////////////////////	 
 
//USB-BSP ����	   
//STM32F4����ģ��-�⺯���汾
//�Ա����̣�http://mcudev.taobao.com								  
//*******************************************************************************
//�޸���Ϣ
//��
////////////////////////////////////////////////////////////////////////////////// 	   

//USB������Դ���ƿ�
#define USB_HOST_PWRCTRL 	PAout(15)	//PA15

extern USB_OTG_CORE_HANDLE  USB_OTG_dev; /* ������������usb_usr.c�ļ����� */
extern USB_OTG_CORE_HANDLE  USB_OTG_Core;
   
//USB OTG �ײ�IO��ʼ��
//pdev:USB OTG�ں˽ṹ��ָ��
void USB_OTG_BSP_Init(USB_OTG_CORE_HANDLE *pdev)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ��ص�GPIOȫ���л�������ģʽ */
	{
		RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}

	/* USB Host */
	if (pdev == &USB_OTG_Core)
	{
		/* ������STM32-V5������ֻʹ��2���� PB14/OTG_HS_DM �� PB15/OTG_HS_DP ��U�� */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;

		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_OTG2_FS) ;
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_OTG2_FS) ;

		RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_OTG_HS, ENABLE) ;
	}

	/* USB Device */
	if (pdev == &USB_OTG_dev)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;

		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_OTG1_FS);
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_OTG1_FS);

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
		RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS, ENABLE);
	}
}
//USB OTG �ж�����,����USB FS�ж�
//pdev:USB OTG�ں˽ṹ��ָ��
void USB_OTG_BSP_EnableInterrupt(USB_OTG_CORE_HANDLE *pdev)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* �ȹر����к�USB��ص��ж� */
	USB_OTG_BSP_DisableInterrupt();

	/* USB Host */
	if (pdev == &USB_OTG_Core)
	{
		NVIC_InitStructure.NVIC_IRQChannel = OTG_HS_IRQn;	/* USE_USB_OTG_HS */
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		/* ʹ�� OTG_HS_IRQn */
		NVIC_Init(&NVIC_InitStructure);
	}

	/* USB Device */
	if (pdev == &USB_OTG_dev)
	{
		NVIC_InitStructure.NVIC_IRQChannel = OTG_FS_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED
			NVIC_InitStructure.NVIC_IRQChannel = OTG_HS_EP1_OUT_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);

			NVIC_InitStructure.NVIC_IRQChannel = OTG_HS_EP1_IN_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
		#endif
	}
}
//USB OTG �ж�����,����USB FS�ж�
//pdev:USB OTG�ں˽ṹ��ָ��
void USB_OTG_BSP_DisableInterrupt(void)
{ 
	NVIC_InitTypeDef NVIC_InitStructure;

	/* �ȹر����к�USB��ص��ж� */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;

	NVIC_InitStructure.NVIC_IRQChannel = OTG_FS_IRQn;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = OTG_HS_IRQn;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = OTG_HS_EP1_OUT_IRQn;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = OTG_HS_EP1_IN_IRQn;
	NVIC_Init(&NVIC_InitStructure);	
}
//USB OTG �˿ڹ�������(������δ�õ�)
//pdev:USB OTG�ں˽ṹ��ָ��
//state:0,�ϵ�;1,�ϵ�
void USB_OTG_BSP_DriveVBUS(USB_OTG_CORE_HANDLE *pdev, uint8_t state)
{ 
}
//USB_OTG �˿ڹ���IO����(������δ�õ�)
//pdev:USB OTG�ں˽ṹ��ָ��
void  USB_OTG_BSP_ConfigVBUS(USB_OTG_CORE_HANDLE *pdev)
{ 
} 
//USB_OTG us����ʱ����
//�����̲���SYSTEM�ļ��е�delay.c�����delay_us����ʵ��
//�ٷ����̲��õ��Ƕ�ʱ��2��ʵ�ֵ�.
//usec:Ҫ��ʱ��us��.
void USB_OTG_BSP_uDelay (const uint32_t usec)
{ 
	bsp_DelayUS(usec);
}
//USB_OTG ms����ʱ����
//�����̲���SYSTEM�ļ��е�delay.c�����delay_ms����ʵ��
//�ٷ����̲��õ��Ƕ�ʱ��2��ʵ�ֵ�.
//msec:Ҫ��ʱ��ms��.
void USB_OTG_BSP_mDelay (const uint32_t msec)
{  
	bsp_DelayMS(msec);
}
   




