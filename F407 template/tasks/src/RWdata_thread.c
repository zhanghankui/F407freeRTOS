
#include "RWdata_thread.h"
#include "w25qxx.h"
#include "stdio.h"

//w25qxx˫����ϵͳ

uint8_t powerdump = 0;
EEPROMupdate_obj FLAG_EEPROMUPDATE[NB_OF_VAR];

static uint16_t DataVar = 0;
static uint32_t CurReAddress;
static uint8_t transfernow = 0;
/* Virtual address defined by the user: 0xFFFF value is prohibited */

//extern uint8_t VirtAddVarTab;
//uint8_t VirtAddVarTab = 0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static uint16_t InitCurrReAddress(void);
static void __EE_Init(void);
static void EE_Format(void);
static uint16_t EE_VerifyPageFullWriteVariable(uint16_t VirtAddress, uint16_t Data);
static uint16_t EE_PageTransfer(uint16_t VirtAddress, uint16_t Data);
static uint16_t EE_FindValidPage(uint8_t Operation);

uint32_t ReadOneWord(uint32_t addr)
{
  UINT32_union data;
  W25QXX_Read(data.buf,addr,4);
  return data.value;
}

uint16_t ReadHalfWord(uint32_t addr)
{
  UINT16_union data;
  W25QXX_Read(data.buf,addr,2);
  return data.value;
}

void WriteByte(uint32_t addr,uint8_t data)
{
  W25QXX_Write_NoCheck(&data,addr,1);
}
void WriteHalfWord(uint32_t addr,uint16_t data)
{
  UINT16_union udata;
  udata.value =  data;  
  W25QXX_Write_NoCheck(udata.buf,addr,2);
}

void WriteOneWord(uint32_t addr,uint32_t data)
{
  UINT32_union udata;
  udata.value = data; 
  W25QXX_Write_NoCheck(udata.buf,addr,4);
}




void UpdateEEPROM_Flag(uint16_t Addr)
{
	if(Addr<NB_OF_VAR)
	{
		FLAG_EEPROMUPDATE[Addr].update = true;
	}
}



//initial read address, for reduce query time in every W/R
uint16_t InitCurrReAddress(void)
{
	FLASH_Status FlashStatus = FLASH_COMPLETE;
	uint16_t ValidPage = PAGE0;
	//uint32_t Address;
	uint32_t PageEndAddress;

	/* Get valid Page for write operation */
	ValidPage = EE_FindValidPage(WRITE_IN_VALID_PAGE);

	/* Get the valid Page start Address */
	//Address = (uint32_t)(EEPROM_START_ADDRESS + (uint32_t)(ValidPage * PAGE_SIZE));
	CurReAddress = (uint32_t)(EEPROM_START_ADDRESS + (uint32_t)(ValidPage * PAGE_SIZE)+4);//������ʼ4���ֽ�
	
	/* Check if there is no valid page */
	if (ValidPage == NO_VALID_PAGE)
	{
		return  NO_VALID_PAGE;
	}
  
	/* Get the valid Page end Address */
	PageEndAddress = (uint32_t)((EEPROM_START_ADDRESS-2) + (uint32_t)((1 + ValidPage) * PAGE_SIZE));

	/* Check each active page address starting from begining */
	while (CurReAddress < PageEndAddress)
	{
		/* Verify if Address and Address+2 contents are 0xFFFFFFFF */
		if (ReadOneWord(CurReAddress) == 0xFFFFFFFF)
		{
		//д��ַ�ȶ���ַ��4
			CurReAddress = CurReAddress - 4;      
			/* Set variable virtual address */
			FlashStatus = FLASH_COMPLETE;
			/* Return program operation status */
			return FlashStatus;
		}
		else
		{
			/* Next address location */
			CurReAddress = CurReAddress + 4;
		}
	}
	//����Ѱ����β
	CurReAddress = CurReAddress - 4;	
  /* Return PAGE_FULL in case the valid page is full */
  return PAGE_FULL;
}

