#ifndef __GLOBAL_STRUCT_H__
#define	__GLOBAL_STRUCT_H__
//#include "lwip/api.h"

#define CANOpen_THREAD_PRIO       (configMAX_PRIORITIES-1)              //CANopen���ݴ�������Ϊ������ȼ�
#define EOPS2THREAD_PRIO       (configMAX_PRIORITIES-3)              //CANopen���ݴ�������Ϊ������ȼ�

//#define TIMER_THEAD_PRIO          (configMAX_PRIORITIES-3)              //��ʱ���������ȼ�
//#define BODY_CONTROL_THREAD_PRIO        (configMAX_PRIORITIES-4)
//#define ARM_CONTROL_THREAD_PRIO			(configMAX_PRIORITIES-5)
//#define MOTION_THREAD_PRIO              (configMAX_PRIORITIES-6) 
//#define TCP_THREAD_PRIO           ( tskIDLE_PRIORITY + 3 )			   //TCP �������ȼ�
//#define FEED_BACK_PRIO            ( tskIDLE_PRIORITY + 4 ) 			   //feedback �������ȼ�
//#define USERPROCESS_THREAD_PRIO	  ( tskIDLE_PRIORITY + 5 )             //���ݴ����������ȼ�

//#define BODY_CONTROL_THREAD_STACK        2000
//#define ARM_CONTROL_THREAD_STACK		 2000

//#define ROBOT_BODY               1
//#define ROBOT_WITH_ARM           0

//#define MAX_CLIENT_NUM           3                        //�������ӵĿͻ�����Ŀ����
//#define MAC_COMM_COMM            10                       // ����ͬǰ׺����������� �� drive�� armset��
//#define NET_BUFFER_MAX_NUMBER  1000                       //���緵�����ݵ����ֵ

///********Client ������ģʽ��0Ϊ������ֵ��1Ϊ�ٿ�ģʽ 2Ϊ���ģʽֻ�ɽ�������*/
//#define NET_CLINET_USER_MODE_FREE   0
//#define NET_CLIENT_USER_MODE_WR     1
//#define NET_CLIENT_USER_MODE_RO     2

//       /*��������     ���Ĵ�����      �ڱۿ���     �����ۿ���    ��е�ֿ���*/
//extern xQueueHandle xQ_DRIVE_COMM, xQ_FLIP_COMM, xQ_ARM_COMM,xQ_HAND_COMM;
//extern const char* gNetCommandResStr[];
//extern char gNetBuffer[NET_BUFFER_MAX_NUMBER];
//extern int gNetDataSize;
////ÿһ�����ӵ�TCP client��Ϣ
//typedef struct _netclientuserinfostruct
//{
//    char is_used;              //user id
//	char req;                  //required flag
//    char send_flag;	
//	char mode;                 //user mode
//	int  user_id;
//    int period;                //feedback period in ms
//	struct netconn *conn;
//    
//}NET_CLIENT_USER_INFO_STRUCT;

//extern NET_CLIENT_USER_INFO_STRUCT user_info[MAX_CLIENT_NUM];

///*--------------------------------------------------------------------------*/
///*
//** NET COMMAND RESPONSE STATE
//** �����յ�һ������ʱ���ᷴ�����յ��������״̬
//**
//*/
///*--------------------------------------------------------------------------*/
//#define NET_COMMAND_RESPONSE_INVALID  0    //invalid command
//#define NET_COMMAND_RESPONSE_REACHED  1    //reach the target
//#define NET_COMMAND_RESPONSE_CANCELED 2    //canceled
//#define NET_COMMAND_RESPONSE_PUSHED   3    //push by other command
//#define NET_COMMAND_RESPONSE_OK       4    //invalid command
//#define NET_COMMAND_RESPONSE_FAILED   5    //invalid command
//#define NET_COMMAND_RESPONSE_READONLY 6    //read only mode
//#define NET_COMMAND_RESPONSE_OVERRUN  7    //message pool overrun




///*
//** drive ��ģʽ
//**
//*/
//#define NET_ROBOT_DRIVER_TYPE_DIRECT    1
//#define NET_ROBOT_DRIVER_TYPE_INDIRECT  2
//#define NET_ROBOT_DRIVER_TYPE_GOAHEAD   3
//#define NET_ROBOT_DRIVER_TYPE_GOROTATE  4
//#define NET_ROBOT_DRIVER_TYPE_POWER_OFF  5

//#define NET_ROBOT_DRIVER_MODE_CURRENT  1
//#define NET_ROBOT_DRIVER_MODE_VELOCITY 2
//#define NET_ROBOT_DRIVER_MODE_POSITION 3

