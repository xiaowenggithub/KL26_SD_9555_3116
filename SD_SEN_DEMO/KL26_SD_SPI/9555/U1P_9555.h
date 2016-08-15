#ifndef U1P_9555_H
#define U1P_9555_H

/*@TODO*/
/*include essential libraries for the header*/
#include "board.h"
//#include "main.h"
#include "stdint-gcc.h"
//#include "fsl_i2c_master_driver.c"

#include "fsl_i2c_master_driver.h"

/* pedometer application configure register */
#define DEFAULT_SLEEPMIN_MSB 	0x0C
#define DEFAULT_SLEEPMIN_LSB 	0xE0		// sleepmin----3296
#define DEFAULT_SLEEPMAX_MSB 	0x13
#define DEFAULT_SLEEPMAX_LSB 	0x20		// sleepmax----4896
#define DEFAULT_SLEEPTHD_MSB 	0x00
#define DEFAULT_SLEEPTHD_LSB 	0x5A		// sleep threshold----90
#define DEFAULT_CONFIG_REG	 	0xE0		// Configuration register----224	open the pedometer application
#define DEFAULT_STEPLEN			0x64		// 100		Set to 0 to automatically estimate
#define DEFAULT_HEIGHT			0xB4		// 180
#define DEFAULT_WEIGHT			0x4E		// 78
#define DEFAULT_FILTSTEP		0x01
#define DEFAULT_SEX_FILTTIME	0x83		// male and filter time
#define DEFAULT_SPEEDPRD		0x05		// 5 seconds in which to compute speed
#define DEFAULT_STEPTHRESHOLD	0x82		// Large values make the pedometer less sensitive
#define DEFAULT_ACTCNT_MSB		0x00
#define DEFAULT_ACTCNT_LSB		0x00
#define DEFAULT_STEPCOALESCE	0x01

/* pedometer application status register offset */
#define STPCNT_OFFSET			0x02
#define DISTANCE_OFFSET			0x04
#define SPEED_OFFSET			0x06
#define CALORIES_OFFSET			0x08
#define SLEEPCNT_OFFSET			0x0A

/*-----------------------mode enable/disable---------------------*/
#define SLEEP_ENABLE			0x01
#define SLEEP_DISABLE			0x00

/*		mode_suspend 		*/
#define PEDO_ENABLE				0x00
#define PEDO_DISABLE			0x20

/*----------------------user's data type--------------------------*/
/*			extern uint8_t U1P_9555_set_user_data(uint8_t data_type, uint8_t data)			*/
#define TYPE_STEPLEN			0x07
#define TYPE_HEIGHT				0x08
#define TYPE_WEIGHT				0x09

/*-----------------------AFE mode choose------------------------*/
/*			U1P_9555_AFE_config()			*/
#define GMODE_2					0x40
#define GMODE_4					0x80
#define GMODE_8					0x00

/*----------------------AFE status XYZ registers----------------------*/
/* 			uint8_t U1P_9555_read_XYZ(uint8_t afe_status)			*/
#define AFE_STAGE_0				0x00
#define AFE_STAGE_1				0x06
#define AFE_ABS_DATA			0x0C
#define AFE_GMODE				0x12
#define AFE_LPF					0x18
#define AFE_HPF					0x1E

/*--------------------interrupt configure application and flag on bit-----------------*/

/*		U1P_9555_int_config(uint8_t int_app, uint8_t act_flag)		*/
/*	the application number	*/
#define PEDO_APP				0x15
#define DIR_APP					0x1A
/*	activity flag number	*/
#define ACTCHG					0x04		// for pedo_app
#define STEPCHG					0x05		// for pedo_app
#define SUSPCHG					0x06		// for pedo_app
#define MRGFL					0x07		// for pedo_app
#define SIXDIR_FLG				0x07		// for direction application
/*	gpio mask number	*/
#define GPIO6_MASK				0x6F
#define GPIO7_MASK				0x5F
#define GPIO8_MASK				0x3F
/* GPIO OFFSET */
#define GPIO6_OFFSET				0x00
#define GPIO7_OFFSET				0x02
#define GPIO8_OFFSET				0x04
#define GPIO9_OFFSET				0x06

/*-----------------Wake up the MMA9555---------------------*/
/* wake up from deep sleep */
/* 0x00---Disables the Sleep mode	0x01----Enables the Sleep mode */
extern uint8_t U1P_9555_wakeup(uint8_t sleep_mode);

/*-----------------Read functions---------------------*/

/* get the user's config data on pedometer application */
extern uint8_t U1P_9555_read_config(void);

