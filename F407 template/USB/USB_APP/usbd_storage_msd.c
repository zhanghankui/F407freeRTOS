#include "usbd_msc_mem.h"
#include "usb_conf.h"
#include "w25qxx.h"			 
#include "sdio_sdcard.h"	 
//////////////////////////////////////////////////////////////////////////////////	 
 
//USB������  ��������	   
//STM32F4����ģ��-�⺯���汾
//�Ա����̣�http://mcudev.taobao.com									  
//*******************************************************************************
//�޸���Ϣ
//��
////////////////////////////////////////////////////////////////////////////////// 	   
 
//���֧�ֵ��豸��,2��
#define STORAGE_LUN_NBR 	2

////////////////////////////�Լ������һ�����USB״̬�ļĴ���///////////////////
//bit0:��ʾ����������SD��д������
//bit1:��ʾ��������SD����������
//bit2:SD��д���ݴ����־λ
//bit3:SD�������ݴ����־λ
//bit4:1,��ʾ��������ѯ����(�������ӻ�������)
//vu8 USB_STATUS_REG=0;
////////////////////////////////////////////////////////////////////////////////


//USB Mass storage ��׼��ѯ����(ÿ��lunռ36�ֽ�)
const int8_t  STORAGE_Inquirydata[] = { 
  
	/* LUN 0 */ 
	0x00,		
	0x80,		
	0x02,		
	0x02,
	(USBD_STD_INQUIRY_LENGTH - 4),
	0x00,
	0x00,	
	0x00,
    /* Vendor Identification */
    'M', 'C', 'U', 'D', 'W', 'V', 'T', 'H', ' ',//9�ֽ� MCU dwvth
    /* Product Identification */
    'S', 'P', 'I', ' ', 'F', 'l', 'a', 's', 'h',//15�ֽ� spiflash
    ' ','D', 'i', 's', 'k', ' ',               //disk
    /* Product Revision Level */	
    '1', '.', '0', ' ',							//4�ֽ�		 1.0
	
	/* LUN 1 */
	0x00,
	0x80,		
	0x02,		
	0x02,
	(USBD_STD_INQUIRY_LENGTH - 5),
	0x00,
	0x00,	
	0x00,
	/* Vendor Identification */
	'M', 'C', 'U', 'D', 'W', 'V', 'T', 'H', ' ',//9�ֽ� MCU DWVTH
	/* Product Identification */				
	'S', 'D', ' ', 'F', 'l', 'a', 's', 'h', ' ',//15�ֽ� SDFLASH
	'D', 'i', 's', 'k', ' ', ' ',                //DISK
    /* Product Revision Level */
	'1', '.', '0' ,' ',                      	//4�ֽ� 1.0
}; 
int8_t STORAGE_Init (uint8_t lun);
int8_t STORAGE_GetCapacity (uint8_t lun,uint32_t *block_num,uint32_t *block_size);
int8_t  STORAGE_IsReady (uint8_t lun);
int8_t  STORAGE_IsWriteProtected (uint8_t lun);
int8_t STORAGE_Read (uint8_t lun,uint8_t *buf,uint32_t blk_addr,uint16_t blk_len);
int8_t STORAGE_Write (uint8_t lun,uint8_t *buf,uint32_t blk_addr,uint16_t blk_len);
int8_t STORAGE_GetMaxLun (void);

//USB Device �û��ص������ӿ�
USBD_STORAGE_cb_TypeDef USBD_MICRO_SDIO_fops =
{
	STORAGE_Init,
	STORAGE_GetCapacity,
	STORAGE_IsReady,
	STORAGE_IsWriteProtected,
	STORAGE_Read,
	STORAGE_Write,
	STORAGE_GetMaxLun,
	(int8_t *)STORAGE_Inquirydata,
};

/* USBD_STORAGE_fops �������̼����е� usbd_msc_scsi.c ���� */
USBD_STORAGE_cb_TypeDef  *USBD_STORAGE_fops=&USBD_MICRO_SDIO_fops;//ָ��USBD_MICRO_SDIO_fops�ṹ��.

