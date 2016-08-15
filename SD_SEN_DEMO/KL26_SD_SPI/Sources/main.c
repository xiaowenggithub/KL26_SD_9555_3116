/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_device_registers.h"
#include <stdio.h>
// SDK Included Files
#include "board.h"
#include "main.h"

#include "fsl_spi_master_driver.h"
#include "fsl_spi_dma_master_driver.h"
#include "fsl_clock_manager.h"
#include "fsl_debug_console.h"
#include "diskio.h"
#include "fsl_sd_disk.h"
#include "fsl_sdcard_spi.h"
#include "ff.h"
#include "U1P_9555.h"
#include "U1P_CY3116.h"
//#include "fillefsop.h"

extern sdspi_card_t g_card;
extern sdspi_spi_t g_spi;
extern sdspi_ops_t g_ops;
//extern spi_master_state_t g_spiState;
extern spi_dma_master_state_t spiDmaMasterState;
extern spi_dma_master_user_config_t userDmaConfig;
// extern spi_master_user_config_t userConfig;
uint8_t buff[256];
//uint8_t buff1[4096*2];
uint8_t bufftrans[2048];
FATFS FatFs[1];
#if 0
FIL filesrc;
FIL filedest;

#endif
FIL filedata;

void pedometer_test(void);

int main(void) {
	uint8_t result, ds, fr, res;
	DWORD time1 = 0;
	DWORD time2 = 0;
	DWORD time3 = 0;
	DWORD time4 = 0;
	DWORD returnspibt = 0;
	DWORD i = 0;
	DWORD j = 0;
	uint8_t distance, steps, speed, calories, times = 0;
	unsigned int bw, br;

	i2c_master_state_t master;
	uint8_t user_data[17] = { 0x0c, 0xe0, 0x13, 0x20, 0x00, 0x5a, 0xe0, 0x64,
			0xb4, 0x4e, 0x01, 0x03, 0x05, 0x82, 0x00, 0x00, 0x01 };

	hardware_init();
	// Init OSA layer.
	OSA_Init();
	GPIO_DRV_Init(NULL, SPICSPins);
	GPIO_DRV_Init(NULL, ledPins);

#if 0
	I2C_DRV_MasterInit(BOARD_I2C_INSTANCE, &master);
	OSA_TimeDelay(50);

	CY3116_Gesture_Reco();
#endif

#if 1
	ds = sdcard_disk_initialize(SD);
	printf("this is gesture sd sep\r\n");
	if (ds) {
		printf("\nFailed to initialize SD disk\r\n");
		for (;;) {
		}
	}

	fr = f_mount(&FatFs[0], "0:", 1);
	if (fr) {
		printf("\nError mounting file system %d\r\n", fr);
		for (;;) {
		}
	}

	I2C_DRV_MasterInit(BOARD_I2C_INSTANCE, &master);

	U1P_9555_config(user_data);
	U1P_9555_wakeup(SLEEP_DISABLE);

	//  CY3116_Gesture_Reco();
	uint8_t cmdBuff[1] = { 0xFF };
	uint8_t receiveBuff[1] = { 0x00 };
	uint8_t errortimes = 0;

	i2c_status_t returnValue, returnValue2;

	i2c_device_t CY3116_slave = { .address = SLAVE_CY3116_ADDR, .baudRate_kbps =
			100 };
	printf("Gesture Recognicion TEST \r\n");

	while (1) {
		cmdBuff[0] = 0xaa;
		OSA_TimeDelay(100);
		returnValue = I2C_DRV_MasterReceiveData(
		BOARD_I2C_INSTANCE, &CY3116_slave, cmdBuff, 1, receiveBuff,
				sizeof(receiveBuff));

		if (returnValue != kStatus_I2C_Success) {

			errortimes++;
			if (errortimes < 20) {
				continue;
			} else {
				printf("I2C communication failed, error code: %d\r\n",
						returnValue);
				errortimes = 0;
				break;
			}
		}

		//	printf("receiveBuff[0] = %x\n", receiveBuff[0]);
		if ((receiveBuff[0] == 0x40) | (receiveBuff[0] == 0x41)) // ignore the proximity sensor
				{
			//	printf("First Button Pressed\n");
			//	OSA_TimeDelay(50);
			errortimes = 0;
			_Init: while ((receiveBuff[0] & 0x01) == 1)	// wait for next detection
			{
				OSA_TimeDelay(50);
				returnValue2 = I2C_DRV_MasterReceiveData(
				BOARD_I2C_INSTANCE, &CY3116_slave, cmdBuff, 1, receiveBuff,
						sizeof(receiveBuff));

				if (returnValue2 != kStatus_I2C_Success) {

					if (errortimes < 20) {
						continue;
					} else {
						printf(
								"I2C communication failed up, error code: %d\n",
								returnValue2);
						errortimes = 0;
						break;
					}

				}

				if ((receiveBuff[0] & 0x01) != 1)
					break;	//One tap only

				if ((receiveBuff[0] == 0x21) | (receiveBuff[0] == 0x20)) {

					GPIO_DRV_TogglePinOutput(kGpioLED2);
					OSA_TimeDelay(500);
					GPIO_DRV_TogglePinOutput(kGpioLED2);
					OSA_TimeDelay(500);
#if 1
					gesture = gestureup;
					res = f_open(&filedata, "0:/9555data.txt",
					FA_OPEN_ALWAYS | FA_WRITE);
					printf("write open number is%d\r\n", res);

					memset(buff, 0, sizeof(buff));

					distance = U1P_9555_pedo_status(0x02);
					printf("\rdistance = %d\r\n", distance);
					steps = U1P_9555_pedo_status(0x04);
					printf("\rsteps = %d\r\n", steps);
					speed = U1P_9555_pedo_status(0x06);
					printf("\rcurrent speed = %d mph\r\n", speed);
					calories = U1P_9555_pedo_status(0x08);
					printf("\rcalories = %d\r\n", calories);

					sprintf(buff,
							"distance is %d,steps is %d,speed is %d,calories is %d\r\n",
							distance, steps, speed, calories);
					//	printf("%s\r\n",buff);
					res = f_lseek(&filedata, f_size(&filedata));
					//	printf("f_lseek is %ds\r\n",res);

					fr = f_write(&filedata, buff, strlen(buff), &bw);

					//	printf("%d,%d\r\n",fr,bw);

					f_close(&filedata);
#endif
					break;
				} else
					goto _Init;

			}

			printf("End of the gesture.\r\n");
			OSA_TimeDelay(100);
		} else {
			//	printf("Wait for the gesture\n");
			OSA_TimeDelay(100);
		}

		if ((receiveBuff[0] == 0x20) | (receiveBuff[0] == 0x21))// ignore the proximity sensor
				{
			printf("First Button Pressed\r\n");

			errortimes = 0;
			_Init1: while ((receiveBuff[0] & 0x01) == 1)// wait for next detection
			{
				OSA_TimeDelay(50);
				returnValue2 = I2C_DRV_MasterReceiveData(
				BOARD_I2C_INSTANCE, &CY3116_slave, cmdBuff, 1, receiveBuff,
						sizeof(receiveBuff));

				if (returnValue2 != kStatus_I2C_Success) {
					if (returnValue2 != kStatus_I2C_Success) {

						if (errortimes < 20) {
							continue;
						} else {
							printf(
									"I2C communication failed  down , error code: %d\n",
									returnValue2);
							errortimes = 0;
							break;
						}

					}
				}

				if ((receiveBuff[0] & 0x01) != 1)
					break;	//One tap only

				if ((receiveBuff[0] == 0x41) | (receiveBuff[0] == 0x40)) {

					GPIO_DRV_TogglePinOutput(kGpioLED1);
					OSA_TimeDelay(500);

					GPIO_DRV_TogglePinOutput(kGpioLED1);
					OSA_TimeDelay(500);

					gesture = gesturedown;

#if 1

					res = f_open(&filedata, "0:/9555data.txt",
					FA_OPEN_ALWAYS | FA_READ);


					//		res = f_lseek(&filedata, 0);

					while (1) {
						memset(buff, 0, sizeof(buff));
						OSA_TimeDelay(50);
						fr = f_read(&filedata, buff, sizeof(buff) - 1, &br);
						if (fr) {
							break;
						}

						if (br == (sizeof(buff) - 1)) {
							buff[255] = '\0';
							printf("%s\r\n", buff);

						} else if ((br < (sizeof(buff) - 1) && (br != 0))) {
							buff[br] = '\0';
							printf("%s\r\n", buff);
						} else {
							printf("read is end\r\n");
							break;
						}
					}

					f_close(&filedata);
#endif

					break;
				} else
					goto _Init1;

			}

			printf("End of the gesture.\r\n");
			OSA_TimeDelay(100);
		} else {
			//	printf("Wait for the gesture\n");
			OSA_TimeDelay(100);
		}

	}

#endif

#if 0

	I2C_DRV_MasterInit(BOARD_I2C_INSTANCE, &master);

	U1P_9555_config(user_data);
	U1P_9555_wakeup(SLEEP_DISABLE);
	pedometer_test();

#endif

	PRINTF("\r\nSPI board to board blocking example\r\n");
#if 0
	PRINTF("\r\nThis example run on instance %d", (uint32_t)SPI_MASTER_INSTANCE);
	PRINTF("\r\nBe sure master's SPI%d and slave's SPI%d are connected\r\n",
			(uint32_t)SPI_MASTER_INSTANCE, (uint32_t)SPI_MASTER_INSTANCE);
#endif
#if 0
	result = sdcard_disk_initialize(SD);
	PRINTF("disk init is %d",result);
	for(j=0;j<4096*2;j++)
	{
		buff[j] = 'a';
	}
	time1 = OSA_TimeGetMsec();
	result = SDSPI_DRV_WriteBlocks(&g_spi,&g_card,buff,16768,16);
	time2 = OSA_TimeGetMsec();
	PRINTF("disk write is %d",result);
	//  uint8_t *buff2 = OSA_MemAllocZero(sizeof(buff1));
	//   PRINTF("clearbuff1 is %s\n",buff1);
	time3 = OSA_TimeGetMsec();
	SDSPI_DRV_ReadBlocks(&g_spi,&g_card,buff,16768,16);
	time4 = OSA_TimeGetMsec();

	time1 = time2-time1;
	time3 = time4-time3;

	printf("working read 1KB need %d,write 1KB need %d\n\n",time1,time3);
	PRINTF("%s\n",buff);
#endif

#if 0
	ds = sdcard_disk_initialize(SD);
	if(ds)
	{
		printf("\nFailed to initialize SD disk\r\n");
		for(;;) {}
	}

	/* Select current logical device driver (0 = USB, 1 = SD) */
	//	fr = f_chdrive(SD);
	fr = f_mount(&FatFs[0],"0:",1);
	if(fr)
	{
		printf("\nError mounting file system %d\r\n",fr);
		for(;;) {}
	}

#endif

#if 0
	I2C_DRV_MasterInit(BOARD_I2C_INSTANCE, &master);

	U1P_9555_config(user_data);
	U1P_9555_wakeup(SLEEP_DISABLE);

	res = f_open(&filedata,"0:/9555data.txt",FA_CREATE_ALWAYS|FA_WRITE);
	printf("write open number is%d\r\n",res);

	while(1)
	{
		printf("\r/*********************************************\r\n");
		if(times > 5)
		{
			times = 0;
			break;
		}
		memset(buff,0,sizeof(buff));

		distance = U1P_9555_pedo_status(0x02);
		printf("\rdistance = %d\r\n",distance);
		steps = U1P_9555_pedo_status(0x04);
		printf("\rsteps = %d\r\n",steps);
		speed = U1P_9555_pedo_status(0x06);
		printf("\rcurrent speed = %d mph\r\n",speed);
		calories = U1P_9555_pedo_status(0x08);
		printf("\rcalories = %d\r\n",calories);

		sprintf(buff,"distance is %d,steps is %d,speed is %d,calories is %d\r\n",distance,steps,speed,calories);
		printf("%s\r\n",buff);

		fr = f_write(&filedata, buff, strlen(buff), &bw);

		printf("%d,%d\r\n",fr,bw);

		if (fr)
		{
			printf("write is error\r\n");
			break;
		}
		/* error or disk full */

		printf("\r/*********************************************\r\n");

		OSA_TimeDelay(500);
		times++;

		//	U1P_9555_read_config();
	}

	f_close(&filedata);
#endif

#if 0
	result = f_close(&file);
	printf("close is %d",result);

	res=f_open(&file,"0:/yuhao.c",FA_WRITE);	 //写

	printf("write open hax.c number is%d\r\n",res);

	res=f_write(&file,buff,sizeof(buff),&bw);
	//u8 mf_write(buff,sizeof(buff));
	f_close(&file);

#endif
#if 0
	result=f_open(&filesrc,"0:/test.txt",FA_OPEN_EXISTING|FA_READ);	 //读
	printf("open is %d",result);

	result = f_open(&filedest,"0:/mp10.c",FA_CREATE_ALWAYS|FA_WRITE);

	printf("open is %d",result);

	//    	fr = f_read(&filesrc, bufftrans, sizeof bufftrans, &br);
	//    	 fr = f_write(&filedest, bufftrans, br, &bw);

	//	time1 = OSA_TimeGetMsec();
	returnspibt = getSpiMaxFrequency(&g_spi);
	printf("working %u\n\n",returnspibt);
	for (;;) {

		i++;
		time1 = OSA_TimeGetMsec();
		fr = f_read(&filesrc, bufftrans, sizeof bufftrans, &br); /* Read a chunk of source file */
		time2 = OSA_TimeGetMsec();
		if (fr || br == 0) break; /* error or eof */
		fr = f_write(&filedest, bufftrans, br, &bw);
		time3 = OSA_TimeGetMsec();/* Write it to the destination file */
		if (fr || bw < br) break; /* error or disk full */
		//    time1 = OSA_TimeGetMsec();
		time1 = time2-time1;
		time2 = time3-time2;
		printf("working  times %d read 8KB need %d,write 8KB need %d\n\n",i,time1,time2);
	}
	i = 0;

	//	 time2 = OSA_TimeGetMsec();
	//	 time1= time2-time1;
	printf("working %ul\n\n",time1);
	/* Close open files */
	f_close(&filesrc);
	f_close(&filedest);

	printf("isisi over\n\n");
#endif

#if 0
	//	mf_gets(0x20);
	res=f_read(&filesrc,buffread,200,&br);

	printf("the read  is%s\r\n",buffread);

	//mf_read(0x0A);

	//  	f_close(&file);

#endif

	OSA_TimeDelay(2U);

}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

