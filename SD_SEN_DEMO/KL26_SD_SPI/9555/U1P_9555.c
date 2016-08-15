
#include "U1P_9555.h"
#include "fsl_i2c_master_driver.h"

/*-----------Mailbox Struct-----------*/

const i2c_device_t slave =
{
	.address = 0x4c,
	.baudRate_kbps = 100
};

static void pause(void)
{
    int n;
    for(n=0; n<40; n++)
		__asm("nop");


    //	NOP;
}

 uint8_t U1P_9555_version(void)
 {
	 uint8_t finresult;
	 uint16_t result;
	 i2c_status_t check;
	 uint32_t instance = BOARD_I2C_INSTANCE;
	 I2C_Type * base = g_i2cBase[instance];

	 I2C_DRV_MasterSetBaudRate(instance, &slave);
	 /* Set direction to send for sending of address and data. */
	 I2C_HAL_SetDirMode(base, kI2CSend);

	 /* Enable i2c interrupt.*/
//	 I2C_HAL_ClearInt(base);
//	 I2C_HAL_SetIntCmd(base, true);

     /* Generate start signal. */
	 I2C_HAL_SendStart(base);				//start

	 /* Send first byte in address buffer to trigger interrupt.*/
	 I2C_HAL_WriteByte(base, 0x98);						//iic write

	 /* Wait for the transfer to finish.*/
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendAck(base);

	 I2C_HAL_WriteByte(base, 0x00);						//offset
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendAck(base);

	 I2C_HAL_WriteByte(base, 0x15);						//pedometer application id
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendAck(base);

	 I2C_HAL_WriteByte(base, 0x30);						//
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendAck(base);

	 I2C_HAL_WriteByte(base, 0x01);
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendAck(base);

	 I2C_HAL_WriteByte(base, 0x01);
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendNak(base);

	 check = I2C_HAL_SendStop(base);

	 pause();

	 I2C_HAL_SendStart(base);

	 I2C_HAL_WriteByte(base, 0x98);
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendAck(base);

	 I2C_HAL_WriteByte(base, 0x00);
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendAck(base);


	 I2C_HAL_SendStart(base);

	 I2C_HAL_WriteByte(base, 0x99);
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendAck(base);

	 I2C_HAL_SetDirMode(base, kI2CReceive);


	result = I2C_HAL_ReadByte(base);				//0x15----99
	I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	I2C_HAL_SendAck(base);

	result = I2C_HAL_ReadByte(base);				//0x80----15
	I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	I2C_HAL_SendAck(base);

	result = I2C_HAL_ReadByte(base);				//0x01----80
	I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	I2C_HAL_SendAck(base);

	result = I2C_HAL_ReadByte(base);				//0x01----01
	I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	I2C_HAL_SendAck(base);

	result = I2C_HAL_ReadByte(base);				//0x02version----01
	I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	I2C_HAL_SendAck(base);

	finresult = I2C_HAL_ReadByte(base);				//0x00----version
	I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	I2C_HAL_SendNak(base);

	result = I2C_HAL_ReadByte(base);				//this is very important
	printf("result = %d\n",result);

	check = I2C_HAL_SendStop(base);

	 pause();

	if(check)
	    return check;
	else
		return finresult;
 }

 uint8_t U1P_9555_set_user_data(uint8_t data_type, uint8_t data)
 {
	 uint8_t cnt;
	 i2c_status_t check;
	 uint32_t instance = BOARD_I2C_INSTANCE;
	 uint8_t set_steplen[6] = {0x98,0x00,0x15,0x20,data_type,0x01};
	 I2C_Type * base = g_i2cBase[instance];
	 I2C_DRV_MasterSetBaudRate(instance, &slave);
	 I2C_HAL_SetDirMode(base, kI2CSend);

	 I2C_HAL_SendStart(base);

	 for(cnt=0; cnt<6; cnt++)
	 {
		 I2C_HAL_WriteByte(base, set_steplen[cnt]);
		 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
		 I2C_HAL_SendAck(base);

	 }

	 I2C_HAL_WriteByte(base, data);
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendNak(base);

	 check = I2C_HAL_SendStop(base);

	 pause();

	 return check;
 }


 uint8_t U1P_9555_read_height(void)
  {
 	 uint8_t finresult,result;
 	 i2c_status_t check;
 	uint32_t instance = BOARD_I2C_INSTANCE;
 	 I2C_Type * base = g_i2cBase[instance];

 	 I2C_DRV_MasterSetBaudRate(instance, &slave);
 	 /* Set direction to send for sending of address and data. */
 	 I2C_HAL_SetDirMode(base, kI2CSend);

      /* Generate start signal. */
 	 I2C_HAL_SendStart(base);				//start

 	 uint8_t command[5] = {0x98,0x00,0x15,0x10,0x08};
	 uint8_t cnt;
	 for(cnt=0; cnt<0x05; cnt++)
	 {
		 I2C_HAL_WriteByte(base, command[cnt]);						//iic write
		 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
		 I2C_HAL_SendAck(base);
	 }
 	 I2C_HAL_WriteByte(base, 0x01);
 	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
 	 I2C_HAL_SendNak(base);

 	 check = I2C_HAL_SendStop(base);

 	 pause();


 	 I2C_HAL_SendStart(base);

 	 I2C_HAL_WriteByte(base, 0x98);
 	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
 	 I2C_HAL_SendAck(base);

 	 I2C_HAL_WriteByte(base, 0x00);
 	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
 	 I2C_HAL_SendAck(base);


 	 I2C_HAL_SendStart(base);

 	 I2C_HAL_WriteByte(base, 0x99);
 	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
 	 I2C_HAL_SendAck(base);

 	 I2C_HAL_SetDirMode(base, kI2CReceive);


 	 //waveform---- iic data register
 	 //0x15----99,0x80----15,0x01----80,0x01----01,height----01
	 for(cnt=0; cnt<0x05; cnt++)
	 {
		 I2C_HAL_ReadByte(base);
		 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
		 I2C_HAL_SendAck(base);
	 }

 	finresult = I2C_HAL_ReadByte(base);				//0x00----height
 	I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
 	I2C_HAL_SendNak(base);

 	result = I2C_HAL_ReadByte(base);				//this is very important
 	printf("finresult = %d\n",finresult);					//why important

 //	pause();
 	check = I2C_HAL_SendStop(base);

 	 pause();

 	 if(check)
 		 return check;
 	 else
 		 return finresult;
 }


