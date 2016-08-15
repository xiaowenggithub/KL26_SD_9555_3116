/**********************************************
 /	draft: CY3116 I2C TEST
 /**************************************************/

#include "U1P_CY3116.h"
#include "main.h"

const i2c_device_t slave_3116 =
{
	.address = 0x37,
	.baudRate_kbps = 100
};



void CY3116_Version_Test(void) {
	uint8_t i;
	uint8_t cmdBuff[1] = { 0xFF };
	uint8_t receiveBuff[1] = { 0xFF };

	i2c_status_t returnValue;

	i2c_device_t CY3116_slave = { .address = SLAVE_CY3116_ADDR, .baudRate_kbps =
			100 };

	while (1) {
		OSA_TimeDelay(100);

		cmdBuff[0] = 0x8f;//閻庨潧瀚悺銊╁闯閵娧勬殢濞存粣鎷�闁革絻鍔嬪▎銏㈠寲鐠囨彃鐏�ID闁靛棗鍊搁顔界閿熺春Y8CMBR3xxx 闁革絻鍔嬪▎銏ゆ嚀瀹�啠鏋呴柨娑樼焷椤曟碍鎷呭鍛憻闁煎搫鍊诲▓鎴﹀磹閸忓吋笑 154闁靛棴鎷�		printf("Version Test\n\r");
		returnValue = I2C_DRV_MasterReceiveData(BOARD_I2C_INSTANCE,
				&CY3116_slave, cmdBuff, 1, receiveBuff, sizeof(receiveBuff));
		if (returnValue == kStatus_I2C_Success) {
			printf("[%x]		%x\n\r", cmdBuff[0], receiveBuff[0]);
/*			GPIO_DRV_TogglePinOutput(kGpioLED2);
			OSA_TimeDelay(500);
			GPIO_DRV_TogglePinOutput(kGpioLED2);
			OSA_TimeDelay(500);	*/
		//	break;

		} else
			{printf("I2C fained, error code:%d\n\r", returnValue);

		printf("receiveBuff = %x", receiveBuff[0]);
			}
	}
	OSA_TimeDelay(100);
	GPIO_DRV_TogglePinOutput(kGpioLED2);
	OSA_TimeDelay(500);
	GPIO_DRV_TogglePinOutput(kGpioLED2);
	OSA_TimeDelay(500);
	GPIO_DRV_TogglePinOutput(kGpioLED2);
	OSA_TimeDelay(500);
	GPIO_DRV_TogglePinOutput(kGpioLED2);
	OSA_TimeDelay(500);
	printf("OUT \n");
	//while (1);
		//stop

}

void CY3116_Touch_Test(void) {
	uint8_t cmdBuff[1] = { 0xFF };
	uint8_t receiveBuff[1] = { 0xFF };

	i2c_status_t returnValue;

	i2c_device_t CY3116_slave = { .address = SLAVE_CY3116_ADDR, .baudRate_kbps =
			100 };
	printf("Touch  TEST \r\n");

	while (1) {
		cmdBuff[0] = 0xaa;		// Button_stat register addr
		returnValue = I2C_DRV_MasterReceiveData(
		BOARD_I2C_INSTANCE, &CY3116_slave, cmdBuff, 1, receiveBuff,
				sizeof(receiveBuff));

		if (returnValue == kStatus_I2C_Success) {
			PRINTF("\r\n[%d]                      %x", cmdBuff[0],
					receiveBuff[0]);
			printf("Read button status success\r\n");
		}

		else {
			PRINTF("\r\nI2C communication failed, error code: %d", returnValue);
		}

		OSA_TimeDelay(100);
	}

}

