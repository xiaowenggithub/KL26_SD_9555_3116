/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */

#include "fsl_sd_disk.h"	/* Example: MMC/SDC contorl */
#include "malloc.h"

/* Definitions of physical drive number for each drive */
//#define SD_CARD	 0  //SD卡,卷标为0


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	return 0;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case SD:
	
			result = sdcard_disk_initialize(pdrv);
  			break;
	default :
		result = 1;
	}
	if(result)return  STA_NOINIT;
	else return 0; //初始化成功
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
	
	int result;

	switch (pdrv) {
			case SD:
				
	//    result = SD_ReadMultiSectors(sector, count, buff );
				result =  sdcard_disk_read(pdrv, buff, sector, count);
			break;
			default:
			result = 1;
	}

	 if(result==0x00)return RES_OK;	 
    else return RES_ERROR;	
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{
	
	int result;

	switch (pdrv) {
		case SD:
	//		result=SD_WriteMultiSectors(sector,count,buff);
			result = sdcard_disk_write(pdrv, buff, sector, count);
		break;
		default:
			result = 1;
	}

	 if(result == 0x00)return RES_OK;	 
    else return RES_ERROR;	
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case SD:
	 return sdcard_disk_ioctl(pdrv, cmd, buff);

	}

	return RES_PARERR;
}
#endif


DWORD get_fattime (void)
{				 
	//return 0;
	  return	  ((uint32_t)(2013 - 1980) << 25)	/* Fixed to Jan. 1, 2010 */
				| ((uint32_t)1 << 21)
				| ((uint32_t)1 << 16)
				| ((uint32_t)0 << 11)
				| ((uint32_t)0 << 5)
				| ((uint32_t)0 >> 1);
}	

void *ff_memalloc (UINT size)			
{
	//return (void*)mymalloc(SRAMIN,size);


   return malloc(size);

}
//释放内存
void ff_memfree (void* mf)		 
{
	//myfree(SRAMIN,mf);
	free(mf);
}
