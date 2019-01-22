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


F407��F429 ��FSMC���Ʋ�һ����
��F407����: FSMC_Bank1��FSMC_Bank1E
��F429����: FMC_Bank1��FMC_Bank1E
*********************************************************************************************************
*/
#include "includes.h"

/*
**********************************************************************************************************
											��������
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
											��������
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
*	�� �� ��: main
*	����˵��: ��׼c������ڡ�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int main(void)
{
	/* 
	  ����������ǰ��Ϊ�˷�ֹ��ʼ��STM32����ʱ���жϷ������ִ�У������ֹȫ���ж�(����NMI��HardFault)��
	  �������ĺô��ǣ�
	  1. ��ִֹ�е��жϷ����������FreeRTOS��API������
	  2. ��֤ϵͳ�������������ܱ���ж�Ӱ�졣
	  3. �����Ƿ�ر�ȫ���жϣ���Ҹ����Լ���ʵ��������ü��ɡ�
	  ����ֲ�ļ�port.c�еĺ���prvStartFirstTask�л����¿���ȫ���жϡ�ͨ��ָ��cpsie i������__set_PRIMASK(1)
	  ��cpsie i�ǵ�Ч�ġ�
     */
//	__set_PRIMASK(1);  
	
	/* Ӳ����ʼ�� */
	bsp_Init(&BSPobj);
	BSPHandle = &BSPobj;
	
	
	
	/* 1. ��ʼ��һ����ʱ���жϣ����ȸ��ڵδ�ʱ���жϣ������ſ��Ի��׼ȷ��ϵͳ��Ϣ ��������Ŀ�ģ�ʵ����
		  Ŀ�в�Ҫʹ�ã���Ϊ������ܱȽ�Ӱ��ϵͳʵʱ�ԡ�
	   2. Ϊ����ȷ��ȡFreeRTOS�ĵ�����Ϣ�����Կ��ǽ�����Ĺر��ж�ָ��__set_PRIMASK(1); ע�͵��� 
	*/
//	vSetupSysInfoTest();	//ϵͳ���Ժ���
	
	/* �������� */
	AppTaskCreate();
	
	/* �������ȣ���ʼִ������ */
	vTaskStartScheduler();

	/* 
	  ���ϵͳ���������ǲ������е�����ģ����е����Ｋ�п��������ڶ�ʱ��������߿��������
	  heap�ռ䲻����ɴ���ʧ�ܣ���Ҫ�Ӵ�FreeRTOSConfig.h�ļ��ж����heap��С��
	  #define configTOTAL_HEAP_SIZE	      ( ( size_t ) ( 30 * 1024 ) )
	*/
	while(1)
	{
	}
}