/**
  * @brief  Restore the pages to a known good state in case of page's status
  *   corruption after a power loss.
  * @param  None.
  * @retval - Flash error code: on write Flash error
  *         - FLASH_COMPLETE: on success
  */
void __EE_Init(void)
{
  uint32_t readdata;
	uint8_t PageStatus0 = 6, PageStatus1 = 6;
	uint16_t VarIdx = 0;
	uint16_t ReadStatus = 0;
	int16_t x = -1;

		/* Get Page0 status */
  readdata = ReadOneWord(PAGE0_BASE_ADDRESS);
  if(readdata == 0xFFFFFFFF)
  {
    PageStatus0 = ERASED;
  }
  else if((readdata&0xFF000000) == 0xAA000000)
  {
    PageStatus0 = WAIT_ERASED;    
  }  
  else if((readdata&0x00FF0000) == 0x00110000)
  {
    PageStatus0 = VALID_PAGE;    
  }
  else if((readdata&0x0000FF00) == 0x0000EE00)
  {
    PageStatus0 = RECEIVE_DATA;
  }

  readdata = ReadOneWord(PAGE1_BASE_ADDRESS);
  if(readdata == 0xFFFFFFFF)
  {
    PageStatus1 = ERASED;
  }
  else if((readdata&0xFF000000) == 0xAA000000)
  {
    PageStatus1 = WAIT_ERASED;    
  }  
  else if((readdata&0x00FF0000) == 0x00110000)
  {
    PageStatus1 = VALID_PAGE;    
  }
  else if((readdata&0x0000FF00) == 0x0000EE00)
  {
    PageStatus1 = RECEIVE_DATA;
  }


  
	
//	/* Get Page0 status */
//	PageStatus0 = (*(__IO uint16_t*)PAGE0_BASE_ADDRESS);
//	/* Get Page1 status */
//	PageStatus1 = (*(__IO uint16_t*)PAGE1_BASE_ADDRESS);

  /* Check for invalid header states and repair if necessary */
  switch (PageStatus0)
  {
    case ERASED:
      if (PageStatus1 == VALID_PAGE) /* Page0 erased, Page1 valid */
      {//���ٲ���PAGE0
//        /* Erase Page0 */
//        FlashStatus = FLASH_ErasePage(PAGE0_BASE_ADDRESS);// power on erase once, in case erase fail in latest power off 
//        /* If erase operation was failed, a Flash error code is returned */
//        if (FlashStatus != FLASH_COMPLETE)
//        {
//          return FlashStatus;
//        }
      }
      else if (PageStatus1 == RECEIVE_DATA) /* Page0 erased, Page1 receive */
      {//���ٲ���PAGE0
//        /* Erase Page0 */
//        FlashStatus = FLASH_ErasePage(PAGE0_BASE_ADDRESS);
//        /* If erase operation was failed, a Flash error code is returned */
//        if (FlashStatus != FLASH_COMPLETE)
//        {
//          return FlashStatus;
//        }
        /* Mark Page1 as valid */
        WriteByte(PAGE1_BASE_ADDRESS+2, VALID_PAGE);
      }
      //Page0 erased, Page1 wait erased invalid state -> format EEPROM 
//      else if(PageStatus1 == WAIT_ERASED) /* Page0 erased, Page1 wait erased */
//      {
//      	
//      }
      else /* First EEPROM access (Page0&1 are erased) or invalid state -> format EEPROM */
      {
        /* Erase both Page0 and Page1 and set Page0 as valid page */
        EE_Format();
      }
      break;

    case RECEIVE_DATA:
      if (PageStatus1 == VALID_PAGE) /* Page0 receive, Page1 valid */
      {
        /* Transfer data from Page1 to Page0 */
				transfernow = 1;
        for (VarIdx = 0; VarIdx < NB_OF_VAR; VarIdx++)
        {
          if (( ReadHalfWord(PAGE0_BASE_ADDRESS + 6)) == VarIdx)//����ַ���Ź��Ѵ洢�ĵ�һ������
          {
            x = VarIdx;
          }
          if (VarIdx != x)
          {
            /* Read the last variables' updates */
            ReadStatus = EE_ReadVariable(VarIdx, &DataVar);
            /* In case variable corresponding to the virtual address was found */
            if (ReadStatus != 0x1)//�����ҵ�
            {
              /* Transfer the variable to the Page0 */
              EE_VerifyPageFullWriteVariable(VarIdx, DataVar);
            }
          }
        }
				transfernow = 0;
        /* Mark Page0 as valid */
        WriteByte(PAGE0_BASE_ADDRESS+2, VALID_PAGE);
        /* Erase Page1 */
        W25QXX_Erase_Sector(PAGE1);
        
      }
      else if (PageStatus1 == ERASED) /* Page0 receive, Page1 erased */
      {//���ٲ���PAGE1
//        /* Erase Page1 */
//        FlashStatus = FLASH_ErasePage(PAGE1_BASE_ADDRESS);
//        /* If erase operation was failed, a Flash error code is returned */
//        if (FlashStatus != FLASH_COMPLETE)
//        {
//          return FlashStatus;
//        }
        /* Mark Page0 as valid */
        WriteByte(PAGE0_BASE_ADDRESS+2, VALID_PAGE);
      }
      else if(PageStatus1 == WAIT_ERASED) /* Page0 receive, Page1 wait erased */
      {
        /* Erase Page1 */
        W25QXX_Erase_Sector(PAGE1);
        /* Mark Page0 as valid */
        WriteByte(PAGE0_BASE_ADDRESS+2, VALID_PAGE);   	
      }      
      else /* Invalid state -> format eeprom */
      {
        /* Erase both Page0 and Page1 and set Page0 as valid page */
        EE_Format();
      }
      break;

    case VALID_PAGE:
      if (PageStatus1 == VALID_PAGE) /* Invalid state -> format eeprom */
      {
        /* Erase both Page0 and Page1 and set Page0 as valid page */
        EE_Format();
      }
      else if (PageStatus1 == ERASED) /* Page0 valid, Page1 erased */
      {//���ٲ���PAGE1
//        /* Erase Page1 */
//        FlashStatus = FLASH_ErasePage(PAGE1_BASE_ADDRESS);
//        /* If erase operation was failed, a Flash error code is returned */
//        if (FlashStatus != FLASH_COMPLETE)
//        {
//          return FlashStatus;
//        }
      }
      else if(PageStatus1 == WAIT_ERASED) /* Page0 valid, Page1 wait erased */
      {
        /* Erase Page1 */
         W25QXX_Erase_Sector(PAGE1);	
      }        
      else if(PageStatus1 == RECEIVE_DATA) /* Page0 valid, Page1 receive */
      {
        /* Transfer data from Page0 to Page1 */
				transfernow = 1;
        for (VarIdx = 0; VarIdx < NB_OF_VAR; VarIdx++)
        {
          if (ReadHalfWord(PAGE1_BASE_ADDRESS + 6) == VarIdx)
          {
            x = VarIdx;
          }
          if (VarIdx != x)
          {
            /* Read the last variables' updates */
            ReadStatus = EE_ReadVariable(VarIdx, &DataVar);
            /* In case variable corresponding to the virtual address was found */
            if (ReadStatus != 0x1)
            {
              /* Transfer the variable to the Page1 */
              EE_VerifyPageFullWriteVariable(VarIdx, DataVar);
            }
          }
        }
				transfernow = 0;
        /* Mark Page1 as valid */
        WriteByte(PAGE1_BASE_ADDRESS+2, VALID_PAGE);
        /* Erase Page0 */
        W25QXX_Erase_Sector(PAGE0);
      }
			else /* Page0 wait erased, Page1 erased */
			{
				/* Erase both Page0 and Page1 and set Page0 as valid page */
				EE_Format();
			}			
      break;

      case WAIT_ERASED:
      {
        if (PageStatus1 == VALID_PAGE) /* Page0 wait erased, Page1 valid */
        {
          /* Erase Page0 */
          W25QXX_Erase_Sector(PAGE0); 
        }
        else if (PageStatus1 == RECEIVE_DATA) /* Page0 wait erased, Page1 receive */
        {
          /* Erase Page0 */
          W25QXX_Erase_Sector(PAGE0);
          /* Mark Page1 as valid */
          WriteByte(PAGE1_BASE_ADDRESS+2, VALID_PAGE);  
        }
        //Page0 wait erased, Page1 wait erased invalid state -> format EEPROM       
        //		else if(PageStatus1 == WAIT_ERASED) /* Page0 wait erased, Page1 wait erased */
        //		{
        //		
        //		}        
        else /* Page0 wait erased, Page1 erased */
        {
          /* Erase both Page0 and Page1 and set Page0 as valid page */
          EE_Format();
        }
      }
      break;

			default:  /* Any other state -> format eeprom */
      /* Erase both Page0 and Page1 and set Page0 as valid page */
      EE_Format();
      break;
  }

}

