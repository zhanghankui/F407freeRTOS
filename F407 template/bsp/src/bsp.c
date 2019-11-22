/*
*********************************************************************************************************
* @file    bsp.c 
* @author  smilingfrog 
* @company Dafanshu Innovations Co. Ltd
* @version 1.00
* @date    24-May-2018 22:09
* @brief   ����Ӳ���ײ�������������ļ���ÿ��c�ļ����� #include "bsp.h" ���������е���������ģ�顣
*			     bsp = Borad surport packet �弶֧�ְ�
* Update record
* version   date           author          description
* V1.00     24-May-2018    smilingfrog     first release
*********************************************************************************************************
*/	
#include "bsp.h"
#include "FatFS_thread.h"
extern void can_TIM3_Init(void);
/*
*********************************************************************************************************
*	�� �� ��: bsp_Init
*	����˵��: ��ʼ�����е�Ӳ���豸���ú�������CPU�Ĵ���������ļĴ�������ʼ��һЩȫ�ֱ�����ֻ��Ҫ����һ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/

//ͨ�����ڴ�ӡSD�������Ϣ
static void show_sdcard_info(void)
{
	printf("Card ManufacturerID:%d\r\n",SDCardInfo.SD_cid.ManufacturerID);	//������ID
 	printf("Card RCA:%d\r\n",SDCardInfo.RCA);								//����Ե�ַ
	printf("Card Capacity:%d MB\r\n",(u32)(SDCardInfo.CardCapacity>>20));	//��ʾ����
 	printf("Card BlockSize:%d\r\n",SDCardInfo.CardBlockSize);			//��ʾ���С
 	printf("Card CardType:%d\r\n\r\n",SDCardInfo.CardType);		//��ʾ������
}

void bsp_Init(BSP_Handle handle)
{
	/*
		����ST�̼���������ļ��Ѿ�ִ����CPUϵͳʱ�ӵĳ�ʼ�������Բ����ٴ��ظ�����ϵͳʱ�ӡ�
		�����ļ�������CPU��ʱ��Ƶ�ʡ��ڲ�Flash�����ٶȺͿ�ѡ���ⲿSRAM FSMC��ʼ����

		ϵͳʱ��ȱʡ����Ϊ168MHz�������Ҫ���ģ������޸� system_stm32f4xx.c �ļ�
	*/
	/* ���ȼ���������Ϊ4��������0-15����ռʽ���ȼ���0�������ȼ����������������ȼ���*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);


	//�ṹ���ʼ��
	handle->usartdma_fifohandle = 
		USARTDMA_FIFO_init(&handle->usartdma_fifo,sizeof(handle->usartdma_fifo));
	USARTDMA_FIFOhandle_intr = handle->usartdma_fifohandle;


	
	//������ʼ��	
	SetupUSARTDMA_FIFIO(handle->usartdma_fifohandle);
	USARTDMA_Config(512000);
	USART_DMA_Receive((uint32_t)handle->usartdma_fifohandle->pbuf,
		handle->usartdma_fifohandle->buffersize);	
	bsp_InitKey();		/* ��ʼ���������� */	
	LED_Init();/* ��ʼLEDָʾ�ƶ˿� */
	W25QXX_Init();
	SD_Init();//SD����ʼ��
	show_sdcard_info();	//��ӡSD�������Ϣ
	
	//��ʽ��
	uint8_t work[2048];
	f_mkfs("1:",FM_FAT32,1024,work,2048);
	/* �����ļ�ϵͳ */
	result = f_mount(&fs, "1:", 1);	//DEV_MMC 1

	u32 total,free;
	exf_getfree((u8 *)"1:",&total,&free);
	
 	printf("SD Total Size:%d",total>>10);	
 	printf("MB\r\n\r\n");		
 	printf("SD Free Size:%d",free>>10);
 	printf("MB\r\n\r\n");	


//	bsp_InitHardTimer(); /* ��ʼ��TIM2��ʱ�� */
//	bsp_SetTIMOutcompPWM(GPIOE,GPIO_Pin_9,GPIOE,GPIO_Pin_8,TIM1,1,5000000,5000);

	
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/