/*
*********************************************************************************************************
*	�� �� ��: vTaskTask1
*	����˵��: �ӿ���Ϣ������������LED��˸	
*	��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*   �� �� ��: 1  (��ֵԽС���ȼ�Խ�ͣ������uCOS�෴)
*********************************************************************************************************
*/
static void vTaskTask1(void *pvParameters)
{
	uint8_t ucKeyCode;
	uint8_t pcWriteBuffer[500];
	while(1)
	{
		ucKeyCode = bsp_GetKey();
		if (ucKeyCode != KEY_NONE)
		{
			switch (ucKeyCode)
			{
				/* K0������ ��ӡ����ִ����� */
				case KEY_DOWN_K0:
				{
					taskENTER_CRITICAL();   /* �����ٽ��� */			
					printf("=============================================\r\n");			
					printf("������      ����״̬ ���ȼ�   ʣ��ջ �������\r\n");					
					vTaskList((char *)&pcWriteBuffer);
					printf("%s\r\n", pcWriteBuffer);
				
					printf("\r\n������       ���м���         ʹ����\r\n");
					vTaskGetRunTimeStats((char *)&pcWriteBuffer);
					printf("%s\r\n", pcWriteBuffer);
					taskEXIT_CRITICAL();  	/* �˳��ٽ��� */
				}
				break;

				/* K1�����£���������vTask2 */
				case KEY_DOWN_K1:
				{
					taskENTER_CRITICAL();   /* �����ٽ��� */	
					printf("K1�����£���������vTask2\r\n");
					taskEXIT_CRITICAL();  	/* �˳��ٽ��� */
					vTaskSuspend(xHandleTask2);//��������2
				}
				break;

				/* KWU�����£��������ζ�ʱ���жϣ�50ms���ڶ�ʱ���жϽ�����vTask2�ָ� */
				case KEY_DOWN_KWU:
				{
					taskENTER_CRITICAL();   /* �����ٽ��� */
					printf("KWU�����£�������vTask2�ָ�\r\n");
					taskEXIT_CRITICAL();  	/* �˳��ٽ��� */
					vTaskResume(xHandleTask2);
//					bsp_StartHardTimer(1 ,50000, (void *)TIM_CallBack1);
			}
				break;	

				
//				/* K1�����£���������vTask1�����ȼ�Ϊ1 */
//				case KEY_DOWN_K1:
//				{
//					printf("K1�����£���������vTask2�����ȼ�Ϊ1\r\n");
//					vTaskPrioritySet( xHandleTask2, 1);
//					printf("K1�����£�����vTask2�����ȼ��Ѿ�����Ϊ��%d\r\n",  (int)uxTaskPriorityGet(xHandleTask2));
//				}
//				break;

//				/* KWU�����£���������vTask1�����ȼ�Ϊ2 */
//				case KEY_DOWN_KWU:
//				{
//					printf("KWU�����£���������vTask2�����ȼ�Ϊ1\r\n");
//					vTaskPrioritySet( xHandleTask2, 2);
//					printf("KWU�����£�����vTask2�����ȼ��Ѿ�����Ϊ��%d\r\n",  (int)uxTaskPriorityGet(xHandleTask2));
//				}
//				break;				
				
	
//				/* K1������ ɾ������vTaskLED */
//				case KEY_DOWN_K1:
//				{					
//					printf("K1�����£�ɾ������vTask2\r\n");
//					if(xHandleTask2 != NULL)
//					{
//						vTaskDelete(xHandleTask2);
//						xHandleTask2 = NULL;
//					}
//				}
//				break;
//				
//				/* KWU������ ���´�������vTaskLED */
//				case KEY_DOWN_KWU:	
//					printf("KWU�����£����´�������vTask2\r\n");
//					if(xHandleTask2 == NULL)
//					{
//						xTaskCreate(    vTaskTask2,            /* ������  */
//										"vTaskTask2",          /* ������    */
//										512,                 /* stack��С����λword��Ҳ����4�ֽ� */
//										NULL,                /* �������  */
//										2,                   /* �������ȼ�*/
//										&xHandleTask2 );   /* ������  */
//					}
//					break;
					
//				/* K1�����£���������vTask2 */
//				case KEY_DOWN_K1:
//				{
//					printf("K1�����£���������vTask2\r\n");
//					vTaskSuspend(xHandleTask2);
//				}
//				break;
//				
//				/* KWU�����£��ָ�����vTask2 */
//				case KEY_DOWN_KWU:
//				{
//					printf("KWU�����£��ָ�����vTaskLED\r\n");
//					vTaskResume(xHandleTask2);
//				}
//				break;
					
				/* KWU�����£��������ζ�ʱ���жϣ�50ms���ڶ�ʱ���жϽ�����vTask2�ָ� */
//				case KEY_DOWN_KWU:
//				{
//					printf("KWU�����£��������ζ�ʱ���жϣ�50ms���ڶ�ʱ���жϽ�����vTask2�ָ�\r\n");
//					bsp_StartHardTimer(1 ,50000, (void *)TIM_CallBack1);
//				}
//				break;					
				
				/* �����ļ�ֵ������ */
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
*	�� �� ��: vTaskTask2
*	����˵��: LED��˸	
*	��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*   �� �� ��: 2  
*********************************************************************************************************
*/
static void vTaskTask2(void *pvParameters)
{
	while(1)
	{
		taskENTER_CRITICAL();   /* �����ٽ��� */
		printf("����vTask2��������\r\n");
		taskEXIT_CRITICAL();  	/* �˳��ٽ��� */
		LED1=!LED1;
		vTaskDelay(500);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: vTaskTask3
*	����˵��: ��Ϣ��������������LED��˸	
*	��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*   �� �� ��: 3  
*********************************************************************************************************
*/
static void vTaskTask3(void *pvParameters)
{
	while(1)
	{
		taskENTER_CRITICAL();   /* �����ٽ��� */
		printf("����vTask3��������\r\n");
		taskEXIT_CRITICAL();  	/* �˳��ٽ��� */		
		LED0=!LED0;
		vTaskDelay(600);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: vTaskTask4
*	����˵��: ��������Ҳ����������ȼ�������������LED��˸
*	��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*   �� �� ��: 4  
*********************************************************************************************************
*/
static void vTaskTask4(void *pvParameters)
{
	while(1)
	{
		/* LED��˸ */
		bsp_KeyScan();
		vTaskDelay(10);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: TIM_CallBack1
*	����˵��: ��ʱ���жϵĻص��������˺�����bsp_StartHardTimer�����á�		  			  
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void TIM_CallBack1(void)
{
	BaseType_t xYieldRequired;
	UBaseType_t uxSavedInterruptStatus;
//    /* �ָ��������� */
//     xYieldRequired = xTaskResumeFromISR(xHandleTask2);

//	 /* �˳��жϺ��Ƿ���Ҫִ�������л� */
//     if( xYieldRequired == pdTRUE )
//     {
//         portYIELD_FROM_ISR(xYieldRequired);
//     }
	
	uxSavedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();  /* �����ٽ��� */
	{
		/* �û���������������ٽ�δ��룬����������ʱδ�õ� */
	}
	portCLEAR_INTERRUPT_MASK_FROM_ISR( uxSavedInterruptStatus ); /* �˳��ٽ��� */

	/* �ָ��������� */
	xYieldRequired = xTaskResumeFromISR(xHandleTask2);

	/* �˳��жϺ��Ƿ���Ҫִ�������л� */
	if( xYieldRequired == pdTRUE )
	{
	 portYIELD_FROM_ISR(xYieldRequired);
	}		
}


/*
*********************************************************************************************************
*	�� �� ��: StackOverflowTest
*	����˵��: ����ջ�������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void StackOverflowTest(void)
{
	int16_t i;
	uint8_t buf[2048];
	
	(void)buf; /* ��ֹ���� */
	
	/*
	  1. Ϊ���ܹ�ģ������ջ���������������ջ�������������ǿ�ҽ���ʹ�������ʱ�����Ÿ�ֵ��
	     ��Ϊ����M3��M4�ں˵�MCU����ջ����������������������ջ�����ߵ�ַ��buf[2047], �͵�ַ
	     ��buf[0]���������ջ����ˣ�Ҳ�ǴӸߵ�ַbuf[2047]��buf[0]��ĳ����ַ��ʼ�����
	        ��ˣ�����û�ֱ���޸ĵ���buf[0]��ʼ����������Щ������ֵ����ݱȽ���Ҫ����ֱ�ӵ���
	     ���뵽Ӳ���쳣��
	  2. ջ���������������л���ʱ��ִ�еģ���������Ӹ��ӳٺ�������ֹ�޸�����Ҫ�����ݵ���ֱ��
	     ����Ӳ���쳣��
	  3. ����vTaskTaskUserIF��ջ�ռ��С��2048�ֽڣ��ڴ����������Ѿ�������ջ�ռ��С
		 ------uint8_t ucKeyCode;
	     ------uint8_t pcWriteBuffer[500];
	     ����������������ô���ջ�ռ�
	     -------int16_t i;
		 -------uint8_t buf[2048];
	     �ض������
	*/
	for(i = 2047; i >= 0; i--)
	{
		buf[i] = 0x55;
		vTaskDelay(1);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: vApplicationStackOverflowHook
*	����˵��: ջ����Ĺ��Ӻ���
*	��    ��: xTask        ������
*             pcTaskName   ������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void vApplicationStackOverflowHook( TaskHandle_t xTask, signed char *pcTaskName )
{
	printf("����%s ����ջ���\r\n", pcTaskName);
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskCreate
*	����˵��: ����Ӧ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void AppTaskCreate (void)
{
	xTaskCreate( vTaskTask1,   	/* ������  */
							 "vTaskTask1",     	/* ������    */
							 512,               	/* ����ջ��С����λword��Ҳ����4�ֽ� */
							 NULL,              	/* �������  */
							 1,                 	/* �������ȼ�*/
							 &xHandleTask1 );  /* ������  */


	xTaskCreate( vTaskTask2,    		/* ������  */
							 "vTaskTask2",  		/* ������    */
							 512,         		/* ����ջ��С����λword��Ҳ����4�ֽ� */
							 NULL,        		/* �������  */
							 2,           		/* �������ȼ�*/
							 &xHandleTask2 ); /* ������  */

	xTaskCreate( vTaskTask3,     		/* ������  */
							 "vTaskTask3",   		/* ������    */
							 512,             		/* ����ջ��С����λword��Ҳ����4�ֽ� */
							 NULL,           		/* �������  */
							 3,               		/* �������ȼ�*/
							 &xHandleTask3 );  /* ������  */


	xTaskCreate( vTaskTask4,     		/* ������  */
							 "vTaskTask4",   		/* ������    */
							 512,            		/* ����ջ��С����λword��Ҳ����4�ֽ� */
							 NULL,           		/* �������  */
							 4,              		/* �������ȼ�*/
							 &xHandleTask4 );   /* ������  */
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

