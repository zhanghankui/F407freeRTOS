#include "can_thread.h"
#include "stdio.h"
#include "globalstruct.h"
#include "event_groups.h"

#define CANOpen_THREAD_STACK        512



xQueueHandle xQ_CAN_MSG = NULL;                //���CAN���յ�������
xTaskHandle  xH_CANOpen = NULL;

CO_DATA_STRUCT  CO_D = {NULL,NULL,NULL,NULL};
//canopen���ݴ������
static void canopen_dataprocess_thread(void *arg)
{
	uint8_t id;
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
			vTaskDelay(10000);
		}
	}

	while(1)
	{
		//�ɹ����յ�CAN��������
		if(xQueueReceive( xQ_CAN_MSG, &(CAN_Rx_m), (portTickType)1000))//�����Ϣ����Ϊ���ҵ���������Ϊ0����ô�˺������������ء�
		{
//			printf("thread get a can packege\r\n");

			RxMSG.cob_id = (uint16_t)(CAN_Rx_m.m.StdId);
			RxMSG.rtr = CAN_Rx_m.m.RTR;
			RxMSG.len = CAN_Rx_m.m.DLC;
			for(i=0;i<RxMSG.len;i++)
			{
				 RxMSG.data[i] = CAN_Rx_m.m.Data[i];
			}
			id = RxMSG.cob_id&0x7F;
			
			if(1 == CAN_Rx_m.CANx) //���ܵĵ���CAN1�˿�����
			{
				switch(id)
				{
					case 0://�㲥���鲥��Ϣ
					{
					}
					break;
					//�ڵ�
					case 1:
					{
						if(NULL != CO_D.CO_NODE1)
						{
							canDispatch(CO_D.CO_NODE1, &RxMSG);                        // �����յ���CAN����									
						}
					}
					break;
					case 2:
					{
						if(NULL != CO_D.CO_NODE2)
						{
							canDispatch(CO_D.CO_NODE2, &RxMSG);                        // �����յ���CAN����									
						}						
					}
					break;
					case 3:
					{
						if(NULL != CO_D.CO_NODE3)
						{
							canDispatch(CO_D.CO_NODE3, &RxMSG);                        // �����յ���CAN����									
						}								
					}
					break;
					case 4:
					{
						if(NULL != CO_D.CO_NODE4)
						{
							canDispatch(CO_D.CO_NODE4, &RxMSG);                        // �����յ���CAN����									
						}								
					}
					break;				
					default:
						break;
				}
			}
			else if(2 == CAN_Rx_m.CANx)     //���յ�����CAN2�˿�����
			{

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