void CY3116_Gesture_Reco(void) {
	uint8_t cmdBuff[1] = { 0xFF };
	uint8_t receiveBuff[1] = { 0x00 };

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
			printf("I2C communication failed first times 1, error code: %d\r\n", returnValue);
		//		break;
		}

		printf("receiveBuff[0] = %x\r\n", receiveBuff[0]);
		if ((receiveBuff[0] == 0x40) | (receiveBuff[0] == 0x41))// ignore the proximity sensor
				{
			printf("First Button Pressed first\r\n");
			//	OSA_TimeDelay(50);
			_Init: while ((receiveBuff[0] & 0x01) == 1)	// wait for next detection
			{
				OSA_TimeDelay(50);
				returnValue2 = I2C_DRV_MasterReceiveData(
				BOARD_I2C_INSTANCE, &CY3116_slave, cmdBuff, 1, receiveBuff,
						sizeof(receiveBuff));

				if (returnValue2 != kStatus_I2C_Success) {
					printf("I2C communication failed first times 2 , error code: %d\r\n",
							returnValue2);
						break;
				}

				if ((receiveBuff[0] & 0x01) != 1)
					break;	//One tap only

				if ((receiveBuff[0] == 0x21) | (receiveBuff[0] == 0x20)) {
					printf("Gesture Detected\r\n");
					printf("Buttom to Top\r\n");
					OSA_TimeDelay(500);

					gesture = gestureup;

					GPIO_DRV_TogglePinOutput(kGpioLED2);
					OSA_TimeDelay(500);
					GPIO_DRV_TogglePinOutput(kGpioLED2);
					OSA_TimeDelay(500);
					break;
				} else
					goto _Init;

			}

			printf("End of the gesture first.\r\n");
			OSA_TimeDelay(100);
		}
		else
		{
			//printf("Wait for the gesture\n");
			OSA_TimeDelay(100);
		}

		if ((receiveBuff[0] == 0x20) | (receiveBuff[0] == 0x21))// ignore the proximity sensor
				{
			printf("First Button Pressed second\r\n");
			//	OSA_TimeDelay(50);
			_Init1: while ((receiveBuff[0] & 0x01) == 1)	// wait for next detection
			{
				OSA_TimeDelay(50);
				returnValue2 = I2C_DRV_MasterReceiveData(
				BOARD_I2C_INSTANCE, &CY3116_slave, cmdBuff, 1, receiveBuff,
						sizeof(receiveBuff));

				if (returnValue2 != kStatus_I2C_Success) {
					printf("I2C communication failed second times 1, error code: %d\r\n",
							returnValue2);
					//	break;
				}

				if ((receiveBuff[0] & 0x01) != 1)
					break;	//One tap only

				if ((receiveBuff[0] == 0x41) | (receiveBuff[0] == 0x40)) {
					printf("Gesture Detected\r\n");
					printf("Buttom to Top\r\n");
					OSA_TimeDelay(500);
					GPIO_DRV_TogglePinOutput(kGpioLED1);
					OSA_TimeDelay(500);
					gesture = gesturedown;
					GPIO_DRV_TogglePinOutput(kGpioLED1);
					OSA_TimeDelay(500);
					break;
				} else
					goto _Init1;

			}

			printf("End of the gesture second.\r\n");
			OSA_TimeDelay(100);
		}
		else
		{
			//printf("Wait for the gesture\n");
			OSA_TimeDelay(100);
		}


	}

	printf("End of the function\n");
}

void Test_Proximity(void) {
	uint8_t i;
	uint8_t cmdBuff[3] = { 0xFF };
	uint8_t receiveBuff[3] = { 0xFF };

	i2c_status_t returnValue1 = 0, returnValue2 = 0;

	i2c_device_t CY3116_slave = { .address = SLAVE_CY3116_ADDR, .baudRate_kbps =
			100 };

	while (1) {
		//OSA_TimeDelay(100);

		cmdBuff[1] = 0xae;		//prox stat register
		cmdBuff[2] = 0xaa;		//button stat register

		printf("Version Test\n\r");
		returnValue1 = I2C_DRV_MasterReceiveData(BOARD_I2C_INSTANCE,
				&CY3116_slave, &(cmdBuff[1]), 1, &(receiveBuff[1]),
				sizeof(receiveBuff[1]));
		OSA_TimeDelay(1);		//wait for first receiving. can't be eliminated
		returnValue2 = I2C_DRV_MasterReceiveData(BOARD_I2C_INSTANCE,
				&CY3116_slave, &(cmdBuff[2]), 1, &(receiveBuff[2]),
				sizeof(receiveBuff[2]));

		printf("return value 1 = %d, return value 2 = %d\r\n", returnValue1,
				returnValue2);
		if ((returnValue1 != kStatus_I2C_Success)
				|| (returnValue2 != kStatus_I2C_Success)) {
			printf("Failed. \r\n");
					OSA_TimeDelay(50);
		}

		else {
			printf("prox stat : receiveBuff[1] = %x\r\n", receiveBuff[1]);
			printf("button stat : receiveBuff[2] = %x\r\n", receiveBuff[2]);
		}

		OSA_TimeDelay(100);

	}
}

