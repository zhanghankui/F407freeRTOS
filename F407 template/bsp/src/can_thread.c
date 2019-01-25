#include "can_thread.h"
#include "stdio.h"
#include "globalstruct.h"

#define CANOpen_THREAD_STACK        512

xQueueHandle xQ_CAN_MSG = NULL;                //存放CAN接收到的数据
xTaskHandle  xH_CANOpen = NULL;

CO_DATA_STRUCT  CO_D = {NULL,NULL};
//canopen数据处理进程
static void canopen_dataprocess_thread(void *arg)
{
	 int i;
	 Message RxMSG = Message_Initializer;
	 CANOpen_Message CAN_Rx_m;
	 xQ_CAN_MSG = xQueueCreate(20,sizeof(CANOpen_Message));          //创建一个能容纳20个数据的队列
	 if(NULL == xQ_CAN_MSG)
	 {
	 	//创建队列失败
		while(1)
		{
			printf("Creat the CANOpen Queue failed! \r\n");
			vTaskDelay(1000);
		}
	 }
	
	 while(1)
	 {
	 	//成功接收到CAN总线数据
	 	if(xQueueReceive( xQ_CAN_MSG, &(CAN_Rx_m), (portTickType)100))
		{
			  printf("thread get a can packege\r\n");
			
			  RxMSG.cob_id = (uint16_t)(CAN_Rx_m.m.StdId);
			  RxMSG.rtr = CAN_Rx_m.m.RTR;
			  RxMSG.len = CAN_Rx_m.m.DLC;
			  for(i=0;i<RxMSG.len;i++)
			  {
			     RxMSG.data[i] = CAN_Rx_m.m.Data[i];
			  }
			  if((NULL != CO_D.CO_CAN1) && (1 == CAN_Rx_m.CANx))          //接受的的是CAN1端口数据
			  {
			  	canDispatch(CO_D.CO_CAN1, &RxMSG);                        // 处理收到的CAN数据
			  }
			  else if((NULL != CO_D.CO_CAN2) && (2 == CAN_Rx_m.CANx))     //接收到的是CAN2端口数据
			  {
			  	 canDispatch(CO_D.CO_CAN2, &RxMSG);                       // 处理收到的CAN数据
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
