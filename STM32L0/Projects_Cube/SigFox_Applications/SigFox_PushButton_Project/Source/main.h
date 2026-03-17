#ifndef _MAIN_H
#define _MAIN_H

/**
* @file    main.c
* @author  AMG & HEA & RF BU
* @version V1.4.0
* @date    May, 2018
* @brief   Command line DEMO program
* @details
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* THIS SOURCE CODE IS PROTECTED BY A LICENSE.
* FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
* IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.
*
* <h2><center>&copy; COPYRIGHT 2018 STMicroelectronics</center></h2>
*/

/* Includes ------------------------------------------------------------------*/
#include "assert.h"

/* Switch between FKIXXX-STM32 solutions and BlueNRG based sol. */
#include "hw_config.h"
#include "stdio.h"
#include "stdlib.h"
/* Sigfox includes */
enum{S_IDLE = 0, S_BOOT, S_SETUP, S_TRACKING, S_DEEPSLEEP, S_TEST, S_WAKEUP, S_GPS_TEST, S_TEST_MODE};
enum{W_RTC = 0, W_ACCEL, W_RS, W_NOMOVE, W_RTC_A, W_RTC_B, W_NONE};
	
/* Fucntions Prototypes-------------------------------------------------------*/
void Appli_Exti_CB(uint16_t GPIO_Pin);
extern void alarm_b_set(void);
extern void alarm_b_set_off(void);
extern void stop_mode_enter(_Bool use_rtc);
extern void stop_mode_config(void);
uint32_t get_bat(void);
void get_bat_en(_Bool en);
uint32_t get_bat_adc(void);
float get_temp(void);

#define VDD							2800
#define BATADC_OFFSET		50

#define UBIGNSS_ENABLE			1
#define UBIGNSS_DISABLE			0

#define INT_RISING_EDGE			1  
#define INT_FALLING_EDGE		0

#define RS_INT_Pin GPIO_PIN_0
#define RS_INT_GPIO_Port GPIOA
#define RS_INT_EXTI_IRQn EXTI0_1_IRQn
#define S2LP_SDN_Pin GPIO_PIN_2
#define S2LP_SDN_GPIO_Port GPIOA
#define GPIO_RES4_Pin GPIO_PIN_3
#define GPIO_RES4_GPIO_Port GPIOA
#define S2LP_CSN_Pin GPIO_PIN_4
#define S2LP_CSN_GPIO_Port GPIOA
#define TCXO_EN_Pin GPIO_PIN_0
#define TCXO_EN_GPIO_Port GPIOB
#define BAT_ADC_Pin GPIO_PIN_1
#define BAT_ADC_GPIO_Port GPIOB
#define S2LP_GPIO0_Pin GPIO_PIN_2
#define S2LP_GPIO0_GPIO_Port GPIOB
#define S2LP_GPIO1_Pin GPIO_PIN_8
#define S2LP_GPIO1_GPIO_Port GPIOA
#define GPS_EN_Pin GPIO_PIN_15
#define GPS_EN_GPIO_Port GPIOA
#define PA_CSD_Pin GPIO_PIN_3
#define PA_CSD_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_4
#define LED_GPIO_Port GPIOB
#define ACCEL_INT1_Pin GPIO_PIN_5
#define ACCEL_INT1_GPIO_Port GPIOB
#define ACCEL_INT2_Pin GPIO_PIN_8
#define ACCEL_INT2_GPIO_Port GPIOB

#endif


