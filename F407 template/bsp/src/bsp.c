/*
*********************************************************************************************************
* @file    bsp.c 
* @author  smilingfrog 
* @company Dafanshu Innovations Co. Ltd
* @version 1.00
* @date    24-May-2018 22:09
* @brief   这是硬件底层驱动程序的主文件。每个c文件可以 #include "bsp.h" 来包含所有的外设驱动模块。
*			     bsp = Borad surport packet 板级支持包
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
*	函 数 名: bsp_Init
*	功能说明: 初始化所有的硬件设备。该函数配置CPU寄存器和外设的寄存器并初始化一些全局变量。只需要调用一次
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/

//通过串口打印SD卡相关信息
static void show_sdcard_info(void)
{
	printf("Card ManufacturerID:%d\r\n",SDCardInfo.SD_cid.ManufacturerID);	//制造商ID
 	printf("Card RCA:%d\r\n",SDCardInfo.RCA);								//卡相对地址
	printf("Card Capacity:%d MB\r\n",(u32)(SDCardInfo.CardCapacity>>20));	//显示容量
 	printf("Card BlockSize:%d\r\n",SDCardInfo.CardBlockSize);			//显示块大小
 	printf("Card CardType:%d\r\n\r\n",SDCardInfo.CardType);		//显示卡类型
}

void bsp_Init(BSP_Handle handle)
{
	/*
		由于ST固件库的启动文件已经执行了CPU系统时钟的初始化，所以不必再次重复配置系统时钟。
		启动文件配置了CPU主时钟频率、内部Flash访问速度和可选的外部SRAM FSMC初始化。

		系统时钟缺省配置为168MHz，如果需要更改，可以修改 system_stm32f4xx.c 文件
	*/
	/* 优先级分组设置为4，可配置0-15级抢占式优先级，0级子优先级，即不存在子优先级。*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);


	//结构体初始化
	handle->usartdma_fifohandle = 
		USARTDMA_FIFO_init(&handle->usartdma_fifo,sizeof(handle->usartdma_fifo));
	USARTDMA_FIFOhandle_intr = handle->usartdma_fifohandle;


	
	//函数初始化	
	SetupUSARTDMA_FIFIO(handle->usartdma_fifohandle);
	USARTDMA_Config(512000);
	USART_DMA_Receive((uint32_t)handle->usartdma_fifohandle->pbuf,
		handle->usartdma_fifohandle->buffersize);	
	bsp_InitKey();		/* 初始化按键变量 */	
	LED_Init();/* 初始LED指示灯端口 */
	W25QXX_Init();
	SD_Init();//SD卡初始化
	show_sdcard_info();	//打印SD卡相关信息
	
	//格式化
//	uint8_t work[2048];
//	f_mkfs("1:",FM_FAT32,1024,work,2048);
	/* 挂载文件系统 */
	result = f_mount(&fs, "1:", 1);	//DEV_MMC 1

	u32 total,free;
	exf_getfree((u8 *)"1:",&total,&free);
	
 	printf("SD Total Size:%d",total>>10);	
 	printf("MB\r\n\r\n");		
 	printf("SD Free Size:%d",free>>10);
 	printf("MB\r\n\r\n");	


//	bsp_InitHardTimer(); /* 初始化TIM2定时器 */
//	bsp_SetTIMOutcompPWM(GPIOE,GPIO_Pin_9,GPIOE,GPIO_Pin_8,TIM1,1,5000000,5000);

	
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/

