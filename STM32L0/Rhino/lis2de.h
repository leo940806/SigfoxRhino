#ifndef __LIS2DE_H
#define __LIS2DE_H

#ifdef __cplusplus
 extern "C" {
#endif


#include "main.h"


extern I2C_HandleTypeDef hi2c1;

/*-------------------------------------------------*/
#define accel_i2c				hi2c1
#define accel_dev 			0x30
#define accel_num_config	19
#define ACCEL_VER				0x33
/*-------------------------------------------------*/
#define STATUS_REG_AUX		0x07
#define	OUT_TEMP_L				0x0C
#define	OUT_TEMP_H				0x0D
#define	WHO_AM_I		  		0x0F
#define	CTRL_REG0					0x1E
#define	TEMP_CFG_REG			0x1F
#define	CTRL_REG1					0x20
#define	CTRL_REG2					0x21
#define	CTRL_REG3					0x22
#define	CTRL_REG4					0x23
#define	CTRL_REG5					0x24
#define	CTRL_REG6					0x25
#define	REFERENCE					0x26
#define	STATUS_REG				0x27
#define	FIFO_READ_START		0x28
#define	OUT_X_H						0x29
#define	OUT_Y_H						0x2B
#define	OUT_Z_H						0x2D
#define	FIFO_CTRL_REG			0x2E
#define	FIFO_SRC_REG			0x2F
#define	INT1_CFG					0x30
#define	INT1_SRC					0x31
#define	INT1_THS					0x32
#define	INT1_DURATION			0x33
#define	INT2_CFG					0x34
#define	INT2_SRC					0x35
#define	INT2_THS					0x36
#define	INT2_DURATION			0x37
#define	CLICK_CFG					0x38
#define	CLICK_SRC					0x39
#define	CLICK_THS					0x3A
#define	TIME_LIMIT				0x3B
#define	TIME_LATENCY			0x3C
#define	TIME_WINDOW				0x3D
#define	ACT_THS						0x3E
#define	ACT_DUR						0x3F
/*-------------------------------------------------*/
#define ZYXDA_mask				0x08		//--------STATUS_REG Check if new xyz data is available
#define TDA_mask					0x04		//--------STATUS_REG_AUS check if new temperature data is available
/*-------------------------------------------------*/
#define ACT_INT_DUR				5       // 5 mins
#define ODR_1HZ						0x1F
#define ODR_10Hz					0x2F
#define ACCEL_TIMER_MAX_MINS	34
 	

void accel_i2c_rx(uint8_t *addr, uint8_t *data);
void accel_i2c_tx(uint8_t *addr, uint8_t *data);
void accel_get_count(void);
void accel_get_xyz(void);
float accel_get_temp(void);
void accel_init(void);
void accel_sleep(void);
void accel_reboot(void);
void accel_reset_act_int2(void);
void accel_set_sensitivity(uint8_t sensitivity);
void accel_set_act_duration(uint8_t duration);
_Bool accel_test(void);
#endif