///*
//***********�������Ĵ�������Ϣ
//*/
//typedef struct _netrobotdrivercontrolstruct
//{
//    char type;//command type
//    char mode;//run mode
//    int  left_c;//left driver setting value
//	int  right_c;//right driver setting value
//	int  left_v;//left driver setting value
//	int  right_v;//right driver setting value
//    int  velocity;//forward velocity in indirect
//    int  yawrate;//yaw rate in indirect
//	int  left_p;//left driver setting value
//	int  right_p;//right driver setting value
//    int  go_distance;
//    int  go_degree;
//	char norm;//normalized flag
//	char push;//push flag
//}NET_ROBOT_DRIVER_CONTROL_STRUCT;
///*
//********����ڱ۵Ŀ�����Ϣ
//*/
//#define NET_ROBOT_FLIPPER_TYPE_FREE      0
//#define NET_ROBOT_FLIPPER_TYPE_DIRECT    1
//#define NET_ROBOT_FLIPPER_TYPE_SLOP      2
//#define NET_ROBOT_FLIPPER_TYPE_STAIR     3
//#define NET_ROBOT_FLIPPER_TYPE_ABSOLUTE  4
//#define NET_ROBOT_FLIPPER_TYPE_POWER_OFF     5

//#define NET_ROBOT_FLIPPER_MODE_FREE     0
//#define NET_ROBOT_FLIPPER_MODE_CURRENT  1
//#define NET_ROBOT_FLIPPER_MODE_VELOCITY 2
//#define NET_ROBOT_FLIPPER_MODE_POSITION 3
//#define NET_ROBOT_FLIPPER_MODE_HOME     4

//#define NET_ROBOT_FLIPPER_MODE_SLOP_UP   11
//#define NET_ROBOT_FLIPPER_MODE_SLOP_DOWN 12

//#define NET_ROBOT_FLIPPER_MODE_STAIR_UP   21
//#define NET_ROBOT_FLIPPER_MODE_STAIR_DOWN 22
//typedef struct _netrobotflippercontrolstruct
//{
//    char front_type;//command type
//    char back_type;//command type
//    char front_mode;//run mode
//    char back_mode;//run mode
//    int  leftfront;//leftfront  setting value
//	int  rightfront;//rightfront  setting value
//	int  leftback;//leftback  setting value
//	int  rightback;//rightback  setting value
//    float degree;//
//    int velocity;//track velocity in auto mode
//    char front_norm;//front norm flag
//    char back_norm;//back norm flag
//    char push;//push flag
//}NET_ROBOT_FLIPPER_CONTROL_STRUCT;
///*
//*********��������۵Ŀ�����Ϣ
//*/
//#define NET_ROBOT_ARM_TYPE_DIRECT    1
//#define NET_ROBOT_ARM_TYPE_LOCAL     2

//#define NET_ROBOT_ARM_MODE_CURRENT  1
//#define NET_ROBOT_ARM_MODE_VELOCITY 2
//#define NET_ROBOT_ARM_MODE_POSITION 3
//#define NET_ROBOT_ARM_MODE_HOME     4

//#define NET_ROBOT_ARM_MODE_LOCAL_SENSOR_POSITION 11
//typedef struct _netrobotarmcontrolstruct
//{
//    char type;               //command type
//    char mode;               //run mode
//    int joint1;
//    int joint2;
//    int pitch;
//    int roll;
//    int gripper;
//    char norm;//norm flag
//    char push;//push flag
//    int x;
//    int z;
//    int yaw;
//}NET_ROBOT_ARM_CONTROL_STRUCT;

///*
//*************����ĩ��ִ�л����Ŀ�����Ϣ
//*/
//typedef struct _netrobothandcontrolstruct
//{
//    char type;               //command type
//    char mode;               //run mode
//    int joint1;
//    int joint2;
//    //int joint2;
//    int joint3;
//    int joint4;
//    char norm;//norm flag
//    char push;//push flag
//}NET_ROBOT_HAND_CONTROL_STRUCT;
///*
//**********        ���MSG�ṹ	*******************
//**********drive flipper arm hand*******************
//*/
//typedef struct _netrobotdrivercontrolmessagestruct{
//    int user_id;              //user id
//    NET_ROBOT_DRIVER_CONTROL_STRUCT DATA;
//    int command_nubmer;
//    int state;
//}NET_ROBOT_CONTROL_DRIVER_MESSAGE_STRUCT;

//typedef struct _netrobotflippercontrolmessagestruct{
//    int user_id;              //user id
//    NET_ROBOT_FLIPPER_CONTROL_STRUCT DATA;
//    int command_nubmer;
//    int state;
//}NET_ROBOT_CONTROL_FLIPPER_MESSAGE_STRUCT;

//typedef struct _netrobotarmcontrolmessagestruct{
//    int user_id;                //user id
//    NET_ROBOT_ARM_CONTROL_STRUCT DATA;
//    int command_nubmer;
//    int state;
//}NET_ROBOT_CONTROL_ARM_MESSAGE_STRUCT;

//typedef struct _netrobothandcontrolmessagestruct{
//    int user_id;                //user id
//    NET_ROBOT_HAND_CONTROL_STRUCT DATA;
//    int command_nubmer;
//    int state;
//}NET_ROBOT_CONTROL_HAND_MESSAGE_STRUCT;

//NET_CLIENT_USER_INFO_STRUCT* NetUserSearchBySocket(struct netconn * sock);
#endif