uint8_t U1P_9555_read_weight(void)
{
	 uint8_t finresult;
	 uint16_t result;
	 i2c_status_t check;
	 uint32_t instance = BOARD_I2C_INSTANCE;
	 I2C_Type * base = g_i2cBase[instance];

	 I2C_DRV_MasterSetBaudRate(instance, &slave);
	 /* Set direction to send for sending of address and data. */
	 I2C_HAL_SetDirMode(base, kI2CSend);

	  /* Generate start signal. */
	 I2C_HAL_SendStart(base);				//start

	 uint8_t command[5] = {0x98,0x00,0x15,0x10,0x09};
	 uint8_t cnt;
	 for(cnt=0; cnt<0x05; cnt++)
	 {
		 I2C_HAL_WriteByte(base, command[cnt]);						//iic write
		 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
		 I2C_HAL_SendAck(base);
	 }

	 I2C_HAL_WriteByte(base, 0x01);
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendNak(base);

	 I2C_HAL_SendStop(base);

	 pause();

	 I2C_HAL_SendStart(base);

	 I2C_HAL_WriteByte(base, 0x98);
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendAck(base);

	 I2C_HAL_WriteByte(base, 0x00);
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendAck(base);


	 I2C_HAL_SendStart(base);

	 I2C_HAL_WriteByte(base, 0x99);
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendAck(base);

	 I2C_HAL_SetDirMode(base, kI2CReceive);

	//waveform---- iic data register
	//0x15----99,0x80----15,0x01----80,0x01----01,weight----01
	 for(cnt=0; cnt<0x05; cnt++)
	 {
		 result = I2C_HAL_ReadByte(base);
		 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
		 I2C_HAL_SendAck(base);
	 }

	 finresult = I2C_HAL_ReadByte(base);				   //0x00----weight
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendNak(base);

	 result = I2C_HAL_ReadByte(base);				//this is very important
	 printf("result = %d\n",result);
	 check = I2C_HAL_SendStop(base);

	 pause();

	 if(check)
		 return check;
	 else
		 return finresult;
}