void EE_Init(void)
{
	__EE_Init();
	
	InitCurrReAddress();
}

/**
  * @brief  Returns the last stored variable data, if found, which correspond to
  *   the passed virtual address
  * @param  VirtAddress: Variable virtual address
  * @param  Data: Global variable contains the read variable value
  * @retval Success or error status:
  *           - 0: if variable was found
  *           - 1: if the variable was not found
  *           - NO_VALID_PAGE: if no valid page was found.
  */
uint16_t EE_ReadVariable(uint16_t VirtAddress, uint16_t* Data)
{
	uint16_t ValidPage = PAGE0;
	uint16_t AddressValue = 0x5555; 
	uint16_t ReadStatus = 1;
	uint32_t Address = 0x08010000, PageStartAddress = 0x08010000;

	/* Get active Page for read operation */
	ValidPage = EE_FindValidPage(READ_FROM_VALID_PAGE);

	/* Check if there is no valid page */
	if (ValidPage == NO_VALID_PAGE)
	{
		return  NO_VALID_PAGE;
	}

	/* Get the valid Page start Address */
	PageStartAddress = (uint32_t)(EEPROM_START_ADDRESS +2 + (uint32_t)(ValidPage * PAGE_SIZE));

  /* Get the valid Page end Address */
//  Address = (uint32_t)((EEPROM_START_ADDRESS - 1) + (uint32_t)((1 + ValidPage) * PAGE_SIZE));
	if(transfernow == 0)
	{
		Address = CurReAddress + 2;//ǰ��2���ֽ������ݣ����������ֽ��ǵ�ַ
	}
	else
	{
		Address = EEPROM_START_ADDRESS + (uint32_t)((ValidPage+1) * PAGE_SIZE) - 2;//ֱ�Ӵ���ƨ����
	}

		/* Check each active page address starting from end */
	while (Address > PageStartAddress )//�޸�
	{
		/* Get the current location content to be compared with virtual address */
		AddressValue = ReadHalfWord(Address);

		/* Compare the read address with the virtual address */
		if (AddressValue == VirtAddress)
		{
			/* Get content of Address-2 which is variable value */
			*Data = ReadHalfWord(Address - 2);

			/* In case variable value is read, reset ReadStatus flag */
			ReadStatus = 0;

			break;
		}
		else
		{
			/* Next address location */
			Address = Address - 4;
		}
	}

	/* Return ReadStatus value: (0: variable exist, 1: variable doesn't exist) */
	return ReadStatus;
}

