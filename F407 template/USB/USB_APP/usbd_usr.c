#include "usbd_usr.h" 
#include "usb_dcd_int.h"
#include <stdio.h> 
#include "usbd_msc_core.h"
#include "usbd_desc.h"
#include "usb_bsp.h"
//#include <usart.h> 
//////////////////////////////////////////////////////////////////////////////////	 
 
//USBD-USR 代码	   
//STM32F4工程模板-库函数版本
//淘宝店铺：http://mcudev.taobao.com							  
//*******************************************************************************
//修改信息
//无
////////////////////////////////////////////////////////////////////////////////// 	   

/* 添加变量定义 */
__ALIGN_BEGIN  USB_OTG_CORE_HANDLE  USB_OTG_dev   __ALIGN_END;
__ALIGN_BEGIN  USB_OTG_CORE_HANDLE  USB_OTG_Core  __ALIGN_END;





//表示USB连接状态
//0,没有连接;
//1,已经连接;
vu8 bDeviceState=0;		//默认没有连接  

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

//USB OTG 中断服务函数
//处理所有USB中断
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
//指向DEVICE_PROP结构体
//USB Device 用户回调函数. 
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

/* 使能U盘设备 ****************************/
void usbd_OpenMassStorage(void)
{
	USBD_Init(&USB_OTG_dev,
			USB_OTG_FS_CORE_ID,
			&USR_desc,
			&USBD_MSC_cb,
			&USR_cb);
	printf("USB ready\r\n\r\n");		//	
}

/* 移除U盘设备 ***************************/
void usbd_CloseMassStorage(void)
{
	DCD_DevDisconnect(&USB_OTG_dev);

	USB_OTG_BSP_DisableInterrupt();	
}



//USB Device 用户自定义初始化函数
void USBD_USR_Init(void)
{
	//printf("USBD_USR_Init\r\n");
} 
//USB Device 复位
//speed:USB速度,0,高速;1,全速;其他,错误.
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
//USB Device 配置成功
void USBD_USR_DeviceConfigured (void)
{
	printf("MSC Interface started.\r\n"); 
} 
//USB Device挂起
void USBD_USR_DeviceSuspended(void)
{
	printf("Device In suspend mode.\r\n");
} 
//USB Device恢复
void USBD_USR_DeviceResumed(void)
{ 
	printf("Device Resumed\r\n");
}
//USB Device连接成功
void USBD_USR_DeviceConnected (void)
{
	bDeviceState=1;
	printf("USB Device Connected.\r\n");
}
//USB Device未连接
void USBD_USR_DeviceDisconnected (void)
{
	bDeviceState=0;
	printf("USB Device Disconnected.\r\n");
} 


