uint8_t U1P_9555_set_sensitivity(uint8_t sensitivity)
 {
	 uint8_t cnt;
	 i2c_status_t check;
	 uint32_t instance = BOARD_I2C_INSTANCE;
	 uint8_t command[6] = {0x98,0x00,0x15,0x20,0x0D,0x01};
	 I2C_Type * base = g_i2cBase[instance];
	 I2C_DRV_MasterSetBaudRate(instance, &slave);
	 I2C_HAL_SetDirMode(base, kI2CSend);

	 I2C_HAL_SendStart(base);

	 for(cnt=0; cnt<6; cnt++)
	 {
		 I2C_HAL_WriteByte(base, command[cnt]);
		 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
		 I2C_HAL_SendAck(base);

	 }

	 I2C_HAL_WriteByte(base, sensitivity);
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendNak(base);

	 check = I2C_HAL_SendStop(base);
	 pause();

	 return check;
 }

 uint8_t U1P_9555_config(uint8_t* user_data)
{
	 uint8_t cnt;
	 i2c_status_t check;
	 uint32_t instance = BOARD_I2C_INSTANCE;
	 uint8_t command[6] = {0x98,0x00,0x15,0x20,0x00,0x11};
	 I2C_Type * base = g_i2cBase[instance];
	 I2C_DRV_MasterSetBaudRate(instance, &slave);
	 I2C_HAL_SetDirMode(base, kI2CSend);

	 I2C_HAL_SendStart(base);

	 for(cnt=0; cnt<6; cnt++)
	 {
		 I2C_HAL_WriteByte(base, command[cnt]);
		 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
		 I2C_HAL_SendAck(base);

	 }

	 for(cnt=0; cnt<16; cnt++)
	 {
		 I2C_HAL_WriteByte(base, user_data[cnt]);
		 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
		 I2C_HAL_SendAck(base);
	//	 printf("user_data = %d\n",user_data[cnt]);
	 }

	 I2C_HAL_WriteByte(base, user_data[cnt]);
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendNak(base);

	 check = I2C_HAL_SendStop(base);
	 pause();

	 return check;
}

uint8_t U1P_9555_read_config(void)
{
	uint16_t result;
	i2c_status_t check;
	uint32_t instance = BOARD_I2C_INSTANCE;
	I2C_Type * base = g_i2cBase[instance];

	I2C_DRV_MasterSetBaudRate(instance, &slave);
	/* Set direction to send for sending of address and data. */
	I2C_HAL_SetDirMode(base, kI2CSend);

	/* Generate start signal. */
	I2C_HAL_SendStart(base);				//start

	uint8_t command[5] = {0x98,0x00,0x15,0x10,0x00};				// 10----read config register, 00----offset
	uint8_t cnt;
	for(cnt=0; cnt<0x05; cnt++)
	{
		I2C_HAL_WriteByte(base, command[cnt]);						//iic write
	 	I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 	I2C_HAL_SendAck(base);
	}

	I2C_HAL_WriteByte(base, 0x10);									// 10----bytes to read
	I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	I2C_HAL_SendNak(base);

	I2C_HAL_SendStop(base);

	pause();

	I2C_HAL_SendStart(base);

	I2C_HAL_WriteByte(base, 0x98);
	I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendAck(base);

	 I2C_HAL_WriteByte(base, 0x00);
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendAck(base);


	 I2C_HAL_SendStart(base);

	 I2C_HAL_WriteByte(base, 0x99);
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendAck(base);

	 I2C_HAL_SetDirMode(base, kI2CReceive);

	//waveform---- iic data register
	//0x15----99,0x80----15,0x01----80,0x01----01,weight----01
	 for(cnt=0; cnt<0x15; cnt++)							// 0x15----bytes to read
	 {
		 result = I2C_HAL_ReadByte(base);
		 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
		 I2C_HAL_SendAck(base);
		 printf("result = %d\n",result);
	 }

	 result = I2C_HAL_ReadByte(base);				//0x00----weight
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendNak(base);
	 printf("result = %d\n",result);
	 result = I2C_HAL_ReadByte(base);				//this is very important
	 printf("result = %d\n",result);
	 check = I2C_HAL_SendStop(base);

	 pause();
	 return check;
}

uint8_t U1P_9555_read_activity(void)
{
	 uint8_t activity;
	 uint16_t result;
	 i2c_status_t check;
	 uint32_t instance = BOARD_I2C_INSTANCE;
	 I2C_Type * base = g_i2cBase[instance];
	 I2C_DRV_MasterSetBaudRate(instance, &slave);

	 I2C_HAL_SetDirMode(base, kI2CSend);


	 I2C_HAL_SendStart(base);				//start

	 uint8_t command[5] = {0x98,0x00,0x15,0x30,0x00};		// 30-read pedometer status register, 02-cal offset
	 uint8_t cnt;
	 for(cnt=0; cnt<0x05; cnt++)
	 {
		 I2C_HAL_WriteByte(base, command[cnt]);						//iic write
		 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
		 I2C_HAL_SendAck(base);
	 }

	 I2C_HAL_WriteByte(base, 0x01);							// read two bytes
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendNak(base);

	 I2C_HAL_SendStop(base);

	 pause();

	 I2C_HAL_SendStart(base);

	 I2C_HAL_WriteByte(base, 0x98);							// iic write
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendAck(base);

	 I2C_HAL_WriteByte(base, 0x00);							// 9555 register offset
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendAck(base);


	 I2C_HAL_SendStart(base);

	 I2C_HAL_WriteByte(base, 0x99);
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);			// iic read
	 I2C_HAL_SendAck(base);

	 I2C_HAL_SetDirMode(base, kI2CReceive);

	//waveform---- iic data register
	//0x15----99,0x80----15,0x01----80,0x01----01,0x00----01
	 for(cnt=0; cnt<0x05; cnt++)
	 {
		 result = I2C_HAL_ReadByte(base);
		 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
		 I2C_HAL_SendAck(base);
//		 printf("result = %d\n",result);
	 }

	 activity = I2C_HAL_ReadByte(base);				//
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendNak(base);

	 result = I2C_HAL_ReadByte(base);				//this is very important
	 printf("activity = %d\r\n",activity);
	 check = I2C_HAL_SendStop(base);

	 pause();
	 if(check)
	 {
		 printf("\r------fail！！--------\r\n");
		 return check;
	 }
	 else
		 return activity;
}

