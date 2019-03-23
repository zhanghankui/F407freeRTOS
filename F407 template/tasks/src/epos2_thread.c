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
	if(readNetworkDict(d,nodeId,index,subIndex,bDataType,useBlockMode) == 0)//���ͳɹ�
	{
		Startsystimerforovertime();//����ϵͳ��ʱ��
		while(1)
		{
			taskENTER_CRITICAL();   /* �����ٽ��� */	
			SDO_Readstate  = getReadResultNetworkDict(d,nodeId,data,&datasize,&abortCode);
			taskEXIT_CRITICAL();  	/* �˳��ٽ��� */
			if(COMMovertimer == true)
			{
//				printf("COMM overtime\r\n");
				return 0xEE;//��Ӧ��ʱ
			}
			if(SDO_Readstate != SDO_UPLOADstate)//�յ����ػ��쳣
			{
				if(SDO_Readstate == SDO_ABORTED_RCV)
				{
					//���������
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
	if(writeNetworkDict(d,nodeId,index,subIndex,count,bDataType,data,useBlockMode) == 0)//���ͳɹ�
	{
		Startsystimerforovertime();//����ϵͳ��ʱ��
		while(1)
		{
			taskENTER_CRITICAL();   /* �����ٽ��� */
			SDO_Writestate  = getWriteResultNetworkDict(d,nodeId,&abortCode);
			taskEXIT_CRITICAL();  	/* �˳��ٽ��� */
			if(COMMovertimer == true)
			{
//				printf("COMM overtime\r\n");
				return 0xEE;//��Ӧ��ʱ
			}
			if(SDO_Writestate != SDO_DOWNLOADstate)//�յ����ػ��쳣
			{
				if(SDO_Writestate != SDO_FINISHED)
				{
					//���������
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
	UNS32 _obj;
	UNS8 nodeId = 1;
	uint16_t Statusword,Controlword;
	d = (CO_Data *)pvParameters;//���ݵ��β�	

	while(1)//һֱ��⣬ֱ���豸����
	{
		if(ReadSDO(d,nodeId,0x6041,0,&Statusword,0) != 0xEE)//��Ӧ��
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
	
	//��������master PDO
	ExpectedSize = sizeof (UNS32);
	
//	COBID = 0x180+nodeId;//��վ Receive = ��վ Transmit
//	writeLocalDict( d, /*CO_Data* d*/
//			0x1400, /*UNS16 index*/
//			0x01, /*UNS8 subind*/ 
//			&COBID, /*void * pSourceData,*/ 
//			&ExpectedSize, /* UNS8 * pExpectedSize*/
//			RW);  /* UNS8 checkAccess */
	
	COBID = 0xC0000200+nodeId;//��վ Transmit = ��վ Receive	
	writeLocalDict( d, /*CO_Data* d*/
			0x1800, /*UNS16 index*/
			0x01, /*UNS8 subind*/ 
			&COBID, /*void * pSourceData,*/ 
			&ExpectedSize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */
	
	ExpectedSize= sizeof(UNS8);
	
//	Transmission_Type = TRANS_EVENT_PROFILE;
//	writeLocalDict( d, /*CO_Data* d*/
//			0x1400, /*UNS16 index*/
//			0x02, /*UNS8 subind*/ 
//			&Transmission_Type, /*void * pSourceData,*/ 
//			&ExpectedSize, /* UNS8 * pExpectedSize*/
//			RW);  /* UNS8 checkAccess */	
			
	Transmission_Type = TRANS_EVENT_PROFILE;//asynchronous
	writeLocalDict( d, /*CO_Data* d*/
			0x1800, /*UNS16 index*/
			0x02, /*UNS8 subind*/ 
			&Transmission_Type, /*void * pSourceData,*/ 
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
	
	//����PDO
	//����PDO��������
	COBID &= 0x7FF;
	WriteSDO(d,nodeId,0x1400,0x01,&COBID,0);//����RPDO1
	WriteSDO(d,nodeId,0x1400,0x02,&Transmission_Type,0);//����RPDO1��������Ϊ��ͬ��ѭ��ģʽ
	
	highestSubIndex = 0;
	WriteSDO(d,nodeId,0x1600,0x00,&highestSubIndex,0);//����RPDO1
	WriteSDO(d,nodeId,0x1600,0x01,&_obj,0);//����RPDO1	
	highestSubIndex = 1;
	WriteSDO(d,nodeId,0x1600,0x00,&highestSubIndex,0);//����RPDO1	
	

	//ɾ������
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
