/*
*********************************************************************************************************
*
*	ģ������ : USB�豸����ģ��,�������洢�豸
*	�ļ����� : usb_bsp_msc.h
*	��    �� : V1.0
*	˵    �� : ����USB Deviceģ��h�ļ���Ӧ�ó��������һ��h�ļ����ɡ�
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2013-02-01 armfly  ��ʽ����
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _USB_BSP_USR_H
#define _USB_BSP_USR_H

#include "usbd_msc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usb_conf.h"
#include "usb_bsp.h"

void usbd_OpenMassStorage(void);
void usbd_CloseMassStorage(void);
void usbd_OpenCDC(void);

extern USB_OTG_CORE_HANDLE     USB_OTG_dev;

#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