//��ʼ���洢�豸
//lun:�߼���Ԫ���,0,SD��;1,SPI FLASH
//����ֵ:0,�ɹ�;
//    ����,�������
int8_t STORAGE_Init (uint8_t lun)
{
	if (SD_Init() != 0)
	{
		return (-1); 
	}
	W25QXX_Init();
	return 0; 
} 

//��ȡ�洢�豸�������Ϳ��С
//lun:�߼���Ԫ���,1,SD��;0,SPI FLASH
//block_num:������(������)
//block_size:���С(������С)
//����ֵ:0,�ɹ�;
//    ����,�������
int8_t STORAGE_GetCapacity (uint8_t lun, uint32_t *block_num, uint32_t *block_size)
{     
	if(lun==1)//SD ��
	{
		if(SD_GetStatus() != 0 )
		{
			return (-1); 
		}   		
		*block_size=512;  
		*block_num=SDCardInfo.CardCapacity/512;
	}else
	{
		*block_size=512;  
		*block_num=1024*1024*2/512;	//SPI FLASH��ǰ��12M�ֽ�,�ļ�ϵͳ��  2M
	}	
	return 0; 
} 

//�鿴�洢�豸�Ƿ����
//lun:�߼���Ԫ���,0,SD��;1,SPI FLASH
//����ֵ:0,����;
//    ����,δ����
int8_t  STORAGE_IsReady (uint8_t lun)
{
	static int8_t last_status = 0;
	if(lun==1)//SD ��
	{
		if(last_status  < 0)
		{
			SD_Init();
			last_status = 0;
		}
		
		if(SD_GetStatus() != 0)
		{
			last_status = -1;
			return (-1); 
		} 		
	}		
//	USB_STATUS_REG|=0x10;//�����ѯ
	return 0;
} 

//�鿴�洢�豸�Ƿ�д����
//lun:�߼���Ԫ���,1,SD��;0,SPI FLASH
//����ֵ:0,û��д����;
//    ����,д����(ֻ��)
int8_t  STORAGE_IsWriteProtected (uint8_t lun)
{
	return  0;
} 

//�Ӵ洢�豸��ȡ����
//lun:�߼���Ԫ���,0,SD��;1,SPI FLASH
//buf:���ݴ洢���׵�ַָ��
//blk_addr:Ҫ��ȡ�ĵ�ַ(������ַ)
//blk_len:Ҫ��ȡ�Ŀ���(������) 
//����ֵ:0,�ɹ�;
//    ����,������� 
int8_t STORAGE_Read (uint8_t lun,uint8_t *buf,uint32_t blk_addr,uint16_t blk_len)
{
	int8_t res=0;
	if(lun==1)
	{
		res = SD_ReadDisk(buf,blk_addr,blk_len);	
	}else
	{
		W25QXX_Read(buf,blk_addr*512,blk_len*512);
	} 
	return res;
}
//��洢�豸д����
//lun:�߼���Ԫ���,0,SD��;1,SPI FLASH
//buf:���ݴ洢���׵�ַָ��
//blk_addr:Ҫд��ĵ�ַ(������ַ)
//blk_len:Ҫд��Ŀ���(������) 
//����ֵ:0,�ɹ�;
//    ����,������� 
int8_t STORAGE_Write (uint8_t lun,uint8_t *buf,uint32_t blk_addr,uint16_t blk_len) 
{
	int8_t res=0;
	if(lun==1)
	{
		res = SD_WriteDisk (buf,blk_addr,blk_len);
	}else
	{
		W25QXX_Write(buf,blk_addr*512,blk_len*512);
	} 
	return res; 
}
//��ȡ֧�ֵ�����߼���Ԫ����
//����ֵ:֧�ֵ��߼���Ԫ����-1
int8_t STORAGE_GetMaxLun (void)
{
	return (STORAGE_LUN_NBR - 1);
}
























