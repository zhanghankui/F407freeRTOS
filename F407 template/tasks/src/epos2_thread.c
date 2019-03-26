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

#include "systimer.h"
// **************************************************************************
// the defines


// **************************************************************************
// the typedefs	


// **************************************************************************
//declaration static function prototypes



// **************************************************************************
// the globals

#define EPOS2init_THREAD_STACK        512

xTaskHandle  xH_EPOS2init = NULL;
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
				if(SDO_Readstate == SDO_ABORTED_RCV)
				{
					//处理错误码
				}	
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
	uint16_t Statusword,Controlword;
	d = (CO_Data *)pvParameters;//传递的形参	

	while(1)//一直检测，直到设备在线
	{
		if(ReadSDO(d,nodeId,0x6041,0,&Statusword,0) != 0xEE)//无应答
		{
//			printf("get Statusword\r\n");
			break;
		}
		closeSDOtransfer(d,nodeId,SDO_CLIENT);
//		vTaskDelay(10);
	}
//	printf("Statusword: %x\r\n",Statusword);
	if(Statusword&0x0008)//driver has error
	{
		Controlword = 0x0080;
		WriteSDO(d,nodeId,0x6040,0,&Controlword,0);
	}
	
	//重新配置master PDO
	ExpectedSize = sizeof (UNS32);
	
	COBID = 0x180+nodeId;//主站 Receive = 从站 Transmit
	writeLocalDict( d, /*CO_Data* d*/
			0x1400, /*UNS16 index*/
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
			
	Transmission_Type = TRANS_EVENT_PROFILE;//asynchronous
	writeLocalDict( d, /*CO_Data* d*/
			0x1800, /*UNS16 index*/
			0x02, /*UNS8 subind*/ 
			&Transmission_Type, /*void * pSourceData,*/ 
			&ExpectedSize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */

	ExpectedSize= sizeof(UNS16);
	inhibittime = 0;
	writeLocalDict( d, /*CO_Data* d*/
			0x1800, /*UNS16 index*/
			0x03, /*UNS8 subind*/ 
			&inhibittime, /*void * pSourceData,*/ 
			&ExpectedSize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */	

	highestSubIndex = 1;
	ExpectedSize = sizeof(UNS8);
	writeLocalDict( d, /*CO_Data* d*/
			0x1600, /*UNS16 index*/
			0x00, /*UNS8 subind*/ 
			&highestSubIndex, /*void * pSourceData,*/ 
			&ExpectedSize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */

	_obj = 0x60640020;
	ExpectedSize = sizeof(UNS32);
	writeLocalDict( d, /*CO_Data* d*/
			0x1600, /*UNS16 index*/
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
			
	_obj = 0x20C10040;
	ExpectedSize = sizeof(UNS32);
	writeLocalDict( d, /*CO_Data* d*/
			0x1A00, /*UNS16 index*/
			0x01, /*UNS8 subind*/ 
			&_obj, /*void * pSourceData,*/ 
			&ExpectedSize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */
	
	masterSendNMTstateChange(d,	nodeId,NMT_Enter_PreOperational);
	vTaskDelay(1);
	//配置PDO
	//设置PDO传输类型
	COBID = 0x200+nodeId;
	WriteSDO(d,nodeId,0x1400,0x01,&COBID,0);//设置RPDO1
	WriteSDO(d,nodeId,0x1400,0x02,&Transmission_Type,0);//设置RPDO1传输类型为非同步循环模式
	
	highestSubIndex = 0;
	_obj = 0x20C10040;	
	WriteSDO(d,nodeId,0x1600,0x00,&highestSubIndex,0);//设置RPDO1
	WriteSDO(d,nodeId,0x1600,0x01,&_obj,0);//设置RPDO1	
	highestSubIndex = 1;
	WriteSDO(d,nodeId,0x1600,0x00,&highestSubIndex,0);//设置RPDO1	
	
	//每10ms传送一次位置速度变化
	COBID = 0x40000180+nodeId;
	inhibittime = 100;
	WriteSDO(d,nodeId,0x1800,0x01,&COBID,0);//设置TPDO1
	WriteSDO(d,nodeId,0x1800,0x02,&Transmission_Type,0);//设置TPDO1传输类型为非同步循环模式
	WriteSDO(d,nodeId,0x1800,0x03,&inhibittime,0);//设置inhibit time 10ms
	
	COBID = 0xC0000280+nodeId;	
	WriteSDO(d,nodeId,0x1801,0x01,&COBID,0);//设置TPDO2
	
	COBID = 0xC0000380+nodeId;	
	WriteSDO(d,nodeId,0x1802,0x01,&COBID,0);//设置TPDO3	

	COBID = 0xC0000480+nodeId;	
	WriteSDO(d,nodeId,0x1803,0x01,&COBID,0);//设置TPDO4

	highestSubIndex = 0;
	_obj = 0x60640020;	
	WriteSDO(d,nodeId,0x1A00,0x00,&highestSubIndex,0);//设置TPDO1
	WriteSDO(d,nodeId,0x1A00,0x01,&_obj,0);//设置TPDO1
	_obj = 0x606C0020;		
	WriteSDO(d,nodeId,0x1A00,0x02,&_obj,0);//设置TPDO1
	highestSubIndex = 2;
	WriteSDO(d,nodeId,0x1A00,0x00,&highestSubIndex,0);//设置TPDO1		


	setState(d,Operational);
	vTaskDelay(1);	
	masterSendNMTstateChange(d,	nodeId,NMT_Start_Node);	
	//删除任务
	vTaskDelete(xH_EPOS2init);
	xH_EPOS2init = NULL;
}


void EPOS2_init(void)
{
	xTaskCreate(EPOS2init_thread, "EPOS2init", EPOS2init_THREAD_STACK, CO_D.CO_CAN1,EOPS2THREAD_PRIO, &xH_EPOS2init);
	if(NULL == xH_EPOS2init)
	{
		printf("EPOS2 init thread created failed!\r\n");
	}
}