/**
  * @brief  Returns the last stored variable data, if found, which correspond to
  *   the passed virtual address
  * @param  VirtAddress: Variable virtual address
  * @retval Data:contains the read variable value
  */

uint16_t EE_ReturnRead(uint16_t VirtAddress)
{
	uint16_t data = 0xFFFF;
	EE_ReadVariable(VirtAddress,&data);
	return data;	
}

/**
  * @brief  Writes/upadtes variable data in EEPROM.
  * @param  VirtAddress: Variable virtual address
  * @param  Data: 16 bit data to be written
  * @retval Success or error status:
  *           - FLASH_COMPLETE: on success
  *           - PAGE_FULL: if valid page is full
  *           - NO_VALID_PAGE: if no valid page was found
  *           - Flash error code: on write Flash error
  */
uint16_t EE_WriteVariable(uint16_t VirtAddress, uint16_t Data)
{
	uint16_t Status = 0;
	//  FLASH_Unlock();

	/* Write the variable virtual address and value in the EEPROM */
	Status = EE_VerifyPageFullWriteVariable(VirtAddress, Data);

	/* In case the EEPROM active page is full */
	if (Status == PAGE_FULL)//PAGE_FULL = 0x80
	{
		/* Perform Page transfer */
		Status = EE_PageTransfer(VirtAddress, Data);
	}

	//	FLASH_Lock();
	/* Return last operation status */
	return Status;
}