void pedometer_test(void) {
	uint8_t distance, steps, speed, calories, fr, bw, times = 0;
	while (1) {
		printf("\r/*********************************************\r\n");
		if (times > 50) {
			times = 0;
			break;
		}
		memset(buff, 0, sizeof(buff));

		distance = U1P_9555_pedo_status(0x02);
		printf("\rdistance = %d\r\n", distance);
		steps = U1P_9555_pedo_status(0x04);
		printf("\rsteps = %d\r\n", steps);
		speed = U1P_9555_pedo_status(0x06);
		printf("\rcurrent speed = %d mph\r\n", speed);
		calories = U1P_9555_pedo_status(0x08);
		printf("\rcalories = %d\r\n", calories);
#if 1
		sprintf(buff,
				"distance is %d,steps is %d,speed is %d,calories is %d\r\n",
				distance, steps, speed, calories);
		printf("%s\r\n", buff);

		//    	fr = f_write(&filedata, buff, sizeof(buff), &bw);

		//    	printf("%d,%d\r\n",fr,bw);

		//    	if (fr)
		//    	{
		//    		printf("write is error\r\n");
		//    		break;
		//   	}
		/* error or disk full */

#endif

		printf("\r/*********************************************\r\n");

		OSA_TimeDelay(500);
		times++;

		//	U1P_9555_read_config();
	}
}