uint16_t U1P_9555_pedo_status(uint8_t pedo_data)
{
	 uint8_t status_msb,status_lsb;
	 uint16_t result;
	 i2c_status_t check;
	 uint32_t instance = BOARD_I2C_INSTANCE;
	 I2C_Type * base = g_i2cBase[instance];
	 I2C_DRV_MasterSetBaudRate(instance, &slave);

	 I2C_HAL_SetDirMode(base, kI2CSend);


	 I2C_HAL_SendStart(base);				//start

	 uint8_t command[5] = {0x98,0x00,0x15,0x30,pedo_data};		// 30-read pedometer status register, 08-cal offset
	 uint8_t cnt;
	 for(cnt=0; cnt<0x05; cnt++)
	 {
		 I2C_HAL_WriteByte(base, command[cnt]);						//iic write
		 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
		 I2C_HAL_SendAck(base);
	 }

	 I2C_HAL_WriteByte(base, 0x02);							// read two bytes
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendNak(base);

	 I2C_HAL_SendStop(base);

	 pause();

	 I2C_HAL_SendStart(base);

	 I2C_HAL_WriteByte(base, 0x98);							// iic write
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendAck(base);

	 I2C_HAL_WriteByte(base, 0x00);							// 9555 register offset
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendAck(base);


	 I2C_HAL_SendStart(base);

	 I2C_HAL_WriteByte(base, 0x99);
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);			// iic read
	 I2C_HAL_SendAck(base);

	 I2C_HAL_SetDirMode(base, kI2CReceive);

	//waveform---- iic data register
	//0x15----99,0x80----15,0x02----80,0x02----02,cal_MSB----02,cal_LSB----00
	 for(cnt=0; cnt<0x05; cnt++)
	 {
		 result = I2C_HAL_ReadByte(base);
		 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
		 I2C_HAL_SendAck(base);
	 }

	 status_msb = I2C_HAL_ReadByte(base);				// cal_LSB----cal_MSB
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendAck(base);

	 status_lsb = I2C_HAL_ReadByte(base);				// 0x00----cal_LSB
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendNak(base);

	 result = I2C_HAL_ReadByte(base);				//this is very important
	// printf("status_lsb = %d\r\n",status_lsb);
		 OSA_TimeDelay(1);
	 check = I2C_HAL_SendStop(base);

	 pause();

	 if(check){
		 printf("fail------------\r\n");
		 return check;
	 }

	 else
		 return ((status_msb << 2) | status_lsb);
}

uint8_t U1P_9555_wakeup(uint8_t sleep_mode)
{
	 uint8_t cnt;
	 i2c_status_t check;
	 uint32_t instance = BOARD_I2C_INSTANCE;

	 /* wake up from deep sleep mode  command: 00 12 20 06 01 00 */
	 uint8_t command[6] = {0x98,0x00,0x12,0x20,0x06,0x01};
	 I2C_Type * base = g_i2cBase[instance];
	 I2C_DRV_MasterSetBaudRate(instance, &slave);
	 I2C_HAL_SetDirMode(base, kI2CSend);

	 I2C_HAL_SendStart(base);

	 for(cnt=0; cnt<6; cnt++)
	 {
		 I2C_HAL_WriteByte(base, command[cnt]);
		 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
		 I2C_HAL_SendAck(base);

	 }


	 I2C_HAL_WriteByte(base, sleep_mode);
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendNak(base);

	 check = I2C_HAL_SendStop(base);

	 pause();

	 return check;
}

