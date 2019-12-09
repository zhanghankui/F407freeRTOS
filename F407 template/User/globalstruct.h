#ifndef __GLOBAL_STRUCT_H__
#define	__GLOBAL_STRUCT_H__
//#include "lwip/api.h"

#define CANOpen_THREAD_PRIO       (configMAX_PRIORITIES-1)              //CANopen数据处理任务定为最高优先级

#define EOPS2THREAD_PRIO       (2)                                      //最低用户任务优先级
#define RWdata_THREAD_PRIO       (1)                                    //RWdata数据处理任务定为最高优先级
//#define TIMER_THEAD_PRIO          (configMAX_PRIORITIES-3)              //定时器任务优先级
//#define BODY_CONTROL_THREAD_PRIO        (configMAX_PRIORITIES-4)
//#define ARM_CONTROL_THREAD_PRIO			(configMAX_PRIORITIES-5)
//#define MOTION_THREAD_PRIO              (configMAX_PRIORITIES-6) 
//#define TCP_THREAD_PRIO           ( tskIDLE_PRIORITY + 3 )			   //TCP 任务优先级
//#define FEED_BACK_PRIO            ( tskIDLE_PRIORITY + 4 ) 			   //feedback 任务优先级
//#define USERPROCESS_THREAD_PRIO	  ( tskIDLE_PRIORITY + 5 )             //数据处理任务优先级

//#define BODY_CONTROL_THREAD_STACK        2000
//#define ARM_CONTROL_THREAD_STACK		 2000

//#define ROBOT_BODY               1
//#define ROBOT_WITH_ARM           0

//#define MAX_CLIENT_NUM           3                        //允许连接的客户端数目上限
//#define MAC_COMM_COMM            10                       // 有相同前缀的命令的条数 如 drive， armset等
//#define NET_BUFFER_MAX_NUMBER  1000                       //网络返回数据的最大值

///********Client 的连接模式，0为不反馈值，1为操控模式 2为监测模式只可接收数据*/
//#define NET_CLINET_USER_MODE_FREE   0
//#define NET_CLIENT_USER_MODE_WR     1
//#define NET_CLIENT_USER_MODE_RO     2

//       /*创建队列     主履带控制      摆臂控制     操作臂控制    机械手控制*/
//extern xQueueHandle xQ_DRIVE_COMM, xQ_FLIP_COMM, xQ_ARM_COMM,xQ_HAND_COMM;
//extern const char* gNetCommandResStr[];
//extern char gNetBuffer[NET_BUFFER_MAX_NUMBER];
//extern int gNetDataSize;
////每一个连接的TCP client信息
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
//** 当接收到一条命令时，会反馈接收到的命令的状态
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
//** drive 的模式
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
//***********保存主履带控制信息
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
//********保存摆臂的控制信息
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
//*********保存操作臂的控制信息
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
//*************保存末端执行机构的控制信息
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
//**********        相关MSG结构	*******************
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
