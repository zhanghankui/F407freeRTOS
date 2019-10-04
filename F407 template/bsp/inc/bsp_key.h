/*
*********************************************************************************************************
* @file    bsp_key.h 
* @author  smilingfrog 
* @company Dafanshu Innovations Co. Ltd
* @version 1.00
* @date    10-July-2018 15:08
* @brief   key

* Update record
* version   date           author          description
* V1.00     10-July-2018   smilingfrog     first release
*********************************************************************************************************
*/	



#ifndef _BSP_KEY_H_
#define _BSP_KEY_H_

// **************************************************************************	
// the includes
#include "sys.h"

//!
//!
//! \defgroup BSP_KEY_H
//!
//@{


#ifdef __cplusplus
extern "C" {
#endif
	
	
// **************************************************************************
// the defines	
#define KEY_COUNT    3	   					/* ��������, 3��������*/
	
/* ����Ӧ�ó���Ĺ��������������� */
#define KEY_DOWN_K0		KEY_0_DOWN
#define KEY_UP_K0		KEY_0_UP
#define KEY_LONG_K0		KEY_0_LONG

#define KEY_DOWN_K1		KEY_1_DOWN
#define KEY_UP_K1		KEY_1_UP
#define KEY_LONG_K1		KEY_1_LONG

#define KEY_DOWN_KWU		KEY_WU_DOWN
#define KEY_UP_KWU		KEY_WU_UP
#define KEY_LONG_KWU		KEY_WU_LONG	
	
	
/*
	�����˲�ʱ��50ms, ��λ10ms��
	ֻ��������⵽50ms״̬�������Ϊ��Ч����������Ͱ��������¼�
	��ʹ������·����Ӳ���˲������˲�����Ҳ���Ա�֤�ɿ��ؼ�⵽�����¼�
*/
#define KEY_FILTER_TIME   4       /* ��λ10ms�� ����40���룬��Ϊ�̰��¼� */   
#define KEY_LONG_TIME     100			/* ��λ10ms�� ����1�룬��Ϊ�����¼� */	
	
// **************************************************************************
// the typedefs		
	
/* ����ID, ��Ҫ����bsp_KeyState()��������ڲ��� */
typedef enum
{
	KID_K0 = 0,
	KID_K1,
	KID_KWU,
}KEY_ID_E;	
	
/*
	ÿ��������Ӧ1��ȫ�ֵĽṹ�������
*/
typedef struct
{
	/* ������һ������ָ�룬ָ���жϰ����ַ��µĺ��� */
	uint8_t (*IsKeyDownFunc)(void); /* �������µ��жϺ���,1��ʾ���� */

	uint8_t  Count;			/* �˲��������� */
	uint16_t LongCount;		/* ���������� */
	uint16_t LongTime;		/* �������³���ʱ��, 0��ʾ����ⳤ�� */
	uint8_t  State;			/* ������ǰ״̬�����»��ǵ��� */
	uint8_t  RepeatSpeed;	/* ������������ */
	uint8_t  RepeatCount;	/* �������������� */
}KEY_T;

/*
	�����ֵ����, ���밴���´���ʱÿ�����İ��¡�����ͳ����¼�

	�Ƽ�ʹ��enum, ����#define��ԭ��
	(1) ����������ֵ,�������˳��ʹ���뿴���������
	(2) �������ɰ����Ǳ����ֵ�ظ���
*/
typedef enum
{
	KEY_NONE = 0,			/* 0 ��ʾ�����¼� */

	KEY_0_DOWN,				/* 0������ */
	KEY_0_UP,				/* 0������ */
	KEY_0_LONG,				/* 0������ */

	KEY_1_DOWN,				/* 1������ */
	KEY_1_UP,				/* 1������ */
	KEY_1_LONG,				/* 1������ */

	KEY_WU_DOWN,				/* WU������ */
	KEY_WU_UP,				/* WU������ */
	KEY_WU_LONG,				/* WU������ */
}KEY_ENUM;

/* ����FIFO�õ����� */
#define KEY_FIFO_SIZE	10
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* ��ֵ������ */
	uint8_t Read;					/* ��������ָ��1 */
	uint8_t Write;					/* ������дָ�� */
	uint8_t Read2;					/* ��������ָ��2 */
}KEY_FIFO_T;


// **************************************************************************
// the globals declaration	


// **************************************************************************
// the function prototypes	
/* ���ⲿ���õĺ������� */
void bsp_InitKey(void);
void bsp_KeyScan(void);
void bsp_PutKey(uint8_t _KeyCode);
uint8_t bsp_GetKey(void);
uint8_t bsp_GetKey2(void);
uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID);
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed);
void bsp_ClearKey(void);	
	
#ifdef __cplusplus
}
#endif // extern "C"

//@} // ingroup
#endif // end of _BSP_KEY_H_ definition	
