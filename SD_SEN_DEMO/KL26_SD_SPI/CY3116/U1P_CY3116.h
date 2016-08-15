#ifndef U1P_CY3116_H
#define U1P_CY3116_H

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "stdbool.h"
//#include "U1P_9555.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_i2c_master_driver.h"

#define SLIDER1_POSITION_REGISTER 	0		//left and right
#define LIFTOFF_SLIDER1_POSITION 	1
#define SLIDER2_POSITION_REGISTER  	2		//up and down
#define LIFTOFF_SLIDER2_POSITION    3
#define BUTTON_STAT_REGISTER 		4

#define SLAVE_CY3116_ADDR (0x37)


void CY3116_Version_Test(void);

void CY3116_Touch_Test(void);

void CY3116_Gesture_Reco(void);	//upward gesture only 

void CY_Gesture_Detection(void);

void Single_Tap_Operation(void);

void Double_Tap_Operation(void);

void Left_Slide_Operation(void);

void Right_Slide_Operation(void);

void Up_Slide_Operation(void);

void Down_Slide_Operation(void);

void Long_Press_Operation(void);

void Test_Proximity(void);

#endif