void CY3116_Set_Config_Test(void) {
	//initialize
	uint8_t cmdBuff[2] = { 0xFF };
	uint16_t receiveBuff_16 = { 0x0000 };
	uint8_t receiveBuff_8 = {0x00};
	uint8_t sendBuff_8 = { 0x00 };
	uint16_t sendBuff_16 = {0x0000};

	i2c_status_t returnValue1, returnValue2, returnValue3;

	i2c_device_t CY3116_slave = { .address = SLAVE_CY3116_ADDR, .baudRate_kbps =
			100 };
	printf("Set Configuration TEST \r\n");

	while (1) {

		cmdBuff[0] = 0x2a;	//prox_touch_th0
		returnValue3 = I2C_DRV_MasterReceiveData(
		BOARD_I2C_INSTANCE, &CY3116_slave, cmdBuff, 1, &(receiveBuff_16),
				sizeof(receiveBuff_16));
		if (returnValue3 != kStatus_I2C_Success) {
			printf("I2C communication failed, error code: %d\r\n",
					returnValue3);
			printf("prox touch th0 read Failure\r\n");
		}

		else
			printf("prox touch th0 Receive Success, receiveBuff = %x\r\n",
					receiveBuff_16);

		OSA_TimeDelay(100);

		cmdBuff[1] = 0x0c;	//base_threshold0
		returnValue3 = I2C_DRV_MasterReceiveData(
		BOARD_I2C_INSTANCE, &CY3116_slave, &(cmdBuff[1]), 1, &(receiveBuff_8),
				sizeof(receiveBuff_8));
		if (returnValue3 != kStatus_I2C_Success) {
			printf("I2C communication failed, error code: %d\r\n",
					returnValue3);
			printf("base_threshold0 read Failure\r\n");
		}

		else
			printf("base_threshold0 Receive Success, receiveBuff = %x\r\n",
					receiveBuff_8);

		OSA_TimeDelay(100);





		//cmdBuff[0] = 0x;	//buzzer_cfg
		sendBuff_16 = 65000;		//prox_touch_th0(16 bit) 62~65000

		returnValue2 = I2C_DRV_MasterSendData(
		BOARD_I2C_INSTANCE, &CY3116_slave, cmdBuff, 1, &sendBuff_16,
				sizeof(sendBuff_16));

		if (returnValue2 != kStatus_I2C_Success) {
			printf("I2C communication failed, error code: %d\r\n",
					returnValue2);
			printf("Buzzer Set Test Failure\r\n");
		} else
			printf("prox_touch Send Success .\r\n");

		OSA_TimeDelay(1);

		sendBuff_8 = 200;		//base_threshold(8 bit) 31~200

				returnValue3 = I2C_DRV_MasterSendData(
				BOARD_I2C_INSTANCE, &CY3116_slave, &(cmdBuff[1]), 1, &sendBuff_8,
						sizeof(sendBuff_8));

				if (returnValue3 != kStatus_I2C_Success) {
					printf("I2C communication failed, error code: %d\r\n",
							returnValue3);
					printf("Buzzer Set Test Failure\r\n");
				} else
					printf("base threshold Send Success 2.\r\n");


	}

}


