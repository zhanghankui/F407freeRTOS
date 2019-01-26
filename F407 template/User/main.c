/*
*********************************************************************************************************
* @file    main.c 
* @author  smilingfrog 
* @company Dafanshu Innovations Co. Ltd
* @version 1.00
* @date    24-May-2018 22:09
* @brief   main

* Update record
* version   date           author          description
* V1.00     24-May-2018    smilingfrog     first release


F407与F429 的FSMC名称不一样，
在F407中是: FSMC_Bank1和FSMC_Bank1E
在F429中是: FMC_Bank1和FMC_Bank1E
*********************************************************************************************************
*/
#include "includes.h"

/*
**********************************************************************************************************
											函数声明
**********************************************************************************************************
*/
static void vTaskTask1(void *pvParameters);
static void vTaskTask2(void *pvParameters);
static void vTaskTask3(void *pvParameters);
static void vTaskTask4(void *pvParameters);
static void AppTaskCreate(void);

static void TIM_CallBack1(void);
static void StackOverflowTest(void);
/*
**********************************************************************************************************
											变量声明
**********************************************************************************************************
*/
static TaskHandle_t xHandleTask1 = NULL;
static TaskHandle_t xHandleTask2 = NULL;
static TaskHandle_t xHandleTask3 = NULL;
static TaskHandle_t xHandleTask4 = NULL;


BSP_Obj     BSPobj;
BSP_Handle  BSPHandle;





/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: 标准c程序入口。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
int main(void)
{
	/* 
	  在启动调度前，为了防止初始化STM32外设时有中断服务程序执行，这里禁止全局中断(除了NMI和HardFault)。
	  这样做的好处是：
	  1. 防止执行的中断服务程序中有FreeRTOS的API函数。
	  2. 保证系统正常启动，不受别的中断影响。
	  3. 关于是否关闭全局中断，大家根据自己的实际情况设置即可。
	  在移植文件port.c中的函数prvStartFirstTask中会重新开启全局中断。通过指令cpsie i开启，__set_PRIMASK(1)
	  和cpsie i是等效的。
     */
//	__set_PRIMASK(1);  
	
	/* 硬件初始化 */
	bsp_Init(&BSPobj);
	BSPHandle = &BSPobj;
	
	
	
	/* 1. 初始化一个定时器中断，精度高于滴答定时器中断，这样才可以获得准确的系统信息 仅供调试目的，实际项
		  目中不要使用，因为这个功能比较影响系统实时性。
	   2. 为了正确获取FreeRTOS的调试信息，可以考虑将上面的关闭中断指令__set_PRIMASK(1); 注释掉。 
	*/
//	vSetupSysInfoTest();	//系统测试函数
	
	/* 创建任务 */
	AppTaskCreate();
	
	/* 启动调度，开始执行任务 */
	vTaskStartScheduler();

	/* 
	  如果系统正常启动是不会运行到这里的，运行到这里极有可能是用于定时器任务或者空闲任务的
	  heap空间不足造成创建失败，此要加大FreeRTOSConfig.h文件中定义的heap大小：
	  #define configTOTAL_HEAP_SIZE	      ( ( size_t ) ( 30 * 1024 ) )
	*/
	while(1)
	{
	}
}

