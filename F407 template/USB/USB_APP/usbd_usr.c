#include "usbd_usr.h" 
#include "usb_dcd_int.h"
#include <stdio.h> 
#include "usbd_msc_core.h"
#include "usbd_desc.h"
#include "usb_bsp.h"
//#include <usart.h> 
//////////////////////////////////////////////////////////////////////////////////	 
 
//USBD-USR ����	   
//STM32F4����ģ��-�⺯���汾
//�Ա����̣�http://mcudev.taobao.com							  
//*******************************************************************************
//�޸���Ϣ
//��
////////////////////////////////////////////////////////////////////////////////// 	   

/* ��ӱ������� */
__ALIGN_BEGIN  USB_OTG_CORE_HANDLE  USB_OTG_dev   __ALIGN_END;
__ALIGN_BEGIN  USB_OTG_CORE_HANDLE  USB_OTG_Core  __ALIGN_END;





//��ʾUSB����״̬
//0,û������;
//1,�Ѿ�����;
vu8 bDeviceState=0;		//Ĭ��û������  

#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED
	extern uint32_t USBD_OTG_EP1IN_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);
	extern uint32_t USBD_OTG_EP1OUT_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);
#endif

/**
  * @brief  This function handles OTG_HS Handler.
  * @param  None
  * @retval None
  */
#ifdef USE_USB_OTG_HS
	void OTG_HS_IRQHandler(void)
	{
		USBH_OTG_ISR_Handler (&USB_OTG_Core);	
	}
#endif

//USB OTG �жϷ�����
//��������USB�ж�
#ifdef USE_USB_OTG_FS
	void OTG_FS_IRQHandler(void)
	{
		USBD_OTG_ISR_Handler (&USB_OTG_dev);
	}
#endif

#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED
	/**
	  * @brief  This function handles EP1_IN Handler.
	  * @param  None
	  * @retval None
	  */
	void OTG_HS_EP1_IN_IRQHandler(void)
	{
		USBD_OTG_EP1IN_ISR_Handler (&USB_OTG_dev);	
	}

	/**
	  * @brief  This function handles EP1_OUT Handler.
	  * @param  None
	  * @retval None
	  */
	void OTG_HS_EP1_OUT_IRQHandler(void)
	{
		USBD_OTG_EP1OUT_ISR_Handler (&USB_OTG_dev);
	}
#endif
//ָ��DEVICE_PROP�ṹ��
//USB Device �û��ص�����. 
USBD_Usr_cb_TypeDef USR_cb =
{
  USBD_USR_Init,
  USBD_USR_DeviceReset,
  USBD_USR_DeviceConfigured,
  USBD_USR_DeviceSuspended,
  USBD_USR_DeviceResumed,
	
  USBD_USR_DeviceConnected,
  USBD_USR_DeviceDisconnected,    
};


#define USER_INFORMATION1  (uint8_t*)"INFO : double Lun configuration"
#define USER_INFORMATION2  (uint8_t*)"INFO : microSD is used"

/* ʹ��U���豸 ****************************/
void usbd_OpenMassStorage(void)
{
	USBD_Init(&USB_OTG_dev,
			USB_OTG_FS_CORE_ID,
			&USR_desc,
			&USBD_MSC_cb,
			&USR_cb);
	printf("USB ready\r\n\r\n");		//	
}

/* �Ƴ�U���豸 ***************************/
void usbd_CloseMassStorage(void)
{
	DCD_DevDisconnect(&USB_OTG_dev);

	USB_OTG_BSP_DisableInterrupt();	
}



//USB Device �û��Զ����ʼ������
void USBD_USR_Init(void)
{
	//printf("USBD_USR_Init\r\n");
} 
//USB Device ��λ
//speed:USB�ٶ�,0,����;1,ȫ��;����,����.
void USBD_USR_DeviceReset (uint8_t speed)
{
	switch (speed)
	{
		case USB_OTG_SPEED_HIGH:
			printf("USB Device Library v1.1.0  [HS]\r\n");
			break; 
		case USB_OTG_SPEED_FULL: 
			printf("USB Device Library v1.1.0  [FS]\r\n");
			break;
		default:
			printf("USB Device Library v1.1.0  [??]\r\n"); 
			break;
	}
}
//USB Device ���óɹ�
void USBD_USR_DeviceConfigured (void)
{
	printf("MSC Interface started.\r\n"); 
} 
//USB Device����
void USBD_USR_DeviceSuspended(void)
{
	printf("Device In suspend mode.\r\n");
} 
//USB Device�ָ�
void USBD_USR_DeviceResumed(void)
{ 
	printf("Device Resumed\r\n");
}
//USB Device���ӳɹ�
void USBD_USR_DeviceConnected (void)
{
	bDeviceState=1;
	printf("USB Device Connected.\r\n");
}
//USB Deviceδ����
void USBD_USR_DeviceDisconnected (void)
{
	bDeviceState=0;
	printf("USB Device Disconnected.\r\n");
} 


















