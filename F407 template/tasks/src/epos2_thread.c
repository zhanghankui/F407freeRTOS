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
static void Admittance_control_thread(void *pvParameters);


// **************************************************************************
// the globals

#define EPOS2init_THREAD_STACK        512
#define Admittance_control_THREAD_STACK        512

xTaskHandle  xH_EPOS2init = NULL;
xTaskHandle  xH_Admittance_control = NULL;
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
	UNS16 inhibittime;
	UNS32 _obj;
	UNS8 nodeId = 1;
//	uint16_t Statusword,Controlword;
//	UNS8 temp;
	d = (CO_Data *)pvParameters;//���ݵ��β�	
	
	//��������master PDO
	ExpectedSize = sizeof (UNS32);
	
	COBID = 0x180+nodeId;//��վ Receive = ��վ Transmit
	writeLocalDict( d, /*CO_Data* d*/
			0x1400, /*UNS16 index*/
			0x01, /*UNS8 subind*/ 
			&COBID, /*void * pSourceData,*/ 
			&ExpectedSize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */
	
	COBID = 0x280+nodeId;//��վ Receive = ��վ Transmit
	writeLocalDict( d, /*CO_Data* d*/
			0x1401, /*UNS16 index*/
			0x01, /*UNS8 subind*/ 
			&COBID, /*void * pSourceData,*/ 
			&ExpectedSize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */	
	
	COBID = 0x380+nodeId;//��վ Receive = ��վ Transmit
	writeLocalDict( d, /*CO_Data* d*/
			0x1402, /*UNS16 index*/
			0x01, /*UNS8 subind*/ 
			&COBID, /*void * pSourceData,*/ 
			&ExpectedSize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */	
			
	COBID = 0x480+nodeId;//��վ Receive = ��վ Transmit
	writeLocalDict( d, /*CO_Data* d*/
			0x1403, /*UNS16 index*/
			0x01, /*UNS8 subind*/ 
			&COBID, /*void * pSourceData,*/ 
			&ExpectedSize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */			
	
	COBID = 0x40000200+nodeId;//��վ Transmit = ��վ Receive	
	writeLocalDict( d, /*CO_Data* d*/
			0x1800, /*UNS16 index*/
			0x01, /*UNS8 subind*/ 
			&COBID, /*void * pSourceData,*/ 
			&ExpectedSize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */
	
	COBID = 0xC0000300+nodeId;//��վ Transmit = ��վ Receive	
	writeLocalDict( d, /*CO_Data* d*/
			0x1801, /*UNS16 index*/
			0x01, /*UNS8 subind*/ 
			&COBID, /*void * pSourceData,*/ 
			&ExpectedSize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */	
	
	COBID = 0xC0000400+nodeId;//��վ Transmit = ��վ Receive	
	writeLocalDict( d, /*CO_Data* d*/
			0x1802, /*UNS16 index*/
			0x01, /*UNS8 subind*/ 
			&COBID, /*void * pSourceData,*/ 
			&ExpectedSize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */		
	
	COBID = 0xC0000500+nodeId;//��վ Transmit = ��վ Receive	
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
	inhibittime = 0;//master���ͽ�ֹʱ��Ϊ0
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

	_obj = 0x60640020;//ʵ��λ�� 32λ
	ExpectedSize = sizeof(UNS32);
	writeLocalDict( d, /*CO_Data* d*/
			0x1600, /*UNS16 index*/
			0x01, /*UNS8 subind*/ 
			&_obj, /*void * pSourceData,*/ 
			&ExpectedSize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */
			
	_obj = 0x60780010;//ʵ�ʵ��� 16λ
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

	_obj = 0x60410010;// ״̬�� 16λ
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
			
	_obj = 0x20620020;//PM�趨ֵ
	ExpectedSize = sizeof(UNS32);
	writeLocalDict( d, /*CO_Data* d*/
			0x1A00, /*UNS16 index*/
			0x01, /*UNS8 subind*/ 
			&_obj, /*void * pSourceData,*/ 
			&ExpectedSize, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */


	while(1)//һֱ��⣬ֱ���豸����
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
	//����PDO
	//����PDO��������
	COBID = 0x200+nodeId;
	WriteSDO(d,nodeId,0x1400,0x01,&COBID,0);//����RPDO1
	WriteSDO(d,nodeId,0x1400,0x02,&Transmission_Type,0);//����RPDO1��������Ϊ��ͬ��ѭ��ģʽ
	
	highestSubIndex = 0;
	_obj = 0x20620020;	
	WriteSDO(d,nodeId,0x1600,0x00,&highestSubIndex,0);//����RPDO1
	WriteSDO(d,nodeId,0x1600,0x01,&_obj,0);//����RPDO1	
	highestSubIndex = 1;
	WriteSDO(d,nodeId,0x1600,0x00,&highestSubIndex,0);//����RPDO1	
	
	//ÿ2ms����һ��λ�ü�����
	COBID = 0x40000180+nodeId;
	inhibittime = 20;
	WriteSDO(d,nodeId,0x1800,0x01,&COBID,0);//����TPDO1
	WriteSDO(d,nodeId,0x1800,0x02,&Transmission_Type,0);//����TPDO1��������Ϊ��ͬ��ѭ��ģʽ
	WriteSDO(d,nodeId,0x1800,0x03,&inhibittime,0);//����inhibit time 10ms
	
	//ʵʱ����״̬��
	COBID = 0x40000280+nodeId;
	inhibittime = 0;	
	WriteSDO(d,nodeId,0x1801,0x01,&COBID,0);//����TPDO2
	WriteSDO(d,nodeId,0x1801,0x02,&Transmission_Type,0);//����TPDO1��������Ϊ��ͬ��ѭ��ģʽ
	WriteSDO(d,nodeId,0x1801,0x03,&inhibittime,0);//��ֹinhibit time 

	COBID = 0xC0000380+nodeId;	
	WriteSDO(d,nodeId,0x1802,0x01,&COBID,0);//����TPDO3	

	COBID = 0xC0000480+nodeId;	
	WriteSDO(d,nodeId,0x1803,0x01,&COBID,0);//����TPDO4

	highestSubIndex = 0;
	_obj = 0x60640020;	
	WriteSDO(d,nodeId,0x1A00,0x00,&highestSubIndex,0);//����TPDO1
	WriteSDO(d,nodeId,0x1A00,0x01,&_obj,0);//����TPDO1
	_obj = 0x60780010;		
	WriteSDO(d,nodeId,0x1A00,0x02,&_obj,0);//����TPDO1
	highestSubIndex = 2;
	WriteSDO(d,nodeId,0x1A00,0x00,&highestSubIndex,0);//����TPDO1		

	highestSubIndex = 0;
	_obj = 0x60410010;	
	WriteSDO(d,nodeId,0x1A01,0x00,&highestSubIndex,0);//����TPDO2
	WriteSDO(d,nodeId,0x1A01,0x01,&_obj,0);//����TPDO2
	highestSubIndex = 1;
	WriteSDO(d,nodeId,0x1A01,0x00,&highestSubIndex,0);//����TPDO2		


	setState(d,Operational);//master����Operational 
	vTaskDelay(10);	
	masterSendNMTstateChange(d,	nodeId,NMT_Start_Node);//slave����Operational 
	vTaskDelay(10);	

	
	//PPM
	//д����ģʽ
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

//�����豸״̬��
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
	
	Modes_of_Operation = -1;//PM
	WriteSDO(d,nodeId,0x6060,0x00,&Modes_of_Operation,0);	
	vTaskDelay(10);	
  
	xTaskCreate(Admittance_control_thread, "Admittance_control", Admittance_control_THREAD_STACK, NULL,EOPS2THREAD_PRIO, &xH_Admittance_control);	
	//ɾ������
	vTaskDelete(xH_EPOS2init);
	xH_EPOS2init = NULL;

}

static void Admittance_control_thread(void *pvParameters)
{
  int32_t last_position=0;
  int32_t position; 
  int16_t current;
  REAL32 Fext;
  REAL32 velocity;
  REAL32 M=6000.0f,B=800.0f,K=0.3f;
  REAL32 xd=0.0f,dxd=0.0f,ddxd=0.0f;
//	uint16_t Statusword,Controlword;
//	UNS8 temp;
	vTaskDelay(10);	
  while(1)
  {
    position = Position_Actual_Value;
    current = -Current_Actual_Value;//��������������෴
    Fext = (current*0.9f+Fext*0.1f)*0.2f;//��Ľ�Ϊbutter worth�˲�
//    if(fabsf(Fext)>250.0f)
//    {
//      Fext = 0;
//    }
    velocity = (position - last_position)/2.0f;//unit: pulse/ms
    last_position = position;

    ddxd = (Fext-K*position-B*velocity)/M;
    dxd +=ddxd;
    xd +=dxd;
    Position_Mode_Setting_Value = (int32_t)xd;
  	vTaskDelay(20);	    
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