/*
*********************************************************************************************************
*	函 数 名: vTaskTask1
*	功能说明: 接口消息处理，这里用作LED闪烁	
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 1  (数值越小优先级越低，这个跟uCOS相反)
*********************************************************************************************************
*/
static void vTaskTask1(void *pvParameters)
{
	uint8_t ucKeyCode;
	uint8_t pcWriteBuffer[500];
	uint8_t buf[512];
	uint32_t sd_size;
	while(1)
	{
		ucKeyCode = bsp_GetKey();
		if (ucKeyCode != KEY_NONE)
		{
			switch (ucKeyCode)
			{
				/* K0键按下 打印任务执行情况 */
				case KEY_DOWN_K0:
				{
					taskENTER_CRITICAL();   /* 进入临界区 */			
					printf("=============================================\r\n");			
					printf("任务名      任务状态 优先级   剩余栈 任务序号\r\n");					
					vTaskList((char *)&pcWriteBuffer);
					printf("%s\r\n", pcWriteBuffer);
				
					printf("\r\n任务名       运行计数         使用率\r\n");
					vTaskGetRunTimeStats((char *)&pcWriteBuffer);
					printf("%s\r\n", pcWriteBuffer);
					taskEXIT_CRITICAL();  	/* 退出临界区 */
				}
				break;

				/* K1键按下，挂起任务vTask2 */
				case KEY_DOWN_K1:
				{
					taskENTER_CRITICAL();   /* 进入临界区 */	
					
					if(SD_ReadDisk(buf,0,1)==0)	//读取0扇区的内容
					{	
						printf("SECTOR 0 DATA:\r\n");
						for(sd_size=0;sd_size<512;sd_size++)printf("%x ",buf[sd_size]);//打印0扇区数据    	   
						printf("\r\nDATA ENDED\r\n");
					}		
					
//					printf("K1键按下，挂起任务vTask2\r\n");
					taskEXIT_CRITICAL();  	/* 退出临界区 */
//					vTaskSuspend(xHandleTask2);//挂起任务2
				}
				break;

				/* KWU键按下，启动单次定时器中断，50ms后在定时器中断将任务vTask2恢复 */
				case KEY_DOWN_KWU:
				{
					taskENTER_CRITICAL();   /* 进入临界区 */
					for(sd_size=0;sd_size<512;sd_size++)
					{
						buf[sd_size] = sd_size;
					}
					SD_WriteDisk(buf,0,1);
//					printf("KWU键按下，将任务vTask2恢复\r\n");
					taskEXIT_CRITICAL();  	/* 退出临界区 */
//					vTaskResume(xHandleTask2);
//					bsp_StartHardTimer(1 ,50000, (void *)TIM_CallBack1);
			}
				break;	

				
//				/* K1键按下，设置任务vTask1的优先级为1 */
//				case KEY_DOWN_K1:
//				{
//					printf("K1键按下，设置任务vTask2的优先级为1\r\n");
//					vTaskPrioritySet( xHandleTask2, 1);
//					printf("K1键按下，任务vTask2的优先级已经设置为：%d\r\n",  (int)uxTaskPriorityGet(xHandleTask2));
//				}
//				break;

//				/* KWU键按下，设置任务vTask1的优先级为2 */
//				case KEY_DOWN_KWU:
//				{
//					printf("KWU键按下，设置任务vTask2的优先级为1\r\n");
//					vTaskPrioritySet( xHandleTask2, 2);
//					printf("KWU键按下，任务vTask2的优先级已经设置为：%d\r\n",  (int)uxTaskPriorityGet(xHandleTask2));
//				}
//				break;				
				
	
//				/* K1键按下 删除任务vTaskLED */
//				case KEY_DOWN_K1:
//				{					
//					printf("K1键按下，删除任务vTask2\r\n");
//					if(xHandleTask2 != NULL)
//					{
//						vTaskDelete(xHandleTask2);
//						xHandleTask2 = NULL;
//					}
//				}
//				break;
//				
//				/* KWU键按下 重新创建任务vTaskLED */
//				case KEY_DOWN_KWU:	
//					printf("KWU键按下，重新创建任务vTask2\r\n");
//					if(xHandleTask2 == NULL)
//					{
//						xTaskCreate(    vTaskTask2,            /* 任务函数  */
//										"vTaskTask2",          /* 任务名    */
//										512,                 /* stack大小，单位word，也就是4字节 */
//										NULL,                /* 任务参数  */
//										2,                   /* 任务优先级*/
//										&xHandleTask2 );   /* 任务句柄  */
//					}
//					break;
					
//				/* K1键按下，挂起任务vTask2 */
//				case KEY_DOWN_K1:
//				{
//					printf("K1键按下，挂起任务vTask2\r\n");
//					vTaskSuspend(xHandleTask2);
//				}
//				break;
//				
//				/* KWU键按下，恢复任务vTask2 */
//				case KEY_DOWN_KWU:
//				{
//					printf("KWU键按下，恢复任务vTaskLED\r\n");
//					vTaskResume(xHandleTask2);
//				}
//				break;
					
				/* KWU键按下，启动单次定时器中断，50ms后在定时器中断将任务vTask2恢复 */
//				case KEY_DOWN_KWU:
//				{
//					printf("KWU键按下，启动单次定时器中断，50ms后在定时器中断将任务vTask2恢复\r\n");
//					bsp_StartHardTimer(1 ,50000, (void *)TIM_CallBack1);
//				}
//				break;					
				
				/* 其他的键值不处理 */
				default:
				{
				}					
				break;
			}
		}		

		vTaskDelay(20);//unit portTICK_PERIOD_MS
	}
}

/*
*********************************************************************************************************
*	函 数 名: vTaskTask2
*	功能说明: LED闪烁	
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 2  
*********************************************************************************************************
*/
static void vTaskTask2(void *pvParameters)
{
	while(1)
	{
		taskENTER_CRITICAL();   /* 进入临界区 */
//		printf("任务vTask2正在运行\r\n");
		taskEXIT_CRITICAL();  	/* 退出临界区 */
		LED1=!LED1;
		vTaskDelay(500);
	}
}

/*
*********************************************************************************************************
*	函 数 名: vTaskTask3
*	功能说明: 信息处理，这里是用作LED闪烁	
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 3  
*********************************************************************************************************
*/
static void vTaskTask3(void *pvParameters)
{
	while(1)
	{
		taskENTER_CRITICAL();   /* 进入临界区 */
//		printf("任务vTask3正在运行\r\n");
		taskEXIT_CRITICAL();  	/* 退出临界区 */		
		LED0=!LED0;
		vTaskDelay(600);
	}
}

/*
*********************************************************************************************************
*	函 数 名: vTaskTask4
*	功能说明: 启动任务，也就是最高优先级任务，这里用作LED闪烁
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 4  
*********************************************************************************************************
*/
static void vTaskTask4(void *pvParameters)
{
	while(1)
	{
		/* LED闪烁 */
		bsp_KeyScan();
		vTaskDelay(10);
	}
}