void CY3116_Buzzer_Config_Test(void)
{
	//initialize
	uint8_t cmdBuff[1] = { 0xFF };
	uint8_t receiveBuff[1] = { 0x00 };
	uint8_t sendBuff[1] = {0x00};

	i2c_status_t returnValue1, returnValue2, returnValue3;

	i2c_device_t CY3116_slave = {
		.address = SLAVE_CY3116_ADDR,
		.baudRate_kbps =100
	};
	printf("Set Configuration TEST \r\n");

	while(1)
	{

		cmdBuff[0] = 0x3f;	//buzzer time
		sendBuff[0] = 0x77;	//valid value: 1~127 (0x01~0x7f)

		returnValue1 = I2C_DRV_MasterSendData(
			BOARD_I2C_INSTANCE,
			&CY3116_slave,
			cmdBuff,
			1,
			sendBuff,
			sizeof(sendBuff)
			);

		if(returnValue1 != kStatus_I2C_Success)
		{
			printf("I2C communication failed, error code: %d\r\n", returnValue1);
			printf("Buzzer Set Test Failure\r\n");
		}
		else
			printf("Send Success 1.\r\n");


		cmdBuff[0] = 0x3e;	//buzzer_cfg
		sendBuff[0] = 0x81;		//enable buzzer, 4KHz frequency

		returnValue2 = I2C_DRV_MasterSendData(
			BOARD_I2C_INSTANCE,
			&CY3116_slave,
			cmdBuff,
			1,
			sendBuff,
			sizeof(sendBuff)
			);

		if(returnValue2 != kStatus_I2C_Success)
		{
			printf("I2C communication failed, error code: %d\r\n", returnValue2);
			printf("Buzzer Set Test Failure\r\n");
		}
		else
			printf("Send Success 2.\r\n");


		cmdBuff[0] = 0x3e;
		returnValue3 = I2C_DRV_MasterReceiveData(
			BOARD_I2C_INSTANCE,
			&CY3116_slave,
			cmdBuff,
			1,
			receiveBuff,
			sizeof(receiveBuff)
			);
		if(returnValue3 != kStatus_I2C_Success)
		{
			printf("I2C communication failed, error code: %d\r\n", returnValue3);
			printf("Buzzer Set Test Failure\r\n");
		}

		else
			printf("Receive Success, receiveBuff = %x\r\n", receiveBuff[0]);

		OSA_TimeDelay(100);


	}









}













void Single_Tap_Operation(void) {
	printf("Single Tap Detected\r\n");
	printf("test end\r\n");
	while (1)
		;
}
void Double_Tap_Operation(void) {
	printf("Double Tap Detected\r\n");
	printf("test end\r\n");
	while (1)
		;
}
void Left_Slide_Operation(void) {
	printf("Left Slide Detected\r\n");
	printf("test end\r\n");
	while (1)
		;
}
void Right_Slide_Operation(void) {
	printf("Right Slide Detected\r\n");
	printf("test end\r\n");
	while (1)
		;
}
void Up_Slide_Operation(void) {
	printf("Up Slide Detected\r\n");
	printf("test end\r\n");
	while (1)
		;
}
void Down_Slide_Operation(void) {
	printf("Down Slide Detected\r\n");
	printf("test end\r\n");
	while (1)
		;
}
void Long_Press_Operation(void) {
	printf("Long Press Detected\r\n");
	printf("test end\r\n");
	while (1)
		;
}

//host interrupt triggered by any change of status
/*
 #define SLIDER1_POSITION_REGISTER 	0		//left and right
 #define LIFTOFF_SLIDER1_POSITION 	1
 #define SLIDER2_POSITION_REGISTER  	2		//up and down
 #define LIFTOFF_SLIDER2_POSITION    3
 #define BUTTON_STAT_REGISTER 		4
 */
