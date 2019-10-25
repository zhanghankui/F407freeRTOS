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
#include "appdata.h"

#include "systimer.h"
// **************************************************************************
// the defines


// **************************************************************************
// the typedefs	


// **************************************************************************
//declaration static function prototypes
static void Linearmotor_thread(void *pvParameters);


// **************************************************************************
// the globals

#define EPOS2init_THREAD_STACK        512
#define Linearmotor_THREAD_STACK        512

xTaskHandle  xH_EPOS2init = NULL;
xTaskHandle  xH_Linearmotor_control = NULL;
// **************************************************************************
// the functions	


static uint8_t ReadSDO(CO_Data* d, UNS8 nodeId, UNS16 index, UNS8 subIndex,void* data,UNS8 useBlockMode)
{
	UNS32 errorCode;
	const indextable *ptrTable;	
	UNS8  bDataType;	
	UNS32 datasize;
	UNS32 abortCode;
	UNS8  SDO_UPLOADstate;
	UNS8  SDO_Readstate;
	ptrTable= (*d->scanIndexOD)(d, index, &errorCode);
	bDataType = ptrTable->pSubindex[subIndex].bDataType;
	datasize = ptrTable->pSubindex[subIndex].size;

	if(useBlockMode)
	{
		SDO_UPLOADstate = SDO_BLOCK_UPLOAD_IN_PROGRESS;
	}
	else
	{
		SDO_UPLOADstate = SDO_UPLOAD_IN_PROGRESS;		
	}
	if(readNetworkDict(d,nodeId,index,subIndex,bDataType,useBlockMode) == 0)//发送成功
	{
		Startsystimerforovertime();//启动系统定时器
		while(1)
		{
			taskENTER_CRITICAL();   /* 进入临界区 */	
			SDO_Readstate  = getReadResultNetworkDict(d,nodeId,data,&datasize,&abortCode);
			taskEXIT_CRITICAL();  	/* 退出临界区 */
			if(COMMovertimer == true)
			{
//				printf("COMM overtime\r\n");
				return 0xEE;//无应答超时
			}
			if(SDO_Readstate != SDO_UPLOADstate)//收到返回或异常
			{
				
				return SDO_Readstate;
			}
		}		
	}
	return 0xEF;//send failed
}

static uint8_t WriteSDO(CO_Data* d, UNS8 nodeId, UNS16 index, UNS8 subIndex,void* data,UNS8 useBlockMode)
{
	UNS32 errorCode;
	const indextable *ptrTable;	
	UNS8  bDataType;	
	UNS32 count;
	UNS32 abortCode;
	UNS8  SDO_DOWNLOADstate;
	UNS8  SDO_Writestate;
	ptrTable= (*d->scanIndexOD)(d, index, &errorCode);
	bDataType = ptrTable->pSubindex[subIndex].bDataType;
	count = ptrTable->pSubindex[subIndex].size;

	if(useBlockMode)
	{
		SDO_DOWNLOADstate = SDO_BLOCK_DOWNLOAD_IN_PROGRESS;
	}
	else
	{
		SDO_DOWNLOADstate = SDO_DOWNLOAD_IN_PROGRESS;		
	}
	if(writeNetworkDict(d,nodeId,index,subIndex,count,bDataType,data,useBlockMode) == 0)//发送成功
	{
		Startsystimerforovertime();//启动系统定时器
		while(1)
		{
			taskENTER_CRITICAL();   /* 进入临界区 */
			SDO_Writestate  = getWriteResultNetworkDict(d,nodeId,&abortCode);
			taskEXIT_CRITICAL();  	/* 退出临界区 */
			if(COMMovertimer == true)
			{
//				printf("COMM overtime\r\n");
				return 0xEE;//无应答超时
			}
			if(SDO_Writestate != SDO_DOWNLOADstate)//收到返回或异常
			{
				if(SDO_Writestate != SDO_FINISHED)
				{
					//处理错误码
				}
				return SDO_Writestate;
			}
		}		
	}
	return 0xEF;//send failed	
}


