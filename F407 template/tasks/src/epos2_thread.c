/*
*********************************************************************************************************
* @file    epos2_thread.c 
* @author  smilingfrog 
* @company Dafanshu Innovations Co. Ltd
* @version 1.00
* @date    21-Mar-2019 22:39
* @brief   epos2 thread

* Update record
* version   date           author          description
* V1.00     21-Mar-2019    smilingfrog     first release
*********************************************************************************************************
*/	
	
// **************************************************************************	
// the includes
#include "epos2_thread.h"
#include "stdio.h"
#include "globalstruct.h"
#include "can_thread.h"
#include "event_groups.h"
// **************************************************************************
// the defines


// **************************************************************************
// the typedefs	


// **************************************************************************
//declaration static function prototypes



// **************************************************************************
// the globals

#define EPOS2init_THREAD_STACK        512

extern EventGroupHandle_t xCreatedEventGroup;
xTaskHandle  xH_EPOS2init = NULL;
// **************************************************************************
// the functions	

static void EPOS2init_thread(void *pvParameters)
{
	CO_Data *d;
	UNS32 errorCode;
	const indextable *ptrTable;	
	UNS8  bDataType;	
	UNS32 datasize;
	UNS32 abortCode;
	EventBits_t uxBits;	
	const TickType_t xTicksToWait = 100 / portTICK_PERIOD_MS; /* ����ӳ�100ms */
	
	d = (CO_Data *)pvParameters;//���ݵ��β�	

	while(1)
	{
		//��6041״̬��
		ptrTable= (*d->scanIndexOD)(d, 0x6041, &errorCode);
		bDataType = ptrTable->pSubindex[0].bDataType;
		if(readNetworkDict(d,1,0x6041,0,bDataType,0) == 0)//���ͳɹ�
		{
			while(1)
			{
				/* ��CANthread ����bit0 */
				uxBits = xEventGroupWaitBits(xCreatedEventGroup, /* �¼���־���� */
													 BIT_0_CAN1Dispatched,            /* �ȴ�bit0������ */
													 pdTRUE,             /* �˳�ǰbit0�������������bit0�����òű�ʾ���˳���*/
													 pdTRUE,             /* ����ΪpdTRUE��ʾ�ȴ�bit0������*/
													 xTicksToWait); 	 /* �ȴ��ӳ�ʱ�� */	
				if((uxBits & BIT_0_CAN1Dispatched) == BIT_0_CAN1Dispatched)
				{
					/* ���յ�bit0�����õ���Ϣ */
					datasize = sizeof(Statusword);
					if(getReadResultNetworkDict(d,1,&Statusword,&datasize,&abortCode) == SDO_FINISHED)
					{
						break;
					}
				}
			}
			vTaskDelete(xH_EPOS2init);
			xH_EPOS2init = NULL;		
		}
		else
		{
			vTaskDelay(100);
		}
	}
}


void EPOS2_init(void)
{
	xTaskCreate(EPOS2init_thread, "EPOS2init", EPOS2init_THREAD_STACK, CO_D.CO_CAN1,EOPS2THREAD_PRIO, &xH_EPOS2init);
	if(NULL == xH_EPOS2init)
	{
		printf("EPOS2 init thread created failed!\r\n");
	}
}