/**
  * @brief  Erases PAGE0 and PAGE1 and writes VALID_PAGE header to PAGE0
  * @param  None
  * @retval Status of the last operation (Flash write or erase) done during
  *         EEPROM formating
  */
static void EE_Format(void)
{

	/* Erase Page0 */
	W25QXX_Erase_Sector(PAGE0);

	/* Set Page0 as valid page: Write VALID_PAGE at Page0 base address */
	WriteByte(PAGE0_BASE_ADDRESS+2, VALID_PAGE);

	/* Erase Page1 */
	W25QXX_Erase_Sector(PAGE1);
}

/**
  * @brief  Find valid Page for write or read operation
  * @param  Operation: operation to achieve on the valid page.
  *   This parameter can be one of the following values:
  *     @arg READ_FROM_VALID_PAGE: read operation from valid page
  *     @arg WRITE_IN_VALID_PAGE: write operation from valid page
  * @retval Valid page number (PAGE0 or PAGE1) or NO_VALID_PAGE in case
  *   of no valid page was found
  */
static uint16_t EE_FindValidPage(uint8_t Operation)
{
  uint32_t readdata;
	uint8_t PageStatus0 = 6, PageStatus1 = 6;


      /* Get Page0 status */
  readdata = ReadOneWord(PAGE0_BASE_ADDRESS);
  if(readdata == 0xFFFFFFFF)
  {
    PageStatus0 = ERASED;
  }
  else if((readdata&0xFF000000) == 0xAA000000)
  {
    PageStatus0 = WAIT_ERASED;    
  }  
  else if((readdata&0x00FF0000) == 0x00110000)
  {
    PageStatus0 = VALID_PAGE;    
  }
  else if((readdata&0x0000FF00) == 0x0000EE00)
  {
    PageStatus0 = RECEIVE_DATA;
  }

  readdata = ReadOneWord(PAGE1_BASE_ADDRESS);
  if(readdata == 0xFFFFFFFF)
  {
    PageStatus1 = ERASED;
  }
  else if((readdata&0xFF000000) == 0xAA000000)
  {
    PageStatus1 = WAIT_ERASED;    
  }  
  else if((readdata&0x00FF0000) == 0x00110000)
  {
    PageStatus1 = VALID_PAGE;    
  }
  else if((readdata&0x0000FF00) == 0x0000EE00)
  {
    PageStatus1 = RECEIVE_DATA;
  }





  /* Write or read operation */
  switch (Operation)
  {
    case WRITE_IN_VALID_PAGE:   /* ---- Write operation ---- */
      if (PageStatus1 == VALID_PAGE)
      {
        /* Page0 receiving data */
        if (PageStatus0 == RECEIVE_DATA)
        {
          return PAGE0;         /* Page0 valid */
        }
        else
        {
          return PAGE1;         /* Page1 valid */
        }
      }
      else if (PageStatus0 == VALID_PAGE)
      {
        /* Page1 receiving data */
        if (PageStatus1 == RECEIVE_DATA)
        {
          return PAGE1;         /* Page1 valid */
        }
        else
        {
          return PAGE0;         /* Page0 valid */
        }
      }
      else
      {
        return NO_VALID_PAGE;   /* No valid Page */
      }

    case READ_FROM_VALID_PAGE:  /* ---- Read operation ---- */
      if (PageStatus0 == VALID_PAGE)
      {
        return PAGE0;           /* Page0 valid */
      }
      else if (PageStatus1 == VALID_PAGE)
      {
        return PAGE1;           /* Page1 valid */
      }
      else
      {
        return NO_VALID_PAGE ;  /* No valid Page */
      }

    default:
      return PAGE0;             /* Page0 valid */
  }
}

