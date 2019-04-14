/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "sdio_sdcard.h"
/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */


#define MMC_disk_initialize() SD_Init()
#define MMC_disk_read(buff, sector, count) SD_ReadDisk(buff, sector, count)
#define MMC_disk_write(buff, sector, count) SD_WriteDisk(buff, sector, count)

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
//	int result;

	switch (pdrv) {
//	case DEV_RAM :
//		result = RAM_disk_status();

//		// translate the reslut code here

//		return stat;

	case DEV_MMC :
		stat = 0;
		return stat;

//	case DEV_USB :
//		result = USB_disk_status();

//		// translate the reslut code here

//		return stat;
	default :
		break;		
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	SD_Error result;

	switch (pdrv) {
//	case DEV_RAM :
//		result = RAM_disk_initialize();

//		// translate the reslut code here

//		return stat;

	case DEV_MMC :
		result = MMC_disk_initialize();

		// translate the reslut code here
		if(result == SD_OK)
		{
			stat = RES_OK;
		}
		else
		{
			stat = STA_NODISK;			
		}
		return stat;

//	case DEV_USB :
//		result = USB_disk_initialize();

//		// translate the reslut code here

//		return stat;
	default :
		break;	
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
	SD_Error result;

	switch (pdrv) {
//	case DEV_RAM :
//		// translate the arguments here

//		result = RAM_disk_read(buff, sector, count);

//		// translate the reslut code here

//		return res;

	case DEV_MMC :
		// translate the arguments here

		result = MMC_disk_read(buff, sector, count);

		// translate the reslut code here
		if (result != SD_OK)
		{
			res = RES_ERROR;
			return res;			
		}

		
		#ifdef SD_DMA_MODE		
			/* SDIO工作在DMA模式，需要检查操作DMA传输是否完成 */
			result = SD_WaitReadOperation();
			if (result != SD_OK)
			{
				res = RES_ERROR;
				return res;						
			}
			
			while(SD_GetStatus() != SD_TRANSFER_OK);
		#endif	
		res = RES_OK;			
		return res;

//	case DEV_USB :
//		// translate the arguments here

//		result = USB_disk_read(buff, sector, count);

//		// translate the reslut code here

//		return res;
		default:
			break;			
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
//	case DEV_RAM :
//		// translate the arguments here

//		result = RAM_disk_write(buff, sector, count);

//		// translate the reslut code here

//		return res;

	case DEV_MMC :
		// translate the arguments here

		result = MMC_disk_write((uint8_t *)buff, sector, count);

		// translate the reslut code here
		if (result != SD_OK)
		{
			res = RES_ERROR;
			return res;			
		}

		
		#ifdef SD_DMA_MODE		
			/* SDIO工作在DMA模式，需要检查操作DMA传输是否完成 */
			result = SD_WaitWriteOperation();
			if (result != SD_OK)
			{
				res = RES_ERROR;
				return res;						
			}
			
			while(SD_GetStatus() != SD_TRANSFER_OK);
		#endif	
		res = RES_OK;			
		return res;

//	case DEV_USB :
//		// translate the arguments here

//		result = USB_disk_write(buff, sector, count);

//		// translate the reslut code here

//		return res;
	default:
		break;			
	}

	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
//	int result;

	switch (pdrv) {
//	case DEV_RAM :

//		// Process of the command for the RAM drive

//		return res;

	case DEV_MMC :

		// Process of the command for the MMC/SD card
		switch (cmd)
		{
			/* SD卡磁盘容量： SDCardInfo.CardCapacity */
			case CTRL_SYNC :		/* Wait for end of internal write process of the drive */
				res = RES_OK;
				break;

			case GET_SECTOR_COUNT :	/* Get drive capacity in unit of sector (DWORD) */
				*(DWORD*)buff = SDCardInfo.CardCapacity / 512;
				res = RES_OK;
				break;

			case GET_BLOCK_SIZE :	/* Get erase block size in unit of sector (DWORD) */
				*(WORD*)buff = 512;
				res = RES_OK;
				break;

			case CTRL_ERASE_SECTOR: /* Erase a block of sectors (used when _USE_ERASE == 1) */
			default:
				res = RES_PARERR;
				break;
		}
		return res;

//	case DEV_USB :

//		// Process of the command the USB drive

//		return res;
	default:
		break;		
	}

	return RES_PARERR;
}

/*
*********************************************************************************************************
*	函 数 名: get_fattime
*	功能说明: 获得系统时间，用于改写文件的创建和修改时间。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
DWORD get_fattime (void)
{
	/* 如果有全局时钟，可按下面的格式进行时钟转换. 这个例子是2019-01-01 00:00:00 */

	return	  ((DWORD)(2019 - 1980) << 25)	/* Year = 2013 */
			| ((DWORD)1 << 21)				/* Month = 1 */
			| ((DWORD)1 << 16)				/* Day_m = 1*/
			| ((DWORD)0 << 11)				/* Hour = 0 */
			| ((DWORD)0 << 5)				/* Min = 0 */
			| ((DWORD)0 >> 1);				/* Sec = 0 */
}