uint8_t U1P_9555_read_sixdir(void)
{
	 uint8_t cur_status,debounce_cnt;
	 uint16_t result;
	 i2c_status_t check;
	 uint32_t instance = BOARD_I2C_INSTANCE;
	 I2C_Type * base = g_i2cBase[instance];
	 I2C_DRV_MasterSetBaudRate(instance, &slave);

	 I2C_HAL_SetDirMode(base, kI2CSend);


	 I2C_HAL_SendStart(base);				//start

	 uint8_t command[5] = {0x98,0x00,0x1A,0x30,0x00};		// 1A-six direction app, 30-----read status register, 02-two bytes
	 uint8_t cnt;
	 for(cnt=0; cnt<0x05; cnt++)
	 {
		 I2C_HAL_WriteByte(base, command[cnt]);						//iic write
		 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
		 I2C_HAL_SendAck(base);
	 }

	 I2C_HAL_WriteByte(base, 0x02);							// read two bytes
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendNak(base);

	 I2C_HAL_SendStop(base);

	 pause();

	 I2C_HAL_SendStart(base);

	 I2C_HAL_WriteByte(base, 0x98);							// iic write
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendAck(base);

	 I2C_HAL_WriteByte(base, 0x00);							// 9555 register offset
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendAck(base);


	 I2C_HAL_SendStart(base);

	 I2C_HAL_WriteByte(base, 0x99);
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);			// iic read
	 I2C_HAL_SendAck(base);

	 I2C_HAL_SetDirMode(base, kI2CReceive);

	//waveform---- iic data register
	//0x1A----99,0x80----1A,0x02----80,0x02----02,first byte of six direction status register----02
	 for(cnt=0; cnt<0x05; cnt++)
	 {
		 result = I2C_HAL_ReadByte(base);
		 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
		 I2C_HAL_SendAck(base);
//		 printf("result = %d\n",result);
	 }

	 cur_status = I2C_HAL_ReadByte(base);				// second byte----first byte of six direction status register
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendAck(base);

	 debounce_cnt = I2C_HAL_ReadByte(base);				// 00----second byte of six direction status register
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendNak(base);

	 result = I2C_HAL_ReadByte(base);				//this is very important
	 printf("debounce_cnt = %d\r\n",debounce_cnt);
	 check = I2C_HAL_SendStop(base);

	 pause();
	 if(check)
	 {
		 printf("\r------fail！！--------\r\n");
		 return check;
	 }
	 else
		 return cur_status;

}

uint8_t U1P_9555_pedometer_reset(void)
{
	 uint8_t cnt;
	 i2c_status_t check;
	 uint32_t instance = BOARD_I2C_INSTANCE;

	 /* reset the pedometer application: 00 17 20 01 01 20 */
	 uint8_t command[6] = {0x98,0x00,0x17,0x20,0x01,0x01};
	 I2C_Type * base = g_i2cBase[instance];
	 I2C_DRV_MasterSetBaudRate(instance, &slave);
	 I2C_HAL_SetDirMode(base, kI2CSend);

	 I2C_HAL_SendStart(base);

	 for(cnt=0; cnt<6; cnt++)
	 {
		 I2C_HAL_WriteByte(base, command[cnt]);
		 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
		 I2C_HAL_SendAck(base);

	 }


	 I2C_HAL_WriteByte(base, 0x20);
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendNak(base);

	 check = I2C_HAL_SendStop(base);

	 pause();

	 return check;
}

uint8_t U1P_9555_pedometer_suspend(uint8_t mode_suspend)
{
	 uint8_t cnt;
	 i2c_status_t check;
	 uint32_t instance = BOARD_I2C_INSTANCE;

	 /* reset the pedometer application: 00 17 20 05 01 mode_suspend */
	 uint8_t command[6] = {0x98,0x00,0x17,0x20,0x05,0x01};
	 I2C_Type * base = g_i2cBase[instance];
	 I2C_DRV_MasterSetBaudRate(instance, &slave);
	 I2C_HAL_SetDirMode(base, kI2CSend);

	 I2C_HAL_SendStart(base);

	 for(cnt=0; cnt<6; cnt++)
	 {
		 I2C_HAL_WriteByte(base, command[cnt]);
		 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
		 I2C_HAL_SendAck(base);

	 }


	 I2C_HAL_WriteByte(base, mode_suspend);
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendNak(base);

	 check = I2C_HAL_SendStop(base);

	 pause();

	 return check;

}