/**
  * @brief  Verify if active page is full and Writes variable in EEPROM.
  * @param  VirtAddress: 16 bit virtual address of the variable
  * @param  Data: 16 bit data to be written as variable value
  * @retval Success or error status:
  *           - FLASH_COMPLETE: on success
  *           - PAGE_FULL: if valid page is full
  *           - NO_VALID_PAGE: if no valid page was found
  *           - Flash error code: on write Flash error
  */
static uint16_t EE_VerifyPageFullWriteVariable(uint16_t VirtAddress, uint16_t Data)
{
	FLASH_Status FlashStatus = FLASH_COMPLETE;
	uint16_t ValidPage = PAGE0;
//  uint32_t Address = 0x08010000;
	uint32_t PageEndAddress;

	/* Get valid Page for write operation */
	ValidPage = EE_FindValidPage(WRITE_IN_VALID_PAGE);

	/* Check if there is no valid page */
	if (ValidPage == NO_VALID_PAGE)
	{
		return  NO_VALID_PAGE;
	}

  /* Get the valid Page start Address */
//  Address = (uint32_t)(EEPROM_START_ADDRESS + (uint32_t)(ValidPage * PAGE_SIZE));

  /* Get the valid Page end Address */
  PageEndAddress = (uint32_t)((EEPROM_START_ADDRESS) + (uint32_t)((1 + ValidPage) * PAGE_SIZE));//modified
	CurReAddress = CurReAddress+4;//�޸�Ϊд�ĵ�ַ
  /* Check each active page address starting from begining */
  while (CurReAddress < PageEndAddress)
  {
    /* Verify if Address and Address+1 contents are 0xFFFF */
    if (ReadOneWord(CurReAddress) == 0xFFFFFFFF)
    {
      /* Set variable data and virtual address*/
      WriteHalfWord(CurReAddress, Data);
      /* Set variable virtual address */
      WriteHalfWord(CurReAddress + 2, VirtAddress);
      /* Return program operation status */
      return FlashStatus;
    }
    else
    {//modified algiruthm will never excute to there
      /* Next address location */
      CurReAddress = CurReAddress + 4;
    }
  }

  /* Return PAGE_FULL in case the valid page is full */
  return PAGE_FULL;
}

/**
  * @brief  Transfers last updated variables data from the full Page to
  *   an empty one.
  * @param  VirtAddress: 16 bit virtual address of the variable
  * @param  Data: 16 bit data to be written as variable value
  * @retval Success or error status:
  *           - FLASH_COMPLETE: on success
  *           - PAGE_FULL: if valid page is full
  *           - NO_VALID_PAGE: if no valid page was found
  *           - Flash error code: on write Flash error
  */
