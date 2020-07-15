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
	bsp_InitDWT();

	//�ṹ���ʼ��
	handle->usartdma_fifohandle = 
		USARTDMA_FIFO_init(&handle->usartdma_fifo,sizeof(handle->usartdma_fifo));
	USARTDMA_FIFOhandle_intr = handle->usartdma_fifohandle;


	
	//������ʼ��	
	SetupUSARTDMA_FIFIO(handle->usartdma_fifohandle);
	USARTDMA_Config(115200);
	USART_DMA_Receive((uint32_t)handle->usartdma_fifohandle->pbuf,
		handle->usartdma_fifohandle->buffersize);	
	bsp_InitKey();		/* ��ʼ���������� */	
	LED_Init();/* ��ʼLEDָʾ�ƶ˿� */
	
	LCD_Init();
	
	POINT_COLOR = BLUE;
	LCD_ShowString(30,20,200,16,16,"key_up: Up     key1: Down");	
	LCD_ShowString(30,40,240,16,16,"key0  : Done!  key2: Reserve");
	LCD_ShowString(30,80,200,16,16,"Task Selected:");
	LCD_ShowString(10,100,210,16,16,"Position:          PosErr:");
	LCD_ShowString(30,120,240,16,16,"    Task       State   Count");	
	POINT_COLOR = RED;
	LCD_ShowString(30,140,100,16,16,"1.EPOS2_Init");
	LCD_ShowString(30,160,100,16,16,"2.Move+ Task");
	LCD_ShowString(30,180,100,16,16,"3.Move- Task");
	LCD_ShowString(30,200,100,16,16,"4.Back0 Task");
	LCD_ShowString(30,220,100,16,16,"5.IPM Task");	
	
//	SD_Init();//SD����ʼ��
//	show_sdcard_info();	//��ӡSD�������Ϣ
	
//	bsp_InitHardTimer(); /* ��ʼ��TIM2��ʱ�� */
//	bsp_SetTIMOutcompPWM(GPIOE,GPIO_Pin_9,GPIOE,GPIO_Pin_8,TIM1,1,5000000,5000);

	
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/