uint8_t U1P_9555_int_config(uint8_t int_app, uint8_t act_flag, uint8_t GPIO_OFFSET)
{
	 uint8_t cnt;
	 i2c_status_t check;
	 uint32_t instance = BOARD_I2C_INSTANCE;

	 /* GPIO-AppMap Application----configure direction change interrupt: 00 03 20 00 02 1A 07 */
	 //uint8_t gpio_appmap[7] = {0x98,0x00,0x03,0x20,0x00,0x02,0x1A};//device direction change
	 uint8_t gpio_appmap[7] = {0x98,0x00,0x03,0x20,GPIO_OFFSET,0x02,int_app};//merged flag
	 I2C_Type * base = g_i2cBase[instance];
	 I2C_DRV_MasterSetBaudRate(instance, &slave);
	 I2C_HAL_SetDirMode(base, kI2CSend);

	 I2C_HAL_SendStart(base);

	 for(cnt=0; cnt<7; cnt++)
	 {
		 I2C_HAL_WriteByte(base, gpio_appmap[cnt]);
		 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
		 I2C_HAL_SendAck(base);

	 }


	 I2C_HAL_WriteByte(base, act_flag);
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendNak(base);

	 check = I2C_HAL_SendStop(base);

	 pause();

	 return check;
}

uint8_t U1P_9555_gpio_read(void)
{
	 uint8_t gpio;
	 uint16_t result;
	 i2c_status_t check;
	 uint32_t instance = BOARD_I2C_INSTANCE;
	 I2C_Type * base = g_i2cBase[instance];
	 I2C_DRV_MasterSetBaudRate(instance, &slave);

	 I2C_HAL_SetDirMode(base, kI2CSend);


	 I2C_HAL_SendStart(base);				//start

	 uint8_t command[5] = {0x98,0x00,0x19,0x10,0x01};		// 30-read pedometer status register, 08-cal offset
	 uint8_t cnt;
	 for(cnt=0; cnt<0x05; cnt++)
	 {
		 I2C_HAL_WriteByte(base, command[cnt]);						//iic write
		 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
		 I2C_HAL_SendAck(base);
	 }

	 I2C_HAL_WriteByte(base, 0x01);							// read two bytes
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendNak(base);

	 I2C_HAL_SendStop(base);

	 pause();

	 I2C_HAL_SendStart(base);

	 I2C_HAL_WriteByte(base, 0x98);							// iic write
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendAck(base);

	 I2C_HAL_WriteByte(base, 0x00);							// 9555 register offset
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendAck(base);


	 I2C_HAL_SendStart(base);

	 I2C_HAL_WriteByte(base, 0x99);
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);			// iic read
	 I2C_HAL_SendAck(base);

	 I2C_HAL_SetDirMode(base, kI2CReceive);

	//waveform---- iic data register
	//0x15----99,0x80----15,0x02----80,0x02----02,cal_MSB----02,cal_LSB----00
	 for(cnt=0; cnt<0x05; cnt++)
	 {
		 result = I2C_HAL_ReadByte(base);
		 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
		 I2C_HAL_SendAck(base);
	 }

	 gpio = I2C_HAL_ReadByte(base);				// cal_LSB----cal_MSB
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendNak(base);

	 result = I2C_HAL_ReadByte(base);				//this is very important
	 printf("gpio = %d\r\n",gpio);
	 check = I2C_HAL_SendStop(base);

	 pause();

	 if(check){
		 printf("fail------------\r\n");
		 return check;
	 }

	 else
		 return gpio;
}

uint8_t U1P_9555_gpio_set(uint8_t mask, uint8_t gpio_mask)
{
	 uint8_t cnt, command;
	 i2c_status_t check;
	 uint32_t instance = BOARD_I2C_INSTANCE;

	 I2C_Type * base = g_i2cBase[instance];
	 I2C_DRV_MasterSetBaudRate(instance, &slave);

	 /* GPIO Input/Output Application----configure direction change interrupt: 00 19 20 01 01 (AB & 6F) */
	 uint8_t gpio_io[6] = {0x98,0x00,0x19,0x20,0x01,0x01};
	 I2C_HAL_SetDirMode(base, kI2CSend);

	 I2C_HAL_SendStart(base);

	 for(cnt=0; cnt<6; cnt++)
	 {
		 I2C_HAL_WriteByte(base, gpio_io[cnt]);
		 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
		 I2C_HAL_SendAck(base);

	 }

	 //command = 0xAB & 0x6F;
	 command = mask & gpio_mask;

	 I2C_HAL_WriteByte(base, command);
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendNak(base);

	 check = I2C_HAL_SendStop(base);

	 pause();
}