/* get the six direction status registers values */
/*
 * return value description
 * Direction Status change flag.[7]
0: Direction has not changed
1: Direction changed. The bit is cleared when the register is read.
 * [6-3]----reserved		[2]----0(negative direction) 1(positive direction)
 * [1-0]----00: X axis	01: Y axis	10: Z axis
 */
extern uint8_t U1P_9555_read_sixdir(void);


/* --------------get the 9555's pedometer application's status register value--------------------- */

/* get current activity level */
/* return current activity level value
 * 1-----rest		2-------walking			3-------jogging			4-----running		9------rest with merged change
 */
extern uint8_t U1P_9555_read_activity(void);

/* get current height configuration */
/* return current configured height value */
extern uint8_t U1P_9555_read_height(void);

/* get current weight configuration */
/* return current configured weight value */
extern uint8_t U1P_9555_read_weight(void);


/* get 2 bytes pedometer status register value */
/* return request register value */
/*
 * pedo_data = 0x02 return pedometer status register's Step count register
 * pedo_data = 0x04 return pedometer status register's Distance register
 * pedo_data = 0x06 return pedometer status register's Speed register
 * pedo_data = 0x08 return pedometer status register's Calories register
 * pedo_data = 0x0A return pedometer status register's Sleep Count register
 */
extern uint16_t U1P_9555_pedo_status(uint8_t pedo_data);

/*-----------------Set functions---------------------*/

/* set the user's data */
/*
 * data_type = TYPE_HEIGHT		set the user's height
 * data_type = TYPE_WEIGHT		set the user's weight
 * data_type = TYPE_STEPLEN		set the user's steplen
 */
extern uint8_t U1P_9555_set_user_data(uint8_t data_type, uint8_t data);

/* set sensitivity threshold */
extern uint8_t U1P_9555_set_sensitivity(uint8_t sensitivity);
/* configure the user's parameters for app */
extern uint8_t U1P_9555_config(uint8_t user_data[]);


/*-------------------Analog Front End Application--------------------*/
/* select different mode to configure AFE application */
extern uint8_t U1P_9555_AFE_config(uint8_t g_mode);


/*---------AFE application data-------------*/
/*
 * 																														Range of valid values
 * AFE_STAGE_0	Normalized accelerometer data sampled at the default rate with complete bandwidth. 					 	每32,768 to 32,767.
 * AFE_STAGE_1	Normalized accelerometer data sampled at the default rate of half bandwidth.							每32,768 to 32,767.
 * AFE_ABS_DATA Absolute value normalized accelerometer data sampled at the default rate of complete bandwidth.				  0 to 32,767.
 * AFE_GMODE	Raw accelerometer data sampled at the default rate with complete bandwidth.
 * 				㊣2 g mode: 0.061 mg/LSB = 每32768 to 32767	㊣4 g mode: 0.122 mg/LSB = 每8192 to 8192		㊣8 g mode: 0.244 mg/LSB = 每4096 to 4096
 *
 * AFE_LPF		Normalized accelerometer data sampled at the default rate, with configurable,low-pass filter cutoff.	每32,768 to 32,767.
 * AFE_HPF		Normalized accelerometer data sampled at the default rate, with configurable,high-pass filter cutoff.	每32,768 to 32,767.
 *
 */
extern uint8_t U1P_9555_read_xyz(uint8_t afe_status);

/*
extern uint8_t U1P_9555_open_temp(void);
extern uint8_t U1P_9555_read_temp(void);
*/
/*-----------------Reset/Suspend the pedometer application-------------*/

/* This causes the scheduler to invoke the pedometer's reset function */
extern uint8_t U1P_9555_pedometer_reset(void);

/* Enable/disable the Pedometer application */
/* if disable, the scheduler to suspend the Pedometer application, even if the device is not stationary */
extern uint8_t U1P_9555_pedometer_suspend(uint8_t mode_suspend);



/*-----------------OUTPUT INTERRUPT CONFIGURATION---------------------*/

/* 	Configure output interrupt: Device direction change on GPIO6 	*/
extern uint8_t U1P_9555_int_config(uint8_t int_app, uint8_t act_flag, uint8_t GPIO_OFFSET);

/*	before U1P_9555_gpio_set YOU MUST read the return value	*/
extern uint8_t U1P_9555_gpio_read(void);

/*	GPIO input/output enable register	*/
/* mask will be U1P_9555_gpio_read() return value	*/
extern uint8_t U1P_9555_gpio_set(uint8_t mask, uint8_t gpio_mask);


/*-----------------otrher function-------------------*/
extern uint8_t U1P_9555_int_pin_config(void);

extern uint8_t pedometer_int0_enable(void);



/*-----------------Test App---------------------*/
/* return value is this 9555's version number */
extern uint8_t U1P_9555_version(void);
#endif