static uint16_t EE_PageTransfer(uint16_t VirtAddress, uint16_t Data)
{
	FLASH_Status FlashStatus = FLASH_COMPLETE;
	uint32_t NewPageAddress = 0x080103FF, OldPageAddress = 0x08010000;
	uint16_t ValidPage = PAGE0, VarIdx = 0;
	uint16_t EepromStatus = 0, ReadStatus = 0;

	/* Get active Page for read operation */
	ValidPage = EE_FindValidPage(READ_FROM_VALID_PAGE);

	if (ValidPage == PAGE1)       /* Page1 valid */
	{
		/* New page address where variable will be moved to */
		NewPageAddress = PAGE0_BASE_ADDRESS;

		/* Old page address where variable will be taken from */
		OldPageAddress = PAGE1_BASE_ADDRESS;
	}
	else if (ValidPage == PAGE0)  /* Page0 valid */
	{
		/* New page address where variable will be moved to */
		NewPageAddress = PAGE1_BASE_ADDRESS;

		/* Old page address where variable will be taken from */
		OldPageAddress = PAGE0_BASE_ADDRESS;
	}
	else
	{
		return NO_VALID_PAGE;       /* No valid Page */
	}

	/* Set the new Page status to RECEIVE_DATA status */
	WriteByte(NewPageAddress+1, RECEIVE_DATA);
	
	InitCurrReAddress();//refresh initial write address
	
	/* Write the variable passed as parameter in the new active page */
	EepromStatus = EE_VerifyPageFullWriteVariable(VirtAddress, Data);
	/* If program operation was failed, a Flash error code is returned */
	if (EepromStatus != FLASH_COMPLETE)
	{
		return EepromStatus;
	}
	transfernow = 1;
	/* Transfer process: transfer variables from old to the new active page */
	for (VarIdx = 0; VarIdx < NB_OF_VAR; VarIdx++)
	{
		if (VarIdx != VirtAddress)  /* Check each variable except the one passed as parameter */
		{
			/* Read the other last variable updates */
			ReadStatus = EE_ReadVariable(VarIdx, &DataVar);
			/* In case variable corresponding to the virtual address was found */
			if (ReadStatus != 0x1)
			{
				/* Transfer the variable to the new active page */
				EE_VerifyPageFullWriteVariable(VarIdx, DataVar);
			}
		}
	}
	transfernow = 0;

	//if in power off save data status, will not excute erasepage
	if(powerdump)
	{
		WriteByte((OldPageAddress+3), WAIT_ERASED);  	
	}
	else
	{
		/* Erase the old Page: Set old Page status to ERASED status */
		W25QXX_Erase_Sector(ValidPage);
	}


	/* Set new Page status to VALID_PAGE status */
	WriteByte(NewPageAddress+2, VALID_PAGE);

	/* Return last operation flash status */
	return FlashStatus;
}

//д�����̣߳������ݿ���ֱ�Ӷ���д������Ϊ���ܻ��ѽϳ�ʱ�䣬������Ҫ���߳�

#define RWdata_THREAD_STACK        512

xQueueHandle xQ_RWdata_MSG = NULL;                //���Ҫд�������
xTaskHandle  xH_RWdata = NULL;

//EE_WriteVariable(uint16_t VirtAddress, uint16_t Data) 32λ

static void RWdata_thread(void *arg)
{
	ADDRnDATA_obj addrndata;
	xQ_RWdata_MSG = xQueueCreate(20,sizeof(ADDRnDATA_obj));          //����һ��������20�����ݵĶ���	
	
	if(NULL == xQ_RWdata_MSG)
	{
		//��������ʧ��
		while(1)
		{
			printf("Creat the RWdata Queue failed! \r\n");
			vTaskDelay(10000);//1s
		}		
	}	
	while(1)
	{
		//�ɹ����յ�CAN��������
		if(xQueueReceive( xQ_RWdata_MSG, &(addrndata), (portTickType)1000))//�����Ϣ����Ϊ���ҵ���������Ϊ0����ô�˺������������ء�
		{
			EE_WriteVariable(addrndata.addr,addrndata.data);//д������
		}		
	}		
}

void RWdata_init(void)
{
	xTaskCreate(RWdata_thread, "RWdata", RWdata_THREAD_STACK, NULL,RWdata_THREAD_PRIO, &xH_RWdata);
	if(NULL == xH_RWdata)
	{
		printf("RWdata thread created failed!\r\n");
	}
}






/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