uint8_t U1P_9555_int_pin_config(void)
{
	 uint8_t cnt,command;
	 i2c_status_t check;
	 uint32_t instance = BOARD_I2C_INSTANCE;

	 I2C_Type * base = g_i2cBase[instance];
	 I2C_DRV_MasterSetBaudRate(instance, &slave);

	 /* GPIO Input/Output Application----configure direction change interrupt: 00 03 20 09 01 40 */
	 uint8_t gpio_io[6] = {0x98,0x00,0x03,0x20,0x09,0x01};
	 I2C_HAL_SetDirMode(base, kI2CSend);

	 I2C_HAL_SendStart(base);

	 for(cnt=0; cnt<6; cnt++)
	 {
		 I2C_HAL_WriteByte(base, gpio_io[cnt]);
		 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
		 I2C_HAL_SendAck(base);

	 }

	 I2C_HAL_WriteByte(base, 0x40);
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendNak(base);

	 check = I2C_HAL_SendStop(base);

	 pause();

}

uint8_t pedometer_int0_enable(void)
{
	 uint8_t cnt;
	 i2c_status_t check;
	 uint32_t instance = BOARD_I2C_INSTANCE;

	 /* pedometer_int0_enable: 00 18 20 00 01 */
	 uint8_t command[6] = {0x98,0x00,0x18,0x20,0x00,0x01};
	 I2C_Type * base = g_i2cBase[instance];
	 I2C_DRV_MasterSetBaudRate(instance, &slave);
	 I2C_HAL_SetDirMode(base, kI2CSend);

	 I2C_HAL_SendStart(base);

	 for(cnt=0; cnt<6; cnt++)
	 {
		 I2C_HAL_WriteByte(base, command[cnt]);
		 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
		 I2C_HAL_SendAck(base);

	 }


	 I2C_HAL_WriteByte(base, 0xc0);
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendNak(base);

	 check = I2C_HAL_SendStop(base);

	 pause();

	 return check;
}

uint8_t U1P_9555_AFE_config(uint8_t g_mode)
{
	 uint8_t cnt;
	 i2c_status_t check;
	 uint32_t instance = BOARD_I2C_INSTANCE;

	 /* AFE mode choose: 00 06 20 00 01 */
	 uint8_t command[6] = {0x98,0x00,0x06,0x20,0x00,0x01};
	 I2C_Type * base = g_i2cBase[instance];
	 I2C_DRV_MasterSetBaudRate(instance, &slave);
	 I2C_HAL_SetDirMode(base, kI2CSend);

	 I2C_HAL_SendStart(base);

	 for(cnt=0; cnt<6; cnt++)
	 {
		 I2C_HAL_WriteByte(base, command[cnt]);
		 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
		 I2C_HAL_SendAck(base);

	 }


	 I2C_HAL_WriteByte(base, g_mode);
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendNak(base);

	 check = I2C_HAL_SendStop(base);

	 pause();

	 return check;

}


uint8_t U1P_9555_read_xyz(uint8_t afe_status)
{
	 uint16_t xyzdata[11];
	 uint16_t result;
	 i2c_status_t check;
	 uint32_t instance = BOARD_I2C_INSTANCE;
	 I2C_Type * base = g_i2cBase[instance];
	 I2C_DRV_MasterSetBaudRate(instance, &slave);

	 I2C_HAL_SetDirMode(base, kI2CSend);


	 I2C_HAL_SendStart(base);				//start

	 uint8_t command[5] = {0x98,0x00,0x06,0x30,afe_status};		// AFE application, 12-raw data offset
	 uint8_t cnt;
	 for(cnt=0; cnt<0x05; cnt++)
	 {
		 I2C_HAL_WriteByte(base, command[cnt]);						//iic write
		 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
		 I2C_HAL_SendAck(base);
	 }

	 I2C_HAL_WriteByte(base, 0x06);							// read two bytes
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendNak(base);

	 I2C_HAL_SendStop(base);

	 pause();

	 I2C_HAL_SendStart(base);

	 I2C_HAL_WriteByte(base, 0x98);							// iic write
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendAck(base);

	 I2C_HAL_WriteByte(base, 0x00);							// 9555 register offset
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendAck(base);


	 I2C_HAL_SendStart(base);

	 I2C_HAL_WriteByte(base, 0x99);
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);			// iic read
	 I2C_HAL_SendAck(base);

	 I2C_HAL_SetDirMode(base, kI2CReceive);

	//waveform---- iic data register
	//0x15----99,0x80----15,0x02----80,0x02----02,cal_MSB----02,cal_LSB----00
	 for(cnt=0; cnt<0x0a; cnt++)
	 {
		 result = I2C_HAL_ReadByte(base);
		 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
		 I2C_HAL_SendAck(base);
		 xyzdata[cnt] = result;
	 }

	 xyzdata[cnt] = I2C_HAL_ReadByte(base);				// cal_LSB----cal_MSB
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendNak(base);

	 I2C_HAL_ReadByte(base);				//this is very important
	 printf("result = %d\r\n",xyzdata[cnt]);
	 check = I2C_HAL_SendStop(base);

	 for(cnt=0; cnt<11; cnt++)
		 printf("/r xyzdata[%d] = %d\r\n",cnt,xyzdata[cnt]);
	 pause();
	 if(xyzdata[5] > 127)
	 {
		 result = ((0xff - xyzdata[5])*256 + xyzdata[6]);
		 printf("\rx = -%d\r\n",result);
	 }
	 else
	 {
		 result = (xyzdata[5]*256 + xyzdata[6]);
		 printf("\rx = %d\r\n",result);
	 }

	 if(xyzdata[7] > 127)
	 {
		 result = ((0xff - xyzdata[7])*256 + xyzdata[8]);
		 printf("\ry = -%d\r\n",result);
	 }
	 else
	 {
		 result = (xyzdata[7]*256 + xyzdata[8]);
		 printf("\ry = %d\r\n",result);
	 }

	 if(xyzdata[9] > 127)
	 {
		 result = ((0xff - xyzdata[9])*256 + xyzdata[10]);
		 printf("\rz = -%d\r\n",result);
	 }
	 else
	 {
		 result = ((xyzdata[9])*256 + xyzdata[10]);
		 printf("\rz = %d\r\n",result);
	 }

	 if(check){
		 printf("fail------------\r\n");
		 return check;
	 }

	 else
		 return result;

}