/*
*********************************************************************************************************
*	函 数 名: TIM_CallBack1
*	功能说明: 定时器中断的回调函数，此函数被bsp_StartHardTimer所调用。		  			  
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void TIM_CallBack1(void)
{
	BaseType_t xYieldRequired;
	UBaseType_t uxSavedInterruptStatus;
//    /* 恢复挂起任务 */
//     xYieldRequired = xTaskResumeFromISR(xHandleTask2);

//	 /* 退出中断后是否需要执行任务切换 */
//     if( xYieldRequired == pdTRUE )
//     {
//         portYIELD_FROM_ISR(xYieldRequired);
//     }
	
	uxSavedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();  /* 进入临界区 */
	{
		/* 用户可以在这里添加临界段代码，我们这里暂时未用到 */
	}
	portCLEAR_INTERRUPT_MASK_FROM_ISR( uxSavedInterruptStatus ); /* 退出临界区 */

	/* 恢复挂起任务 */
	xYieldRequired = xTaskResumeFromISR(xHandleTask2);

	/* 退出中断后是否需要执行任务切换 */
	if( xYieldRequired == pdTRUE )
	{
	 portYIELD_FROM_ISR(xYieldRequired);
	}		
}


/*
*********************************************************************************************************
*	函 数 名: StackOverflowTest
*	功能说明: 任务栈溢出测试
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void StackOverflowTest(void)
{
	int16_t i;
	uint8_t buf[2048];
	
	(void)buf; /* 防止警告 */
	
	/*
	  1. 为了能够模拟任务栈溢出，并触发任务栈溢出函数，这里强烈建议使用数组的时候逆着赋值。
	     因为对于M3和M4内核的MCU，堆栈生长方向是向下生长的满栈。即高地址是buf[2047], 低地址
	     是buf[0]。如果任务栈溢出了，也是从高地址buf[2047]到buf[0]的某个地址开始溢出。
	        因此，如果用户直接修改的是buf[0]开始的数据且这些溢出部分的数据比较重要，会直接导致
	     进入到硬件异常。
	  2. 栈溢出检测是在任务切换的时候执行的，我们这里加个延迟函数，防止修改了重要的数据导致直接
	     进入硬件异常。
	  3. 任务vTaskTaskUserIF的栈空间大小是2048字节，在此任务的入口已经申请了栈空间大小
		 ------uint8_t ucKeyCode;
	     ------uint8_t pcWriteBuffer[500];
	     这里再申请如下这么大的栈空间
	     -------int16_t i;
		 -------uint8_t buf[2048];
	     必定溢出。
	*/
	for(i = 2047; i >= 0; i--)
	{
		buf[i] = 0x55;
		vTaskDelay(1);
	}
}

/*
*********************************************************************************************************
*	函 数 名: vApplicationStackOverflowHook
*	功能说明: 栈溢出的钩子函数
*	形    参: xTask        任务句柄
*             pcTaskName   任务名
*	返 回 值: 无
*********************************************************************************************************
*/
void vApplicationStackOverflowHook( TaskHandle_t xTask, signed char *pcTaskName )
{
	printf("任务：%s 发现栈溢出\r\n", pcTaskName);
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskCreate
*	功能说明: 创建应用任务
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void AppTaskCreate (void)
{
	xTaskCreate( vTaskTask1,   	/* 任务函数  */
							 "vTaskTask1",     	/* 任务名    */
							 512,               	/* 任务栈大小，单位word，也就是4字节 */
							 NULL,              	/* 任务参数  */
							 1,                 	/* 任务优先级*/
							 &xHandleTask1 );  /* 任务句柄  */


	xTaskCreate( vTaskTask2,    		/* 任务函数  */
							 "vTaskTask2",  		/* 任务名    */
							 512,         		/* 任务栈大小，单位word，也就是4字节 */
							 NULL,        		/* 任务参数  */
							 2,           		/* 任务优先级*/
							 &xHandleTask2 ); /* 任务句柄  */

	xTaskCreate( vTaskTask3,     		/* 任务函数  */
							 "vTaskTask3",   		/* 任务名    */
							 512,             		/* 任务栈大小，单位word，也就是4字节 */
							 NULL,           		/* 任务参数  */
							 3,               		/* 任务优先级*/
							 &xHandleTask3 );  /* 任务句柄  */


	xTaskCreate( vTaskTask4,     		/* 任务函数  */
							 "vTaskTask4",   		/* 任务名    */
							 512,            		/* 任务栈大小，单位word，也就是4字节 */
							 NULL,           		/* 任务参数  */
							 4,              		/* 任务优先级*/
							 &xHandleTask4 );   /* 任务句柄  */
							 
	/*init the Canopen thread*/
	canopen_init();
	
	initTimer();                               //初始化CANopen定时器
	canInit(CAN1,CAN_BAUD_1M);             //初始化CAN1
}






#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

