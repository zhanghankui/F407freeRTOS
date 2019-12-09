


#ifndef __RWDATA_THREAD_H__
#define __RWDATA_THREAD_H__

#include "FreeRTOS.h"
#include <stdbool.h>
#include "task.h"
#include "queue.h"
#include "globalstruct.h"

/* Exported constants --------------------------------------------------------*/
/* Define the size of the sectors to be used */
#define PAGE_SIZE              ((uint32_t)0x1000)  /* Page size = 4KByte */


/* EEPROM start address in Flash */
#define EEPROM_START_ADDRESS  ((uint32_t)(0x00 *PAGE_SIZE)) // EEPROM emulation start address:

/* Pages 0 and 1 base and end addresses */
#define PAGE0_BASE_ADDRESS    ((uint32_t)(EEPROM_START_ADDRESS + 0x0000))
#define PAGE0_END_ADDRESS     ((uint32_t)(EEPROM_START_ADDRESS + (PAGE_SIZE - 1)))

#define PAGE1_BASE_ADDRESS    ((uint32_t)(EEPROM_START_ADDRESS + PAGE_SIZE))
#define PAGE1_END_ADDRESS     ((uint32_t)(EEPROM_START_ADDRESS + (2 * PAGE_SIZE - 1)))



/* Used Flash pages for EEPROM emulation */
#define PAGE0                 ((uint16_t)0x0000)
#define PAGE1                 ((uint16_t)0x0001)

/* No valid page define */
#define NO_VALID_PAGE         ((uint8_t)0xAB)

/* Page status definitions */
#define ERASED                ((uint8_t)0xFF)     /* Page is empty */
#define RECEIVE_DATA          ((uint8_t)0xEE)     /* Page is marked to receive data */
#define VALID_PAGE            ((uint8_t)0x11)     /* Page containing valid data */
#define WAIT_ERASED           ((uint8_t)0xAA)     /* page wait erased*/


/* Valid pages in read and write defines */
#define READ_FROM_VALID_PAGE  ((uint8_t)0x00)
#define WRITE_IN_VALID_PAGE   ((uint8_t)0x01)

/* Page full define */
#define PAGE_FULL             ((uint8_t)0x80)




/* Variables' number */
#define NB_OF_VAR             ((uint16_t)16)


//----------------整数提取高低位字节
typedef union 
  { 
    short int value;
		signed char buf[2];
  }INT16_union;

typedef union 
  { 
    unsigned short value;
		unsigned char buf[2];
  }UINT16_union;

typedef union 
  { 
    unsigned int value;
		unsigned char buf[4];
  }UINT32_union;

typedef struct _ADDRnDATA_obj_
{
	uint16_t addr;
	uint16_t data;
}ADDRnDATA_obj;
	
	
	

typedef struct _EEPROMupdate_obj_
{
	void * content;
	bool update;
	
}EEPROMupdate_obj;



/* Exported types ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

extern uint8_t powerdump;
extern EEPROMupdate_obj FLAG_EEPROMUPDATE[NB_OF_VAR];
extern xQueueHandle xQ_RWdata_MSG;


void UpdateEEPROM_Flag(uint16_t Addr);
void EE_Init(void);
uint16_t EE_ReadVariable(uint16_t VirtAddress, uint16_t* Data);
uint16_t  EE_ReturnRead(uint16_t VirtAddress);
uint16_t EE_WriteVariable(uint16_t VirtAddress, uint16_t Data);

void RWdata_init(void);




#endif