uint8_t U1P_9555_read_temp(void)
{
	 uint8_t temp_M,temp_L;
	 uint16_t result;
	 i2c_status_t check;
	 uint32_t instance = BOARD_I2C_INSTANCE;
	 I2C_Type * base = g_i2cBase[instance];
	 I2C_DRV_MasterSetBaudRate(instance, &slave);

	 I2C_HAL_SetDirMode(base, kI2CSend);


	 I2C_HAL_SendStart(base);				//start

	 uint8_t command[5] = {0x98,0x00,0x06,0x30,0x24};		// 30-read pedometer status register, 08-cal offset
	 uint8_t cnt;
	 for(cnt=0; cnt<0x05; cnt++)
	 {
		 I2C_HAL_WriteByte(base, command[cnt]);						//iic write
		 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
		 I2C_HAL_SendAck(base);
	 }

	 I2C_HAL_WriteByte(base, 0x02);							// read two bytes
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendNak(base);

	 I2C_HAL_SendStop(base);

	 pause();

	 I2C_HAL_SendStart(base);

	 I2C_HAL_WriteByte(base, 0x98);							// iic write
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendAck(base);

	 I2C_HAL_WriteByte(base, 0x00);							// 9555 register offset
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendAck(base);


	 I2C_HAL_SendStart(base);

	 I2C_HAL_WriteByte(base, 0x99);
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);			// iic read
	 I2C_HAL_SendAck(base);

	 I2C_HAL_SetDirMode(base, kI2CReceive);

	//waveform---- iic data register
	//0x15----99,0x80----15,0x02----80,0x02----02,cal_MSB----02,cal_LSB----00
	 for(cnt=0; cnt<0x05; cnt++)
	 {
		 result = I2C_HAL_ReadByte(base);
		 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
		 I2C_HAL_SendAck(base);
	 }

	 temp_M = I2C_HAL_ReadByte(base);				// cal_LSB----cal_MSB
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendNak(base);

	 temp_L = I2C_HAL_ReadByte(base);				//this is very important
	 printf("temp_L = %d\r\n",temp_L);
	 check = I2C_HAL_SendStop(base);

	 pause();
	 printf("temp_M = %d\r\n",temp_M);
	 if(check){
		 printf("fail------------\r\n");
		 return check;
	 }

	 else
		 return ((temp_M<<2)|temp_L);

}

uint8_t U1P_9555_open_temp(void)
{
	 uint8_t cnt;
	 i2c_status_t check;
	 uint32_t instance = BOARD_I2C_INSTANCE;

	 uint8_t command[6] = {0x98,0x00,0x06,0x20,0x00,0x01};
	 I2C_Type * base = g_i2cBase[instance];
	 I2C_DRV_MasterSetBaudRate(instance, &slave);
	 I2C_HAL_SetDirMode(base, kI2CSend);

	 I2C_HAL_SendStart(base);

	 for(cnt=0; cnt<6; cnt++)
	 {
		 I2C_HAL_WriteByte(base, command[cnt]);
		 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
		 I2C_HAL_SendAck(base);

	 }


	 I2C_HAL_WriteByte(base, 0x30);
	 I2C_DRV_MasterWait(instance, I2C_TIMEOUT_MS);
	 I2C_HAL_SendNak(base);

	 check = I2C_HAL_SendStop(base);

	 pause();

	 return check;
}