static void EPOS2init_thread(void *pvParameters)
{
	CO_Data *d;
	UNS8 Transmission_Type;
	UNS32 COBID;
	UNS32 ExpectedSize;	
	UNS8 highestSubIndex;	
	UNS16 inhibittime;
	UNS32 _obj;
	UNS8 nodeId = 1;
//	uint16_t Statusword,Controlword;
//	UNS8 temp;
	d = (CO_Data *)pvParameters;//传递的形参	
	
	//重新配置master PDO
	ExpectedSize = sizeof (UNS32);
	
	COBID = 0x180+nodeId;//主站 Receive = 从站 Transmit
	writeLocalDict( d, /*CO_Data* d*/
			0x1400, /*UNS16 index*/
			0x01, /*UNS8 subind*/ 
			&COBID, /*void * pSourceData,*/ 
			&ExpectedSize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */
	
	COBID = 0x280+nodeId;//主站 Receive = 从站 Transmit
	writeLocalDict( d, /*CO_Data* d*/
			0x1401, /*UNS16 index*/
			0x01, /*UNS8 subind*/ 
			&COBID, /*void * pSourceData,*/ 
			&ExpectedSize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */	
	
	COBID = 0x380+nodeId;//主站 Receive = 从站 Transmit
	writeLocalDict( d, /*CO_Data* d*/
			0x1402, /*UNS16 index*/
			0x01, /*UNS8 subind*/ 
			&COBID, /*void * pSourceData,*/ 
			&ExpectedSize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */	
			
	COBID = 0x480+nodeId;//主站 Receive = 从站 Transmit
	writeLocalDict( d, /*CO_Data* d*/
			0x1403, /*UNS16 index*/
			0x01, /*UNS8 subind*/ 
			&COBID, /*void * pSourceData,*/ 
			&ExpectedSize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */			
	
	COBID = 0x40000200+nodeId;//主站 Transmit = 从站 Receive	
	writeLocalDict( d, /*CO_Data* d*/
			0x1800, /*UNS16 index*/
			0x01, /*UNS8 subind*/ 
			&COBID, /*void * pSourceData,*/ 
			&ExpectedSize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */
	
	COBID = 0xC0000300+nodeId;//主站 Transmit = 从站 Receive	
	writeLocalDict( d, /*CO_Data* d*/
			0x1801, /*UNS16 index*/
			0x01, /*UNS8 subind*/ 
			&COBID, /*void * pSourceData,*/ 
			&ExpectedSize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */	
	
	COBID = 0xC0000400+nodeId;//主站 Transmit = 从站 Receive	
	writeLocalDict( d, /*CO_Data* d*/
			0x1802, /*UNS16 index*/
			0x01, /*UNS8 subind*/ 
			&COBID, /*void * pSourceData,*/ 
			&ExpectedSize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */		
	
	COBID = 0xC0000500+nodeId;//主站 Transmit = 从站 Receive	
	writeLocalDict( d, /*CO_Data* d*/
			0x1803, /*UNS16 index*/
			0x01, /*UNS8 subind*/ 
			&COBID, /*void * pSourceData,*/ 
			&ExpectedSize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */		
	
	ExpectedSize= sizeof(UNS8);
	
	Transmission_Type = TRANS_EVENT_PROFILE;
	writeLocalDict( d, /*CO_Data* d*/
			0x1400, /*UNS16 index*/
			0x02, /*UNS8 subind*/ 
			&Transmission_Type, /*void * pSourceData,*/ 
			&ExpectedSize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */	
			
	Transmission_Type = TRANS_EVENT_PROFILE;
	writeLocalDict( d, /*CO_Data* d*/
			0x1401, /*UNS16 index*/
			0x02, /*UNS8 subind*/ 
			&Transmission_Type, /*void * pSourceData,*/ 
			&ExpectedSize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */				
			
	Transmission_Type = TRANS_EVENT_PROFILE;//asynchronous
	writeLocalDict( d, /*CO_Data* d*/
			0x1800, /*UNS16 index*/
			0x02, /*UNS8 subind*/ 
			&Transmission_Type, /*void * pSourceData,*/ 
			&ExpectedSize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */

	ExpectedSize= sizeof(UNS16);
	inhibittime = 0;//master发送禁止时间为0
	writeLocalDict( d, /*CO_Data* d*/
			0x1800, /*UNS16 index*/
			0x03, /*UNS8 subind*/ 
			&inhibittime, /*void * pSourceData,*/ 
			&ExpectedSize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */	

	highestSubIndex = 2;
	ExpectedSize = sizeof(UNS8);
	writeLocalDict( d, /*CO_Data* d*/
			0x1600, /*UNS16 index*/
			0x00, /*UNS8 subind*/ 
			&highestSubIndex, /*void * pSourceData,*/ 
			&ExpectedSize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */

	_obj = 0x60640020;//实际位置 32位
	ExpectedSize = sizeof(UNS32);
	writeLocalDict( d, /*CO_Data* d*/
			0x1600, /*UNS16 index*/
			0x01, /*UNS8 subind*/ 
			&_obj, /*void * pSourceData,*/ 
			&ExpectedSize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */
			
	_obj = 0x60780010;//实际电流 16位
	ExpectedSize = sizeof(UNS32);
	writeLocalDict( d, /*CO_Data* d*/
			0x1600, /*UNS16 index*/
			0x02, /*UNS8 subind*/ 
			&_obj, /*void * pSourceData,*/ 
			&ExpectedSize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */			

	highestSubIndex = 1;
	ExpectedSize = sizeof(UNS8);
	writeLocalDict( d, /*CO_Data* d*/
			0x1601, /*UNS16 index*/
			0x00, /*UNS8 subind*/ 
			&highestSubIndex, /*void * pSourceData,*/ 
			&ExpectedSize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */

	_obj = 0x60410010;// 状态字 16位
	ExpectedSize = sizeof(UNS32);
	writeLocalDict( d, /*CO_Data* d*/
			0x1601, /*UNS16 index*/
			0x01, /*UNS8 subind*/ 
			&_obj, /*void * pSourceData,*/ 
			&ExpectedSize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */
			

	highestSubIndex = 1;
	ExpectedSize = sizeof(UNS8);
	writeLocalDict( d, /*CO_Data* d*/
			0x1A00, /*UNS16 index*/
			0x00, /*UNS8 subind*/ 
			&highestSubIndex, /*void * pSourceData,*/ 
			&ExpectedSize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */	
			
	_obj = 0x20620020;//PM设定值
	ExpectedSize = sizeof(UNS32);
	writeLocalDict( d, /*CO_Data* d*/
			0x1A00, /*UNS16 index*/
			0x01, /*UNS8 subind*/ 
			&_obj, /*void * pSourceData,*/ 
			&ExpectedSize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */


	while(1)//一直检测，直到设备在线
	{
		if(ReadSDO(d,nodeId,0x6041,0,&Statusword,0) == SDO_FINISHED)
		{
			break;
		}
		else
		{
			closeSDOtransfer(d,nodeId,SDO_CLIENT);			
		}
//		vTaskDelay(10);
	}
//	printf("Statusword: %x\r\n",Statusword);
	if(Statusword&0x0008)//driver has error
	{
		Controlword = 0x0080;
		WriteSDO(d,nodeId,0x6040,0,&Controlword,0);
	}						
	
	masterSendNMTstateChange(d,	nodeId,NMT_Enter_PreOperational);
	vTaskDelay(10);
	//配置PDO
	//设置PDO传输类型
	COBID = 0x200+nodeId;
	WriteSDO(d,nodeId,0x1400,0x01,&COBID,0);//设置RPDO1
	WriteSDO(d,nodeId,0x1400,0x02,&Transmission_Type,0);//设置RPDO1传输类型为非同步循环模式
	
	highestSubIndex = 0;
	_obj = 0x20620020;	
	WriteSDO(d,nodeId,0x1600,0x00,&highestSubIndex,0);//设置RPDO1
	WriteSDO(d,nodeId,0x1600,0x01,&_obj,0);//设置RPDO1	
	highestSubIndex = 1;
	WriteSDO(d,nodeId,0x1600,0x00,&highestSubIndex,0);//设置RPDO1	
	
	//每2ms传送一次位置及电流
	COBID = 0x40000180+nodeId;
	inhibittime = 20;
	WriteSDO(d,nodeId,0x1800,0x01,&COBID,0);//设置TPDO1
	WriteSDO(d,nodeId,0x1800,0x02,&Transmission_Type,0);//设置TPDO1传输类型为非同步循环模式
	WriteSDO(d,nodeId,0x1800,0x03,&inhibittime,0);//设置inhibit time 10ms
	
	//实时更新状态字
	COBID = 0x40000280+nodeId;
	inhibittime = 0;	
	WriteSDO(d,nodeId,0x1801,0x01,&COBID,0);//设置TPDO2
	WriteSDO(d,nodeId,0x1801,0x02,&Transmission_Type,0);//设置TPDO1传输类型为非同步循环模式
	WriteSDO(d,nodeId,0x1801,0x03,&inhibittime,0);//禁止inhibit time 

	COBID = 0xC0000380+nodeId;	
	WriteSDO(d,nodeId,0x1802,0x01,&COBID,0);//设置TPDO3	

	COBID = 0xC0000480+nodeId;	
	WriteSDO(d,nodeId,0x1803,0x01,&COBID,0);//设置TPDO4

	highestSubIndex = 0;
	_obj = 0x60640020;	
	WriteSDO(d,nodeId,0x1A00,0x00,&highestSubIndex,0);//设置TPDO1
	WriteSDO(d,nodeId,0x1A00,0x01,&_obj,0);//设置TPDO1
	_obj = 0x60780010;		
	WriteSDO(d,nodeId,0x1A00,0x02,&_obj,0);//设置TPDO1
	highestSubIndex = 2;
	WriteSDO(d,nodeId,0x1A00,0x00,&highestSubIndex,0);//设置TPDO1		

	highestSubIndex = 0;
	_obj = 0x60410010;	
	WriteSDO(d,nodeId,0x1A01,0x00,&highestSubIndex,0);//设置TPDO2
	WriteSDO(d,nodeId,0x1A01,0x01,&_obj,0);//设置TPDO2
	highestSubIndex = 1;
	WriteSDO(d,nodeId,0x1A01,0x00,&highestSubIndex,0);//设置TPDO2		


	setState(d,Operational);//master进入Operational 
	vTaskDelay(10);	
	masterSendNMTstateChange(d,	nodeId,NMT_Start_Node);//slave进入Operational 
	vTaskDelay(10);	

	
	//PPM
	//写控制模式
	Modes_of_Operation = 1;
	WriteSDO(d,nodeId,0x6060,0x00,&Modes_of_Operation,0);

	Target_Position = 0;
	WriteSDO(d,nodeId,0x607A,0x00,&Target_Position,0);	
	
	Profile_Velocity = 300;
	WriteSDO(d,nodeId,0x6081,0x00,&Profile_Velocity,0);

	Profile_Acceleration = 5000;
	WriteSDO(d,nodeId,0x6083,0x00,&Profile_Acceleration,0);	
	
	Profile_Deceleration = 5000;
	WriteSDO(d,nodeId,0x6084,0x00,&Profile_Deceleration,0);		
	
	Motion_Profile_Type = 1;
	WriteSDO(d,nodeId,0x6086,0x00,&Motion_Profile_Type,0);			

//处理设备状态机
	//switch on Disable
	Controlword = 0x0000;
	WriteSDO(d,nodeId,0x6040,0x00,&Controlword,0);	
	vTaskDelay(10);	
	while((Statusword&0x0140)!=0x0140)
	{
	}	
	//Read to Switch On
	Controlword = 0x0006;
	WriteSDO(d,nodeId,0x6040,0x00,&Controlword,0);
	vTaskDelay(10);		
	while((Statusword&0x0121)!=0x0121)
	{
	}	

	//Switch On & Enable Operation
	Controlword = 0x000F;
	WriteSDO(d,nodeId,0x6040,0x00,&Controlword,0);	
	vTaskDelay(10);		
	while((Statusword&0x0137)!=0x0137)
	{
	}	
	
	Controlword = 0x003F;
	WriteSDO(d,nodeId,0x6040,0x00,&Controlword,0);
	vTaskDelay(10);	
	while((Statusword&0x0400)!=0x0400)
	{
	}
	
//	Modes_of_Operation = -1;//PM
//	WriteSDO(d,nodeId,0x6060,0x00,&Modes_of_Operation,0);	
	vTaskDelay(10);	
  
	xTaskCreate(Linearmotor_thread, "Linearmotor_control", Linearmotor_THREAD_STACK,CO_D.CO_CAN1,EOPS2THREAD_PRIO, &xH_Linearmotor_control);	
	//删除任务
	vTaskDelete(xH_EPOS2init);
	xH_EPOS2init = NULL;

}

static void Linearmotor_thread(void *pvParameters)
{
	CO_Data *d;

	UNS8 nodeId = 1;
	d = (CO_Data *)pvParameters;//传递的形参	

	
	//找原点	
	Home_Offset = 3000;
	WriteSDO(d,nodeId,0x607C,0x00,&Home_Offset,0);	

	Homing_Speeds_Speed_for_Switch_Search = 100;
	WriteSDO(d,nodeId,0x6099,0x01,&Homing_Speeds_Speed_for_Switch_Search,0);		
	
	Homing_Speeds_Speed_for_Zero_Search = 100;
	WriteSDO(d,nodeId,0x6099,0x02,&Homing_Speeds_Speed_for_Zero_Search,0);		

	Homing_Acceleration = 1000;
	WriteSDO(d,nodeId,0x609A,0x00,&Homing_Acceleration,0);			

	Current_Threshold_for_Homing_Mode = 2000;
	WriteSDO(d,nodeId,0x2080,0x00,&Current_Threshold_for_Homing_Mode,0);				

	Home_Position = 0;
	WriteSDO(d,nodeId,0x2081,0x00,&Home_Position,0);		
	
	Homing_Method = -4;
	WriteSDO(d,nodeId,0x6098,0x00,&Homing_Method,0);			
	
	Modes_of_Operation = 6;//HM
	WriteSDO(d,nodeId,0x6060,0x00,&Modes_of_Operation,0);			
	
	Controlword = 0x000F;
	WriteSDO(d,nodeId,0x6040,0x00,&Controlword,0);
	vTaskDelay(10);		
	Controlword = 0x001F;
	WriteSDO(d,nodeId,0x6040,0x00,&Controlword,0);//执行HM
	
	while((Statusword&0x1400)!=0x1400)
	{
	}		
	
	//切到PPM
	Modes_of_Operation = 1;
	WriteSDO(d,nodeId,0x6060,0x00,&Modes_of_Operation,0);
	
	Target_Position = 50000;
	WriteSDO(d,nodeId,0x607A,0x00,&Target_Position,0);	
	
	Profile_Velocity = 3000;
	WriteSDO(d,nodeId,0x6081,0x00,&Profile_Velocity,0);	
	
	Profile_Acceleration = 30000;
	WriteSDO(d,nodeId,0x6083,0x00,&Profile_Acceleration,0);		

	Profile_Deceleration = 30000;
	WriteSDO(d,nodeId,0x6084,0x00,&Profile_Deceleration,0);			
	
	QuickStop_Deceleration = 30000;
	WriteSDO(d,nodeId,0x6085,0x00,&QuickStop_Deceleration,0);			
	
	Motion_Profile_Type = 1;
	WriteSDO(d,nodeId,0x6086,0x00,&Motion_Profile_Type,0);		
	
	vTaskDelay(10);	
  while(1)
  {
		Controlword = 0x003F;
		WriteSDO(d,nodeId,0x6040,0x00,&Controlword,0);
		vTaskDelay(10);	
		while((Statusword&0x0400)!=0x0400)
		{
		}//已运动到指定位置	
		if(Target_Position == 50000)
		{
			Target_Position = 0;		
		}
		else if(Target_Position == 0)
		{
			Target_Position = 50000;			
		}
		WriteSDO(d,nodeId,0x607A,0x00,&Target_Position,0);
//		vTaskDelay(10);			
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
