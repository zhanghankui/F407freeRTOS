#include "can_thread.h"
#include "stdio.h"
#include "globalstruct.h"
#include "event_groups.h"

#define CANOpen_THREAD_STACK        512

extern EventGroupHandle_t xCreatedEventGroup;

xQueueHandle xQ_CAN_MSG = NULL;                //���CAN���յ�������
xTaskHandle  xH_CANOpen = NULL;

CO_DATA_STRUCT  CO_D = {NULL,NULL};
//canopen���ݴ������
static void canopen_dataprocess_thread(void *arg)
{
	int i;
	Message RxMSG = Message_Initializer;
	CANOpen_Message CAN_Rx_m;
	xQ_CAN_MSG = xQueueCreate(20,sizeof(CANOpen_Message));          //����һ��������20�����ݵĶ���
	if(NULL == xQ_CAN_MSG)
	{
		//��������ʧ��
		while(1)
		{
			printf("Creat the CANOpen Queue failed! \r\n");
			vTaskDelay(1000);
		}
	}

	while(1)
	{
		//�ɹ����յ�CAN��������
		if(xQueueReceive( xQ_CAN_MSG, &(CAN_Rx_m), (portTickType)100))//�����Ϣ����Ϊ���ҵ���������Ϊ0����ô�˺������������ء�
		{
			printf("thread get a can packege\r\n");

			RxMSG.cob_id = (uint16_t)(CAN_Rx_m.m.StdId);
			RxMSG.rtr = CAN_Rx_m.m.RTR;
			RxMSG.len = CAN_Rx_m.m.DLC;
			for(i=0;i<RxMSG.len;i++)
			{
				 RxMSG.data[i] = CAN_Rx_m.m.Data[i];
			}
			if((NULL != CO_D.CO_CAN1) && (1 == CAN_Rx_m.CANx))          //���ܵĵ���CAN1�˿�����
			{
				canDispatch(CO_D.CO_CAN1, &RxMSG);                        // �����յ���CAN����
					/* �����¼���־���bit0 */
					xEventGroupSetBits(xCreatedEventGroup, BIT_0_CAN1Dispatched);				
			}
			else if((NULL != CO_D.CO_CAN2) && (2 == CAN_Rx_m.CANx))     //���յ�����CAN2�˿�����
			{
				 canDispatch(CO_D.CO_CAN2, &RxMSG);                       // �����յ���CAN����
			}
		}
	}
}

void canopen_init(void)
{
	xTaskCreate(canopen_dataprocess_thread, "CANOpen", CANOpen_THREAD_STACK, NULL,CANOpen_THREAD_PRIO, &xH_CANOpen);
	if(NULL == xH_CANOpen)
	{
		printf("CANOpen dataprocess thread created failed!\r\n");
	}
}