#if 0
void CY_Gesture_Detection(void) {
	uint8_t i;
	uint8_t cmdBuff[5] = { 0xb0,	//slider1_position register :0
			0xb1, 	//liftoff_slider1_position register: 1
			0xb2,	//slider2_position register: 2
			0xb3,	//liftoff_slider2_position register: 3
			0xaa, 	//button_stat register: 4
			};
	uint8_t receiveBuff[5] = { 0x00 };

	i2c_status_t returnValue[10] = { 0 };

	i2c_device_t CY3116_slave = { .address = SLAVE_CY3116_ADDR, .baudRate_kbps =
			400 };
	printf("Gesture Recognition TEST \r\n");

	_reset: while (1) {
		//first read the register of slider1 and 2
		returnValue[0] = I2C_DRV_MasterReceiveData(
		BOARD_I2C_INSTANCE, &CY3116_slave,
				&(cmdBuff[SLIDER1_POSITION_REGISTER]), 1,
				&(receiveBuff[SLIDER1_POSITION_REGISTER]), sizeof(receiveBuff));
		OSA_TimeDelay(1);
		returnValue[1] = I2C_DRV_MasterReceiveData(
		BOARD_I2C_INSTANCE, &CY3116_slave,
				&(cmdBuff[SLIDER2_POSITION_REGISTER]), 1,
				&(receiveBuff[SLIDER2_POSITION_REGISTER]),
				sizeof(receiveBuff[SLIDER2_POSITION_REGISTER]));//don't know the sensitivity yet. may receive 0 if reacts not fast enough

		if ((returnValue[0] != kStatus_I2C_Success)
				|| (returnValue[1] != kStatus_I2C_Success)) {
			printf("read slider position failed\r\n");
			goto _reset;
		}

		OSA_TimeDelay(100);	//short delay, need adjustment. Should be the time of single tap

		//read the state register of PS
		_read_PS: returnValue[2] = I2C_DRV_MasterReceiveData(
		BOARD_I2C_INSTANCE, &CY3116_slave, &(cmdBuff[BUTTON_STAT_REGISTER]), 1,
				&(receiveBuff[BUTTON_STAT_REGISTER]),
				sizeof(receiveBuff[BUTTON_STAT_REGISTER]));

		if (returnValue[2] != kStatus_I2C_Success) {
			printf("read proximity sensor failed\r\n");
			goto _read_PS;
		}

		if (receiveBuff[BUTTON_STAT_REGISTER] & 0x01 == 1)//LSB = 1: proximity active
				{
			printf("Long Press or Slide\r\n");
			OSA_TimeDelay(300); // need adjustment. delay閻庣懓濂旂粻锝夊触鎼达紕瀹夐悹鍥ュ劜閻︻噣宕氶幏灞惧涧濞戞搫鎷烽鐓庮煥閹存繂袟闁归潧顑呮繛宥夋儍閸曨剚顦ч梻鍌氼嚟閸嬶絿绮欏鍡樼彄濞戞搫鎷烽崑锟�			do {
				returnValue[3] = I2C_DRV_MasterReceiveData(
				BOARD_I2C_INSTANCE, &CY3116_slave,
						&(cmdBuff[BUTTON_STAT_REGISTER]), 1,
						&(receiveBuff[BUTTON_STAT_REGISTER]),
						sizeof(receiveBuff[BUTTON_STAT_REGISTER]));
			} while (returnValue[3] != kStatus_I2C_Success);

			printf("Read PS Success\r\n");

			if (receiveBuff[BUTTON_STAT_REGISTER] & 0x01 == 1)//LSB = 1: proximity active
					{
				printf("Long press detected\r\n");
				Long_Press_Operation();
			}

			else {
				printf("Slide Gesture\n\r");
				do {
					returnValue[4] = I2C_DRV_MasterReceiveData(
					BOARD_I2C_INSTANCE, &CY3116_slave,
							&(cmdBuff[LIFTOFF_SLIDER1_POSITION]), 1,
							&(receiveBuff[LIFTOFF_SLIDER1_POSITION]),
							sizeof(receiveBuff[LIFTOFF_SLIDER1_POSITION]));

					returnValue[5] = I2C_DRV_MasterReceiveData(
					BOARD_I2C_INSTANCE, &CY3116_slave,
							&(cmdBuff[LIFTOFF_SLIDER2_POSITION]), 1,
							&(receiveBuff[LIFTOFF_SLIDER2_POSITION]),
							sizeof(receiveBuff[LIFTOFF_SLIDER2_POSITION]));

				} while ((returnValue[4] != kStatus_I2C_Success)
						|| (returnValue[5] != kStatus_I2C_Success));

				printf("Get slider position success\r\n");

				//assume: 濞戞搫鎷�鐎归潻鎷�~127); 濞戞搫鎷�闁告瑱鎷�28~254); 255: 闁哄牜浜濆畷鐔兼嚔瀹勬澘鐓傞悷娆欓檮閹筹拷
				if (receiveBuff[LIFTOFF_SLIDER1_POSITION] == 255)//閻犲洤鐡ㄥΣ鎴﹀及椤栨瑧鐟愬☉鎾愁儐缁箓宕濋敓锟�					{
					if ((receiveBuff[LIFTOFF_SLIDER2_POSITION] < 128) //闁哄牞鎷烽幃妤冪矉鐠囪尙纾婚柣銊ュ缂嶅懐绱旈鑺バ﹀☉鎾筹躬閸庯拷
							&& (receiveBuff[SLIDER2_POSITION_REGISTER] >= 128))	//鐎殿噯鎷烽～鎰板箳閵夈剱鏇㈡儍閸曨亞绉寸紓鍐惧枟濡插憡绋夌�顔煎姤
							{
								Up_Slide_Operation();
								break;
							}


					if ((receiveBuff[LIFTOFF_SLIDER2_POSITION] >= 128) //闁哄牞鎷烽幃妤冪矉鐠囪尙纾婚柣銊ュ缂嶅懐绱旈鑺バ﹀☉鎾愁儔閸庯拷
							&& (receiveBuff[SLIDER2_POSITION_REGISTER] < 128))//鐎殿噯鎷烽～鎰板箳閵夈剱鏇㈡儍閸曨亞绉寸紓鍐惧枟濡插憡绋夋繝鍥у姤
							{
								Down_Slide_Operation();
								break;
							}


					else
						printf(
								"Lift off from up down slider. But Invalid Gesture. Detection failed.\r\n");
					while (1)
						;
				}

				if (receiveBuff[LIFTOFF_SLIDER2_POSITION] == 255)//閻犲洤鐡ㄥΣ鎴﹀及椤栨矮绠柛娆掓珪缁箓宕濋敓锟�					{
					if ((receiveBuff[LIFTOFF_SLIDER1_POSITION] < 128) //闁哄牞鎷烽幃妤冪矉鐠囪尙纾婚柣銊ュ缂嶅懐绱旈鑺バ︾�闈╃畵閸庯拷
							&& (receiveBuff[SLIDER1_POSITION_REGISTER] >= 128))	//鐎殿噯鎷烽～鎰板箳閵夈剱鏇㈡儍閸曨亞绉寸紓鍐惧枟濡叉悂宕ｉ幎钘夊姤
							{
								Left_Slide_Operation();
								break;
							}


					if ((receiveBuff[LIFTOFF_SLIDER1_POSITION] >= 128) //闁哄牞鎷烽幃妤冪矉鐠囪尙纾婚柣銊ュ缂嶅懐绱旈鑺バ﹂柛娆愬▕閸庯拷
							&& (receiveBuff[SLIDER1_POSITION_REGISTER] < 128))//鐎殿噯鎷烽～鎰板箳閵夈剱鏇㈡儍閸曨亞绉寸紓鍐惧枟濡茬顔忛敃鍌氬姤
							{
								Right_Slide_Operation();
							}


					else
						printf(
								"Lift off from left right slider. But Invalid Gesture. Detection failed.\r\n");
					while (1)
						;
				}

				else {
					printf("Reading Slider status incorrect. \r\n");
					while (1)
						;
				}
			}

		}	//end of long press / slide detection

		else // proximity = 0: single tap or double tap
		{
			printf("Single Tap or Double Tap\r\n");
			receiveBuff[BUTTON_STAT_REGISTER] = 0x00;	//reset
			for (i = 0; i < 4; i++) {
				returnValue[6] = I2C_DRV_MasterReceiveData(
				BOARD_I2C_INSTANCE, &CY3116_slave,
						&(cmdBuff[BUTTON_STAT_REGISTER]), 1,
						&(receiveBuff[BUTTON_STAT_REGISTER]),
						sizeof(receiveBuff[BUTTON_STAT_REGISTER]));

				if (receiveBuff[BUTTON_STAT_REGISTER] & 0x01 == 1)//another tap detected
				{
					Double_Tap_Operation();
				}

			}

			//no other tap detected
			Single_Tap_Operation();

		}

	}

}

#endif
