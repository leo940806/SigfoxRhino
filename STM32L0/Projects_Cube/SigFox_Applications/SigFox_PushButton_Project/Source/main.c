/**
* @file main.c
* @author  AMG - RF Application team
* @version 1.4.0
* @date    March 23, 2018
* @brief This is a ST-SigFox demo that shows how to use the sigfox protocol to 
*         send a message to the base stations each time the push button is pressed.
*         The data sent is a number representing the number of times the button 
*					has been pressed from the boot.        
* @details
* This application sets the MCU in low power. in order to debug it using a SWD debugger it is necessary to:
* - call the following function in the initialization phase:
* \code
* ST_MCU_API_LowPower(0);
* \endcode
* - comment out the following lines.
* \code 
* enterGpioLowPower();
* HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON,PWR_STOPENTRY_WFI);
* ST_MCU_API_SetSysClock();
* exitGpioLowPower();
* \endcode
*
* @note       If the application is booted with the push button pressed, it will use the public key for transmission.    
* \section KEIL_project KEIL project
To use the project with KEIL uVision 5 for ARM, please follow the instructions below:
-# Open the KEIL uVision 5 for ARM and select Project->Open Project menu. 
-# Open the KEIL project
<tt> ...\\Projects\\Projects_Cube\\SigFox_Applications\\SigFox_PushButton_Project\\MDK-ARM\\SigFox_PushButton_Project.uvprojx </tt>
-# Select desired configuration to build
-# Select Project->Rebuild all target files. This will recompile and link the entire application
-# Select Project->Download to download the related binary image.

* \section IAR_project IAR project
To use the project with IAR Embedded Workbench for ARM, please follow the instructions below:
-# Open the Embedded Workbench for ARM and select File->Open->Workspace menu. 
-# Open the IAR project
<tt> ...\\Projects\\Projects_Cube\\SigFox_Applications\\SigFox_PushButton_Project\\EWARM\\SigFox_PushButton_Project.eww </tt>
-# Select desired configuration to build
-# Select Project->Rebuild All. This will recompile and link the entire application
-# Select Project->Download and Debug to download the related binary image.

* \subsection Project_configurations Project configurations
- \c NUCLEO_L1_ALL - Configuration to be used for all RCZ on the NUCLEO-L152RE
- \c NUCLEO_L1_ETSI - Configuration to be used for RCZ1 on the NUCLEO-L152RE
- \c NUCLEO_L1_FCC - Configuration to be used for RCZ2 and RCZ4 on the NUCLEO-L152RE
- \c NUCLEO_L1_ARIB - Configuration to be used for RCZ3 on the NUCLEO-L152RE
- \c NUCLEO_L0_ALL - Configuration to be used for all RCZ on the NUCLEO-L053R8
- \c NUCLEO_L0_ETSI - Configuration to be used for RCZ1 on the NUCLEO-L053R8
- \c NUCLEO_L0_FCC - Configuration to be used for RCZ2 and RCZ4 on the NUCLEO-L053R8
- \c NUCLEO_L0_ARIB - Configuration to be used for RCZ3 on the NUCLEO-L053R8
- \c NUCLEO_F0_ALL - Configuration to be used for all RCZ on the NUCLEO-F072RB
- \c NUCLEO_F0_ETSI - Configuration to be used for RCZ1 on the NUCLEO-F072RB
- \c NUCLEO_F0_FCC - Configuration to be used for RCZ2 and RCZ4 on the NUCLEO-F072RB
- \c NUCLEO_F0_ARIB - Configuration to be used for RCZ1 on the NUCLEO-F072RB
- \c NUCLEO_F4_ETSI - Configuration to be used for RCZ1 on the NUCLEO-F401RE
- \c NUCLEO_F4_FCC - Configuration to be used for RCZ2 and RCZ4 on the NUCLEO-F401RE
- \c NUCLEO_F4_ARIB - Configuration to be used for RCZ3 on the NUCLEO-F401RE
- \c NUCLEO_F4_ALL - Configuration to be used for all RCZ on the NUCLEO-F401RE
* \section Board_supported Boards supported
- \c STEVAL-FKI868V2/1 (for RCZ1 and RCZ3)
- \c STEVAL-FKI915V2/1 (for RCZ2 and RCZ4)
* 
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
#ifdef STM32L053xx

#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "sigfox_types.h"
#include "sigfox_api.h"
#include "sigfox_retriever.h"
#include "mcu_api.h"
#include "st_mcu_api.h"
#include "st_rf_api.h"
#include "nvm_api.h"
/* My Code ------------------------------------------------------------------*/
#include "libubignsscore.h"
#include "ubignss.h"
#include "mcu_api.h"
#include "lis2de.h"
#include "gps.h"
#include "linkdata.h"
#include "stdbool.h"
#include "stm32l0xx_hal_i2c.h"
#include "stm32l0xx_hal_wwdg.h"
#include "stdlib.h"
#include "time.h"



#define PRODUCT_VERSION	0x01

#define DEFAULT_HOUR		0x00
#define DEFAULT_MIN			0x00
#define DEFAULT_SEC			0x00

#define RealMinutes     (float)2
#define rtc_off         (float)1    
#define RTCMinutes      (int)RealMinutes*rtc_off
#define rtc_hour        (uint8_t)(RTCMinutes/60)
#define rtc_minutes     (uint8_t)(RTCMinutes-rtc_hour*60)


#define SFX_REDUCED_POWER		-34
#define S2LP_SMPS_OUT				7
#define S2LP_SMPS_1V8				7
#define S2LP_SMPS_1V7				6
#define S2LP_SMPS_1V6				5
#define S2LP_SMPS_1V5				4
#define S2LP_SMPS_1V4				3
#define S2LP_SMPS_1V3				2
#define S2LP_SMPS_1V2				1
#define S2LP_SMPS_OFF				0


#define ON 							true
#define OFF 						false
#define TOG							2
#define MOVE_TIMEOUT		1000

#define TIME_ERROR_BUFFER	10

// internal EEPROM Assigned Address
#define EEPROM_START_ADDRESS										0x08080000    
#define PRODUCT_VERSION_ADDRESS									0x08080000
#define TIME_REF_ADDRESS												0x08080002
#define GPS_MODE_ADDRESS												0x08080004  
#define GPS_PWR_MODE_ADDRESS										0x08080006  
#define ACCEL_MODE_ADDRESS											0x08080008  
#define UL_FREQ_ADDRESS													0x08080012 
#define DL_THS_ADDRESS													0x08080014 
#define IDLE_MODE_ADDRESS												0x08080016 
#define GPS_TIMEOUT_ADDRESS											0x08080018
#define ACCEL_SENSI_ADDRESS											0x08080020
#define NOMOVE_TIME_ADDRESS											0x08080022
#define HOUR_OF_MSG_ADDRESS											0x08080024
#define SCHEDULED_HOUR_ADDRESS									0x08080026
#define NOMOVE_FORCET_LED_ADDRESS								0x08080028



#define	NOMOVE_THS_ADDRESS											0x08080030
#define	NOMOVE_COUNTER_ADDRESS									0x08080032
#define	MOVE_THS_ADDRESS												0x08080034
#define	MOVE_COUNTER_ADDRESS										0x08080036
#define	CW_POWER_ADDRESS												0x08080038		
#define UL_COUNTER_ADDRESS											0x08080040
#define RC_SEL_ADDRESS													0x08080042
#define TEST_MODE_ADDRESS												0x08080044
#define SLEEP_FLAG_ADDRESS											0x08080046
#define RST_STATE_ADDRESS												0x08080048
#define TIME_INIT_ADDRESS												0x08080050



gps_time_s ubi_time;
uint8_t ubi_payload[UBI_PAYLOAD_LEN];
uint8_t ubi_state;


uint8_t old_dl_payload[8];

uint8_t pac[8];
uint32_t id;
uint8_t rcz = 1, retr_err, use_public_key=0;
uint8_t customer_resp[8];
uint8_t random_x, random_y;

_Bool gps_fixed = 0, rs_trig = 0, move_flag = 0, lastmove_flag = 0, nomove_ths = 0, accel_int2_next_edge = 0x00, accel_init_required = 1, timer_calibrated = 0, boot_flag = 0;
sfx_u8 s_error = 0x00;

trackerparameter_t tracker_data;
sensordata_t	sensor_data;

uint8_t s_state = S_BOOT, lp_mode = S_DEEPSLEEP, last_state = 0x00, rst_state = 0x00, accel_int2 = 0x00;

uint32_t starttime = 0, nowtime = 0, rs_time = 0;


uint8_t dl_ths= 0x00, gps_timeout = 0x00, test_mode = 0x00, ul_counter = 0x00, rc_sel = 0x00, cw_power = 0x00,	sleep_flag = 0x00, 
				move_counter = 0x00, move_ths = 0x00, nomove_sub = 0x00, product_version = PRODUCT_VERSION;
				
				
uint32_t ul_time = 0x0000, accel_time = 0x0000;

uint8_t wake_src = W_NONE, rtc_int = 0x00;

uint32_t ubi_count = 0;

uint32_t bat_idle = 0;

_Bool mid_night;
_Bool ubi_init_required = 1;
	
time_t mytimestampA = 0, mytimestampB = 0;
struct tm mytime ;
struct tm *mytime_temp;

//-------------------------------------------------------//
extern uint8_t sendbuf[max_frame];
extern uint32_t lSystickCounter;
extern uint8_t* pnumSV;
extern uint8_t* pstatus, *plong, *plat, *pNS, *pEW, *ptime;
extern uint8_t GPS_RST_HW_RST[];     
extern uint8_t GPS_RST_GNSS_STOP[]; 
extern uint8_t GPS_RST_GNSS_START[]; 
extern uint8_t GPS_RST_COOLSTART[]; 
extern uint8_t GPS_RST_WARMSTART[]; 
extern uint8_t GPS_RST_HOTSTART[]; 
extern uint8_t accel_init_reg[];
extern uint8_t bat_count;
extern uint32_t bat_tx_raw;

RTC_HandleTypeDef hrtc;
I2C_HandleTypeDef hi2c1;
UART_HandleTypeDef huart1, huart2;
RTC_AlarmTypeDef sAlarmA, sAlarmB;
ADC_HandleTypeDef hadc;
TIM_HandleTypeDef htim6;
WWDG_HandleTypeDef hwwdg;

void _Error_Handler(char *file, int line);


void mcu_system_init(void);
static void MX_USART1_UART_Init(void);
void HAL_UART_MspInit(UART_HandleTypeDef* huart);
static void MX_I2C1_Init(void);
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c);
void HAL_Delay(__IO uint32_t Delay);
void sigfox_library_setup(void);	
void Fatal_Error(void);
static void MX_RTC_Init(_Bool reset);
void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc);
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc);
void HAL_RTCEx_AlarmBEventCallback(RTC_HandleTypeDef *hrtc);
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc);
void HAL_IncTick(void);
uint32_t HAL_GetTick(void);
void standby(_Bool rtc_use);
void standby_init(void);
void delay_second(uint32_t sec);
void Configure_RTC_Clock(void);
void sigfox_geo_bat_ul(void);
sfx_u8 sigfox_boot_dl(void);
//void sigfox_setup_ul(void);
sfx_u8 sigfox_setup_uldl(_Bool downlink_required);
//sfx_u8 sigfox_gps_ul(void);
//sfx_u8 sigfox_gnss_uldl(_Bool downlink_required, _Bool use_ubiscale);
sfx_u8 sigfox_gps_uldl(_Bool downlink_required);
sfx_u8 sigfox_ubi_uldl(_Bool downlink_required);
void sigfox_debug_ul(void);
void LedBlink(uint8_t times);
static void MX_GPIO_Init(void);
void SystemClock_Config(void);
void rf_en(_Bool en);
void ledblink(uint8_t times);
float get_temp(void);
void get_bat_en(_Bool en);
uint32_t get_bat_adc(void);
uint32_t get_bat(void);
static void MX_ADC_Init(void);
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc);
void eeprom_save_para(void);
_Bool sigfox_testmode_dl(void);
uint8_t sigfox_rf_test(uint8_t tmode);
_Bool move_detect(void);
void eeprom_load_para(trackerparameter_t* data);
void led_en(uint8_t on, _Bool en);
void stop_mode_config(void);
void stop_mode_enter(_Bool use_rtc);
void mcu_accel_int2_setting(_Bool edge);
void alarm_a_set(uint32_t sec);
void alarm_b_set(void);
void alarm_b_set_off(void);
void alarm_a_midnight_set(void);
void mcu_write_time(_Bool time_init_flag);
_Bool mcu_read_time(void);
void set_time(struct tm *time);
uint32_t ul_freq_to_sec(uint8_t t_r, uint8_t ul_f);
void accel_dur_set(void);
time_t get_now_timestamp(void);
time_t time_different(time_t time1, time_t time2);
void debug_set_midnight(void);
void sigfox_set_rf_power(uint8_t rf_pwr);

void sigfox_addon_test_logic(void);

static void MX_TIM6_Init(void);
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base);

void rtc_time_reset(RTC_TimeTypeDef *ptime);
void rtc_date_reset(RTC_DateTypeDef *pdate);	


void delay_seconds(uint32_t sec)
{
	for(int s = 0; s < sec; s++)
	{
		#ifdef INCLUDE_WWDG
		HAL_WWDG_Refresh(&hwwdg);
		#endif

		SdkDelayMs(1000);
	}
}

//#define INCLUDE_DEBUG_UART
#ifdef INCLUDE_DEBUG_UART
UART_HandleTypeDef huart2;
struct __FILE
{
  int handle;
  /* Whatever you require here. If the only file you are using is */
  /* standard output using printf() for debugging, no file handling */
  /* is required. */
};
/* FILE is typedef’d in stdio.h. */
FILE __stdout;

int fputc(int ch, FILE *f)
{
//	MX_USART2_UART_Init();
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}
int ferror(FILE *f)
{
  /* Your implementation of ferror(). */
  return 0;
}

static void MX_USART2_UART_Init(void);
/* --------------------------------------------------------------------------*/
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_SWAP_INIT;
  huart2.AdvancedInit.Swap = UART_ADVFEATURE_SWAP_ENABLE;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

#endif

#ifdef INCLUDE_WWDG
static void MX_WWDG_Init(void);
void HAL_WWDG_MspInit(WWDG_HandleTypeDef* hwwdg);

void HAL_WWDG_MspInit(WWDG_HandleTypeDef* hwwdg)
{
  if(hwwdg->Instance==WWDG)
  {
  /* USER CODE BEGIN WWDG_MspInit 0 */

  /* USER CODE END WWDG_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_WWDG_CLK_ENABLE();
    /* WWDG interrupt Init */
    HAL_NVIC_SetPriority(WWDG_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(WWDG_IRQn);
  /* USER CODE BEGIN WWDG_MspInit 1 */

  /* USER CODE END WWDG_MspInit 1 */
  }
}
static void MX_WWDG_Init(void)
{

  /* USER CODE BEGIN WWDG_Init 0 */

  /* USER CODE END WWDG_Init 0 */

  /* USER CODE BEGIN WWDG_Init 1 */

  /* USER CODE END WWDG_Init 1 */
  hwwdg.Instance = WWDG;
  hwwdg.Init.Prescaler = WWDG_PRESCALER_8;
  hwwdg.Init.Window = 0x7F;
  hwwdg.Init.Counter = 0x7F;
  hwwdg.Init.EWIMode = WWDG_EWI_ENABLE;
  if (HAL_WWDG_Init(&hwwdg) != HAL_OK)
  {
		_Error_Handler(__FILE__, __LINE__);
  }
  /* USER CODE BEGIN WWDG_Init 2 */

  /* USER CODE END WWDG_Init 2 */

}
#endif

void sigfox_set_rf_power(uint8_t rf_pwr)
{
	uint8_t smps_t = S2LP_SMPS_OFF;

	switch(rf_pwr)
	{
		case 0x00: 	{smps_t = S2LP_SMPS_1V4; break;}
		case 0x01: 	{smps_t = S2LP_SMPS_1V5; break;}
		case 0x02: 	{smps_t = S2LP_SMPS_1V6; break;}
		case 0x03: 	{smps_t = S2LP_SMPS_1V7; break;}
		case 0x04: 	{smps_t = S2LP_SMPS_1V8; break;}
		default: 	{smps_t = S2LP_SMPS_1V4; break;}
	}
		
	ST_RF_API_smps(smps_t);
	ST_RF_API_reduce_output_power(SFX_REDUCED_POWER);
}

void rtc_time_reset(RTC_TimeTypeDef *ptime)
{
	ptime->DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	ptime->Hours = 0;
	ptime->Minutes = 0;
	ptime->SecondFraction = 0;
	ptime->Seconds = 0;
	ptime->StoreOperation = RTC_STOREOPERATION_SET;
	ptime->SubSeconds = 0;
	ptime->TimeFormat = RTC_HOURFORMAT12_AM;
}
/* --------------------------------------------------------------------------*/
void rtc_date_reset(RTC_DateTypeDef *pdate)
{
	pdate->Date = 0;
	pdate->Month = 0;
	pdate->WeekDay = 0;
	pdate->Year = 0;
}
/* --------------------------------------------------------------------------*/
//#define INCLUDE_SIGFOX_ADDON_TEST

#ifdef INCLUDE_SIGFOX_ADDON_TEST
#include "addon_sigfox_verified_api.h"
sfx_test_mode_t test_mode_num = SFX_TEST_MODE_TX_BPSK; 
void sigfox_addon_test(sfx_test_mode_t test_mode_num);
void sigfox_addon_test_logic(void)
{
	
	sfx_test_mode_t test_mode_array[] = 
	{
		SFX_TEST_MODE_TX_BPSK,
		SFX_TEST_MODE_TX_PROTOCOL,
		SFX_TEST_MODE_RX_PROTOCOL,
		SFX_TEST_MODE_RX_GFSK,
		SFX_TEST_MODE_RX_SENSI,
		SFX_TEST_MODE_TX_SYNTH,
		SFX_TEST_MODE_TX_FREQ_DISTRIBUTION,
		SFX_TEST_MODE_TX_BIT,
		SFX_TEST_MODE_PUBLIC_KEY,
		SFX_TEST_MODE_NVM};
	
	starttime = HAL_GetTick();	
	if(HAL_GPIO_ReadPin(RS_INT_GPIO_Port, RS_INT_Pin))
	{
		while(HAL_GPIO_ReadPin(RS_INT_GPIO_Port, RS_INT_Pin))
		{
			rs_time = HAL_GetTick() - starttime;
			if((rs_time > 2000 && rs_time < 2500))
			{
				led_en(OFF, 1);
			}
			else if((rs_time > 4000 && rs_time < 4500))
			{
				led_en(OFF, 1);
			}
			else if((rs_time > 6000 && rs_time < 6500))
			{
				led_en(OFF, 1);
			}
			else if((rs_time > 8000 && rs_time < 8500))
			{
				led_en(OFF, 1);
			}
			else if((rs_time > 10000 && rs_time < 10500))
			{
				led_en(OFF, 1);
			}
			else if((rs_time > 12000 && rs_time < 12500))
			{
				led_en(OFF, 1);
			}
			else if((rs_time > 14000 && rs_time < 14500))
			{
				led_en(OFF, 1);
			}
			else if((rs_time > 16000 && rs_time < 16500))
			{
				led_en(OFF, 1);
			}
			else if((rs_time > 18000 && rs_time < 18500))
			{
				led_en(OFF, 1);
			}
			else if((rs_time > 20000 && rs_time < 20500))
			{
				led_en(OFF, 1);
			}
			else if(rs_time > 20500)
			{
				break;
			}
			else
			{
				led_en(ON, 1);
			}
		}
	}

	if(rs_time < 2000)
	{
		ledblink(10);
		led_en(OFF, 1);
		HAL_Delay(50);
		stop_mode_enter(1);
		NVIC_SystemReset();
	}
	else
	{
		sigfox_addon_test(test_mode_array[(rs_time/2000) - 1]);
	}
}
/* --------------------------------------------------------------------------*/
void sigfox_addon_test(sfx_test_mode_t test_mode_num)
{
	sfx_error_t s_error_test;
	
	led_en(ON, 1);
	
	sigfox_library_setup();	
	SdkEvalExitShutdown();
	SIGFOX_API_close();
	rf_en(ON);	
	ST_RF_API_reduce_output_power(SFX_REDUCED_POWER);        // 0x00 - 0x7f
	
	s_error_test = ADDON_SIGFOX_VERIFIED_API_test_mode(SFX_RC1, test_mode_num);
	UNUSED(s_error_test);
	led_en(OFF, 1);
	HAL_Delay(50);
	rf_en(OFF);
	HAL_Delay(50);
	stop_mode_enter(0);
	
	NVIC_SystemReset();
}
#endif
/* --------------------------------------------------------------------------*/

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{

  if(htim_base->Instance==TIM6)
  {
  /* USER CODE BEGIN TIM6_MspInit 0 */

  /* USER CODE END TIM6_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_TIM6_CLK_ENABLE();
    /* TIM6 interrupt Init */
    HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 1, 2);
    HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
  /* USER CODE BEGIN TIM6_MspInit 1 */

  /* USER CODE END TIM6_MspInit 1 */
  }

}
/* --------------------------------------------------------------------------*/
/* TIM6 init function */
static void MX_TIM6_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;

  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 8000;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 1000;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

	HAL_TIM_Base_Start_IT(&htim6);
//	TIM6->DIER |= TIM_DIER_UIE;
//	TIM6->CR1 |= TIM_CR1_CEN;
	
}

/* --------------------------------------------------------------------------*/
//uint32_t update_bat(void);
	
//uint32_t update_bat(void)
//{
//	uint8_t loop = 0x00;
//	uint32_t bat_out;
//	while(loop <  10)
//	{
//		bat_out = get_bat(ON);
//		loop++;
//		HAL_Delay(100);
//	}
//	
//	get_bat(OFF);
//	
//	return bat_out;
//}
/* --------------------------------------------------------------------------*/
void debug_set_midnight(void)
{
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	struct tm* midnighttime;
	time_t midnighttimestamp, waketime;
	
	rtc_time_reset(&sTime);
	rtc_date_reset(&sDate);
	
	waketime = mktime(mytime_temp);
	
	midnighttimestamp = waketime - 60;
	
	midnighttime = localtime(&midnighttimestamp);
	
	sTime.Hours = midnighttime->tm_hour;
	sTime.Minutes = midnighttime->tm_min;
	sTime.Seconds = midnighttime->tm_sec;
	sDate.Date = midnighttime->tm_mday;
	sDate.Month = midnighttime->tm_mon;
	sDate.Year = midnighttime->tm_year;
	
	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
}
/* --------------------------------------------------------------------------*/
time_t time_different(time_t time1, time_t time2)
{
	time_t dtime;
	if(time1 > time2)
	{
		dtime = (time1 - time2);
	}
	else
	{
		dtime = (time2 - time1);
	}
	return dtime;
}
/* --------------------------------------------------------------------------*/
time_t get_now_timestamp(void)
{
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;	
	struct tm nowtime;
	time_t nowtimestamp;
	
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	
	nowtime.tm_hour 	= sTime.Hours;
	nowtime.tm_min  	= sTime.Minutes;
	nowtime.tm_sec	 	= sTime.Seconds;
	if(sDate.Year < 100)
	{
		sDate.Year = 119;
	}
	nowtime.tm_year 	= sDate.Year;
	nowtime.tm_mon		=	sDate.Month;
	nowtime.tm_mday	= sDate.Date;
	
	nowtimestamp 			= mktime(&nowtime);	
	
	return nowtimestamp;
}
/* --------------------------------------------------------------------------*/
void accel_dur_set(void)
{
	uint16_t buf;	

	MX_I2C1_Init();

	accel_sleep();
	
	HAL_Delay(50);
	
	buf = ((tracker_data.no_movement_time * 60 )/8);
	
	tracker_data.acc_nomove_ths = (uint8_t) ((uint16_t)buf / 0xff);
	tracker_data.acc_res_dur = (uint8_t)buf % 0xff;
	
	accel_init_reg[35] = tracker_data.acc_sensitivity;
	
	if(tracker_data.acc_nomove_ths == 0)
	{
		nomove_sub = 1;
		accel_init_reg[37] =	tracker_data.acc_res_dur;
	}
	else
	{
		accel_init_reg[37] =	0xFF;
		
	}
	
	accel_init();
	
	if(accel_int2_next_edge == INT_RISING_EDGE)
	{
		accel_reset_act_int2();
	}
	
	mcu_accel_int2_setting(accel_int2_next_edge);
}
/* --------------------------------------------------------------------------*/
void set_time(struct tm *time)
{
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;	
	
	rtc_time_reset(&sTime);
	rtc_date_reset(&sDate);
	
	sTime.Hours = time->tm_hour;
	sTime.Minutes = time->tm_min;
	sTime.Seconds = time->tm_sec;
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_SET;
	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
	/* USER CODE BEGIN RTC_Init 3 */

	/* USER CODE END RTC_Init 3 */

	sDate.Month = time->tm_mon + 1; //translate time.h month to MCU month
	sDate.Date = time->tm_mday; 
	sDate.Year = time->tm_year;
	sDate.WeekDay = 0;

	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
}
/* --------------------------------------------------------------------------*/
_Bool mcu_read_time(void)
{
	_Bool time_init = 0x0;
	time_init = *(uint32_t*)TIME_INIT_ADDRESS;
	
	return time_init;
	
}
/* --------------------------------------------------------------------------*/
void mcu_write_time(_Bool time_init_flag)
{
	HAL_FLASHEx_DATAEEPROM_Unlock();
	
	
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, TIME_INIT_ADDRESS, time_init_flag);

	
	HAL_FLASHEx_DATAEEPROM_Lock();
}
/* --------------------------------------------------------------------------*/
void alarm_a_midnight_set(void)
{
	RTC_DateTypeDef sDate;		
	
	mytime.tm_hour 	= tracker_data.scheduled_hour;
	mytime.tm_min  	= 0;
	mytime.tm_sec	 	= 0;
	mytime.tm_year 	= sDate.Year + 100;
	mytime.tm_mon		=	sDate.Month - 1;  // translate the MCU month to time.h month 
	mytime.tm_mday	= sDate.Date;
	
	mytimestampA 			= mktime(&mytime);
	
	mytime_temp = localtime(&mytimestampA);
	
	sAlarmA.AlarmTime.Hours 	= mytime_temp->tm_hour;
  sAlarmA.AlarmTime.Minutes =	mytime_temp->tm_min;
  sAlarmA.AlarmTime.Seconds = mytime_temp->tm_sec;
  sAlarmA.AlarmTime.SubSeconds = 0;
  sAlarmA.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarmA.AlarmTime.StoreOperation = RTC_STOREOPERATION_SET;
  sAlarmA.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
  sAlarmA.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarmA.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarmA.AlarmDateWeekDay = 1;
  sAlarmA.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarmA, RTC_FORMAT_BIN) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
	
	HAL_NVIC_SetPriority(RTC_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(RTC_IRQn);
}
/* --------------------------------------------------------------------------*/
void alarm_b_set_off(void)
{
	sAlarmB.AlarmTime.Hours = 0;
  sAlarmB.AlarmTime.Minutes = 0;
  sAlarmB.AlarmTime.Seconds = 0;
  sAlarmB.AlarmTime.SubSeconds = 0;
  sAlarmB.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarmB.AlarmTime.StoreOperation = RTC_STOREOPERATION_SET;
  sAlarmB.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
  sAlarmB.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarmB.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarmB.AlarmDateWeekDay = 1;
  sAlarmB.Alarm = RTC_ALARM_B;
	
	mytimestampB = 0;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarmB, RTC_FORMAT_BIN) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
	
	HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_B);
}

//void sigfox_debug_ul(void)
//{
//	uint8_t  message_buf[12], dl_frame[8];
//	
//	sigfox_library_setup();
//	
//	ST_RF_API_reduce_output_power(SFX_REDUCED_POWER);        // 0x00 - 0x7f
//	SdkEvalExitShutdown();
//	rf_en(ON);
//	
////	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

//	
//	
//	message_buf[0] = sTime.Hours;
//	message_buf[1] = sTime.Minutes;
//	message_buf[2] = sTime.Seconds;
//	message_buf[3] = rtc_int;
//	s_error = SIGFOX_API_send_frame(message_buf,4,dl_frame,2,0);
//	
//	SIGFOX_API_close();

//	
//}
/* --------------------------------------------------------------------------*/
void alarm_b_set(void)
{
	uint32_t seconds; 
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;		

	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	
//	if((RTC->CR & (1<<9|1<<13)) == (1<<9|1<<13))
//	{
//		
//		return;					//check whether the alarm is configured
//	}
	
	mytime.tm_hour 	= sTime.Hours;
	mytime.tm_min  	= sTime.Minutes;
	mytime.tm_sec	 	= sTime.Seconds;
	if(sDate.Year < 100)
	{
		sDate.Year = 119;
	}
	mytime.tm_year 	= sDate.Year;
	mytime.tm_mon		=	sDate.Month - 1;  // translate the MCU month to time.h month 
	mytime.tm_mday	= sDate.Date;
	
	mytimestampB 			= mktime(&mytime);
	
	seconds = (tracker_data.no_movement_time - ACCEL_TIMER_MAX_MINS)*60;  // max accel timer value = 2048 secs

	
	mytimestampB += seconds;
	mytime_temp = localtime(&mytimestampB);
	
	sAlarmB.AlarmTime.Hours = mytime_temp->tm_hour;
  sAlarmB.AlarmTime.Minutes = mytime_temp->tm_min;
  sAlarmB.AlarmTime.Seconds = mytime_temp->tm_sec;
  sAlarmB.AlarmTime.SubSeconds = 0;
  sAlarmB.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarmB.AlarmTime.StoreOperation = RTC_STOREOPERATION_SET;
  sAlarmB.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
  sAlarmB.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarmB.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarmB.AlarmDateWeekDay = 1;
  sAlarmB.Alarm = RTC_ALARM_B;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarmB, RTC_FORMAT_BIN) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
	
	HAL_NVIC_SetPriority(RTC_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(RTC_IRQn);
}
/* --------------------------------------------------------------------------*/
uint32_t ul_freq_to_sec(uint8_t t_r, uint8_t ul_f)
{
	uint32_t seconds=3600;

	switch(t_r)
	{
		case 0: 
		{
			seconds = 60;
			break;
		}
		case 1:
		{
			seconds = 3600;
			break;
		}
		case 2: 
		{
			seconds = 86400;
			break;
		}
		case 3:
		{
			seconds = 1;
			break;
		}
	}	
	
	seconds *= ul_f;
	
	return seconds;
}
/* --------------------------------------------------------------------------*/
void alarm_a_set(uint32_t sec )
{
	//Get current time
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;	
	
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	
	mytime.tm_hour 	= sTime.Hours;
	mytime.tm_min  	= sTime.Minutes;
	mytime.tm_sec	 	= sTime.Seconds;
	if(sDate.Year < 100)
	{
		sDate.Year = 119;
	}
	mytime.tm_year 	= sDate.Year;
	mytime.tm_mon		=	sDate.Month-1;
	mytime.tm_mday	= sDate.Date;
	
	mytimestampA 			= mktime(&mytime);	
 


//	Add delay to current time
	
	mytimestampA +=sec;
	

	//timestamp to time struct
	mytime_temp = localtime(&mytimestampA);
	
	
	mid_night = 0;
	
	random_x = (uint8_t)(get_temp()*100);
	random_y = (uint8_t)hrtc.Instance->SSR;
	
	//date changed and calendar is valid, midnight alarm is set
	if((mytime_temp->tm_mday != mytime.tm_mday) && ((RTC->ISR & 1<<4) != 0))   
	{
		mytime_temp->tm_hour 	= 0;
		mytime_temp->tm_min 	= random_x % 5;
		mytime_temp->tm_sec 	= random_y % 60;
		mid_night = 1;
		
	}
/* --------------------------------------------------------------------------*/	
//set alarm
	sAlarmA.AlarmTime.Hours 	= mytime_temp->tm_hour;
  sAlarmA.AlarmTime.Minutes =	mytime_temp->tm_min;
  sAlarmA.AlarmTime.Seconds = mytime_temp->tm_sec;
  sAlarmA.AlarmTime.SubSeconds = 0;
  sAlarmA.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarmA.AlarmTime.StoreOperation = RTC_STOREOPERATION_SET;
  sAlarmA.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarmA.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarmA.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarmA.AlarmDateWeekDay = mytime_temp->tm_mday;
  sAlarmA.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarmA, RTC_FORMAT_BIN) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
	
	HAL_NVIC_SetPriority(RTC_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(RTC_IRQn);
}
/* --------------------------------------------------------------------------*/
void mcu_accel_int2_setting(_Bool edge)
{
	GPIO_InitTypeDef GPIO_InitStruct;	
	
	
	if(edge == INT_RISING_EDGE)
	{
		GPIO_InitStruct.Pin = ACCEL_INT2_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		
		EXTI->EMR = RS_INT_Pin|ACCEL_INT2_Pin;
		EXTI->RTSR = RS_INT_Pin|ACCEL_INT2_Pin;	
		EXTI->FTSR = 0x00;
		
	}
	else // FALLING_EDGE
	{
		GPIO_InitStruct.Pin = ACCEL_INT2_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);		
		
		EXTI->EMR = RS_INT_Pin|ACCEL_INT2_Pin;
		EXTI->RTSR = RS_INT_Pin;	
		EXTI->FTSR = ACCEL_INT2_Pin;
		
	}
	
	HAL_NVIC_SetPriority(EXTI4_15_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
	
}
/* --------------------------------------------------------------------------*/
void mcu_system_init(void)
{
	  /* Initialize the hardware */
//  MCU_BSP_Init();
								
												
	HAL_Init(); 
	SystemClock_Config(); 

  /* Enable the GPIO Clock, reset S2LP */
  SdkEvalM2SGpioInit(M2S_GPIO_SDN,M2S_MODE_GPIO_OUT);
	
  /* Enable SPI */
  SdkEvalSpiInit();
	
	
  SdkDelayMs(10); //Wait for CS to rise

  /* Identify the RF board reading some production data */
  //S2LPManagementIdentificationRFBoard();
  
  /* Reset S2LP afte MCU reset*/
  SdkEvalEnterShutdown();
  SdkDelayMs(10);
  SdkEvalExitShutdown();
  
  /* Macro thath define and initializze a structure named nvmconfig */
  INIT_NVM_CONFIG(nvmConfig);
	
  /*********************** Sigfox Credentials Management***********************/
#ifdef USE_FLASH
  nvmConfig.nvmType = NVM_TYPE_FLASH;
  nvmConfig.userSpaceAddress = (uint32_t)FLASH_USER_START_ADDR; /* Store here NVM recurrent data */
  nvmConfig.dataPageAddress = (uint32_t)FLASH_BOARD_START_ADDR; /* Store here NVM board data (NVM_BoardDataType) */
#endif
  
  SetNVMInitial(&nvmConfig);
//----------------------------Hard Code Credential------------------------------------------
	

//	if(id == 0x00)
//	{		
//		NVM_BoardDataType rhino; 

//		uint8_t pac_buf[] = {0x0B, 0x4A, 0x17, 0x3D, 0x96, 0xCF, 0x12, 0xE0};
//		uint8_t key_buf[] = {0x03, 0x24, 0xC9, 0x51, 0x2D, 0x05, 0x18, 0xE5, 0x92,
//													0x6D, 0x34, 0x79, 0x6D, 0x91, 0xFF, 0xA2};
//		uint8_t aux_buf[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//													0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//		rhino.id = 0x00D20597;
//		rhino.rcz = 0x03000001;
//		strcpy((char*)rhino.pac, (char*)pac_buf);
//		strcpy((char*)rhino.key, (char*)key_buf);
//		rhino.freqOffset = 1740;
//		rhino.rssiOffset = 16;
//		strcpy((char*)rhino.aux, (char*)aux_buf);
//		rhino.checkSum = 0x00000C0F;		
//		NVM_WriteBoardData(&rhino);	
//	}
//----------------------------Hard Code Credential------------------------------------------	
	


	
#ifdef USE_FLASH
  /* Retrieve Sigfox info from FLASH */
  int32_t freqOffset;
  int8_t  rssiOffset;

  retr_err = enc_utils_retrieve_data_from_flash(&id, pac, &rcz, &freqOffset, &rssiOffset);

  if(!retr_err)
  {
    /* Override offsets */
    S2LPManagementSetOffset(freqOffset);
    retr_err = ST_RF_API_set_freq_offset(freqOffset);

    if(!retr_err)
      retr_err = ST_RF_API_set_rssi_offset(rssiOffset);
  }
#else    
  SdkEvalEnterShutdown();
  SdkDelayMs(1);
  /* Retrieve Sigfox info from EEPROM */
//  retr_err=enc_utils_encrypt(enc_key, dec_key, 16, dec_key1, KEY_ENC_VARIABLE);
//	retr_err=enc_utils_retrieve_data(&id, pac, &rcz);
#endif  
  
  if(retr_err!=RETR_OK)
    Fatal_Error();
    /* If the retriever returned an error (code different from RETR_OK) the application must not continue */

	
}

/* --------------------------------------------------------------------------*/
void stop_mode_enter(_Bool use_rtc)
{
	GPIO_InitTypeDef GPIO_InitStruct;
//	SIGFOX_API_close();	
	
	
	HAL_GPIO_WritePin(GPIOA, RS_INT_Pin, GPIO_PIN_RESET);
	
	stop_mode_config();
	
  HAL_PWREx_EnableUltraLowPower();
  HAL_PWREx_EnableFastWakeUp();

	__HAL_RCC_PWR_CLK_ENABLE();

	GPIO_InitStruct.Pin = RS_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(RS_INT_GPIO_Port, &GPIO_InitStruct);
	

	
	  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_1_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);
	

	
	
	SYSCFG->EXTICR[0] = 0x00;
	SYSCFG->EXTICR[2] = 0x01;
	
	
	
	if(use_rtc)
	{
//		MX_RTC_Init(0);
//		alarm_a_set();
			HAL_RTC_Init(&hrtc);
		
		EXTI->EMR |= (1<<17);
		EXTI->RTSR |= (1<<17);	
		EXTI->FTSR |= (1<<17);			// set up Alarm interrupt
		EXTI->SWIER |= (1<<17);
		
		
	}
	else
	{
		SYSCFG->EXTICR[2] = 0x00;
		EXTI->EMR = RS_INT_Pin;
		EXTI->RTSR = RS_INT_Pin;	
		HAL_RTC_DeInit(&hrtc);
		__HAL_RCC_WWDG_CLK_DISABLE();
		
	}
	
//	HAL_RTCEx_SetWakeUpTimer_IT(&hrtc,0,RTC_WAKEUPCLOCK_RTCCLK_DIV16);
	HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
	
	SCB->SCR |= 0x10;
	
	SysTick->CTRL &= (~SysTick_CTRL_ENABLE_Msk);
	
	RTC->CR &= ~(RTC_CR_WUTE|RTC_CR_WUTIE); 
		
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
	
	#ifdef INCLUDE_WWDG
	HAL_WWDG_Refresh(&hwwdg);
	#endif
//	HAL_PWREx_DisableUltraLowPower();
//	__HAL_RCC_PWR_CLK_DISABLE();
	
}
/* --------------------------------------------------------------------------*/
void stop_mode_config(void)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
	
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWREx_EnableUltraLowPower();
  HAL_PWREx_EnableFastWakeUp();
  __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_HSI);
//	__HAL_RCC_HSISTOP_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
	
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
	
//  GPIO_InitStructure.Pin = GPIO_PIN_All & (~GPIO_PIN_13) & (~GPIO_PIN_14) & (~RS_INT_Pin) & (~GPIO_PIN_15)& (~S2LP_SDN_Pin);
//  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
//  GPIO_InitStructure.Pull = GPIO_NOPULL;
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure); 
//	
//	GPIO_InitStructure.Pin = GPIO_PIN_All & (~GPIO_PIN_0) & (~ACCEL_INT2_Pin)& (~GPIO_PIN_6)& (~GPIO_PIN_7);
//  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
//  GPIO_InitStructure.Pull = GPIO_NOPULL;
//	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.Pin = GPIO_PIN_All & (~GPIO_PIN_13) & (~GPIO_PIN_14) & (~S2LP_SDN_Pin)& (~GPIO_PIN_15)& (~RS_INT_Pin);
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	GPIO_InitStructure.Pin = GPIO_PIN_All & (~GPIO_PIN_0) & (~ACCEL_INT2_Pin) & (~LED_Pin) &(~PA_CSD_Pin);
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	HAL_GPIO_DeInit(GPIOB, S2LP_GPIO0_Pin);

//  GPIO_InitStructure.Pin = GPIO_PIN_All;
//  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
//  GPIO_InitStructure.Pull = GPIO_NOPULL;
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure); 
//	
//	GPIO_InitStructure.Pin = GPIO_PIN_All;
//  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
//  GPIO_InitStructure.Pull = GPIO_NOPULL;
//	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
//	HAL_GPIO_DeInit(GPIOB, S2LP_GPIO0_Pin);

  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();

	HAL_I2C_DeInit(&hi2c1);
	HAL_UART_DeInit(&huart1);
	HAL_ADC_DeInit(&hadc);
	HAL_TIM_Base_DeInit(&htim6);

	ADC->CCR &= ~(ADC_CCR_TSEN);                  // Turn off the MCU temperature sensor

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5 | GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14 | GPIO_PIN_11 | GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13 , GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6|GPIO_PIN_7|S2LP_GPIO1_Pin|S2LP_GPIO0_Pin|GPIO_PIN_4|GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_SET);
	HAL_Delay(100);
//	get_bat(OFF);
	gps_en(OFF, 0);
	HAL_Delay(100);
	rf_en(OFF);

}


/* --------------------------------------------------------------------------*/
void led_en(uint8_t on, _Bool en)
{
	if(!en)
	{	
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
		return;
	}
	else
	{
		if(on == ON)
		{
			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
		}
		else if(on == OFF)
		{
			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
		}
		else if(on == TOG)
		{
			HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		}
	}
}
/* --------------------------------------------------------------------------*/
void eeprom_load_para(trackerparameter_t* data)
{
	uint8_t nm_ft_led;
	data->uplink_freq 			= *(uint32_t*)UL_FREQ_ADDRESS;
	data->downlink_freq 		= *(uint32_t*)DL_THS_ADDRESS;
	data->gps_timeout 			= *(uint32_t*)GPS_TIMEOUT_ADDRESS;
	data->acc_mode					= *(uint32_t*)ACCEL_MODE_ADDRESS;
	data->acc_sensitivity	= *(uint32_t*)ACCEL_SENSI_ADDRESS;
//	tracker_data.force_transition = *(uint32_t*)
	data->gps_mode					= *(uint32_t*)GPS_MODE_ADDRESS;
	data->gps_power_mode		= *(uint32_t*)GPS_PWR_MODE_ADDRESS;
	data->idle_mode					= *(uint32_t*)IDLE_MODE_ADDRESS;
//	tracker_data.info_code				= *(uint32_t*)
	nm_ft_led 							= *(uint32_t*)NOMOVE_FORCET_LED_ADDRESS;
//	tracker_data.no_movement			= *(uint32_t*)
	data->no_movement_time	= *(uint32_t*)NOMOVE_TIME_ADDRESS;
	data->positioning_freq	= *(uint32_t*)HOUR_OF_MSG_ADDRESS;
	data->product_ver				= *(uint32_t*)PRODUCT_VERSION_ADDRESS;
	data->time_ref					= *(uint32_t*)TIME_REF_ADDRESS;
//	
//	data->ul_counter 				= *(uint32_t*)UL_COUNTER_ADDRESS;
//	data->test_mode					= *(uint32_t*)TEST_MODE_ADDRESS;
//	data->rc_sel						= *(uint32_t*)RC_SEL_ADDRESS;
//	data->cw_power					= *(uint32_t*)CW_POWER_ADDRESS;
//	data->move_counter			= *(uint32_t*)MOVE_COUNTER_ADDRESS;
//	data->move_ths					= *(uint32_t*)MOVE_THS_ADDRESS;
//	data->nomove_counter		= *(uint32_t*)NOMOVE_COUNTER_ADDRESS;
//	data->nomove_ths				= *(uint32_t*)NOMOVE_THS_ADDRESS;
	
	data->no_movement 			= (nm_ft_led & 0x80) >> 7;
	data->force_transition 	= (nm_ft_led & 0x40) >> 6;
	data->led_active 				= (nm_ft_led & 0x20) >> 5;
	
}
/* --------------------------------------------------------------------------*/
sfx_u8 sigfox_setup_uldl(_Bool downlink_required)
{
	uint8_t bat_5b, message_buf[12], dl_frame[8];
	sfx_u8 s_err = 0;
	
	bat_idle = get_bat();
	
	bat_5b = (bat_idle - 2000)/75;
	tracker_data.info_code = 0x03;
	
	sigfox_library_setup();	
	rf_en(ON);
	sigfox_set_rf_power(tracker_data.rf_output_power);
	SdkEvalExitShutdown();
	
	
//	ST_MCU_API_TimerCalibration(20000);
	
	pack_ul_setup_payload(&tracker_data, bat_5b, message_buf );
	get_bat_en(ON);
	s_err = SIGFOX_API_send_frame(message_buf,9,dl_frame,2,downlink_required);
	get_bat_en(OFF);
	if(downlink_required)
	{
		if(s_err == 0)
		{
			unpack_dl_payload(&tracker_data, dl_frame);																						//save new configuration from DL payload
			accel_init_required = 1;
			
			if((memcmp(&old_dl_payload, &dl_frame, sizeof(dl_frame))) != 0)					//compare old and new configuration
			{																								
				memcpy(&old_dl_payload, &dl_frame, sizeof(dl_frame));				//if not equal, send another SETUP frame without DL request
				pack_ul_setup_payload(&tracker_data, bat_5b, message_buf );													
				s_err = SIGFOX_API_send_frame(message_buf,9,dl_frame,2,0);
			}
		}
	}
	SIGFOX_API_close();
	rf_en(OFF);
	return s_err;
}
/* --------------------------------------------------------------------------*/
//void sigfox_setup_ul(void)
//{
//	uint8_t bat_5b, message_buf[12], dl_frame[8];
//	bat_5b = (bat - 2000)/75;
//	tracker_data.info_code = 0x03;
//	
//	sigfox_library_setup();	
//	ST_RF_API_reduce_output_power(SFX_REDUCED_POWER);        // 0x00 - 0x7f
//	SdkEvalExitShutdown();
//	rf_en(ON);
//	
//	
//	pack_ul_setup_payload(&tracker_data, bat_5b, message_buf );
//	
//	SIGFOX_API_send_frame(message_buf,9,dl_frame,2,0);
//	
//	SIGFOX_API_close();
//	rf_en(OFF);

//}
/* --------------------------------------------------------------------------*/
//sfx_u8 sigfox_gps_ul(void)
//{
//	sfx_u8 s_err = 0;
//	uint32_t  lat_buf = 0x00000000, long_buf = 0x00000000;
//	uint8_t bat_5b, message_buf[11];
//	
//	bat_5b = (bat - 2000)/75;
//	
//	sigfox_library_setup();	
//	ST_RF_API_reduce_output_power(SFX_REDUCED_POWER);        // 0x00 - 0x7f
//	SdkEvalExitShutdown();
//	rf_en(ON);
//	
//	if(gps_fixed)
//	{
//		gps_get_lat_long(&lat_buf, &long_buf);
//		
//		random_x = (uint8_t) lat_buf;
//		random_y = (uint8_t) long_buf;
//		
//		sensor_data.GPS_state = 0;
//	}
//	else
//	{
//		sensor_data.GPS_state = 1;
//		lat_buf = 0;
//		long_buf = 0;
//	}
//	
//	if(HAL_GPIO_ReadPin(ACCEL_INT2_GPIO_Port, ACCEL_INT2_Pin))
//	{
//		sensor_data.no_movement = 0;
//	}
//	
//	sensor_data.gps_lat = lat_buf;
//	sensor_data.gps_long = long_buf;
//	sensor_data.gps_acq_time = (nowtime/1000)/3;
//	sensor_data.sensor1_val = 0x00;
//	
//	pack_ul_gps_payload(&sensor_data, bat_5b, message_buf);

//	s_err = SIGFOX_API_send_frame(message_buf,11,customer_resp,2,0);
//	
//	SIGFOX_API_close();
//	rf_en(OFF);
//	
//	return s_err;
//}
/* --------------------------------------------------------------------------*/
//sfx_u8 sigfox_gnss_uldl(_Bool downlink_required, _Bool use_ubiscale)
//{
//	sfx_u8 s_err = 0;
//	uint8_t bat_5b, dl_frame[8], frame_len, message_buf[SIGFOX_MSGLEN];
//	
//	bat_5b = (get_bat() - 2000)/75;
//	
//	sigfox_library_setup();	
//	ST_RF_API_reduce_output_power(SFX_REDUCED_POWER);        // 0x00 - 0x7f
//	SdkEvalExitShutdown();
//	rf_en(ON);	
//	
//	sensor_data.gps_acq_time = (nowtime/1000)/3;
//	
//	if(use_ubiscale)
//	{
//		
//		if(ubi_state == UBIGNSS_FIX_OK)
//		{
//			sensor_data.GPS_state = 0;
//			frame_len = 12;									//if gps fixed, send 12 bytes frame
//		}
//		else
//		{
//			sensor_data.GPS_state = 1;
//			frame_len = 2;									//if no gps fixed, send 2 bytes only
//		}
//		
//		pack_ul_ubignss_payload(&sensor_data, bat_5b, ubi_payload, message_buf);
//		
//	}
//	else
//	{
//		uint32_t  lat_buf = 0x00000000, long_buf = 0x00000000;
//		
//		if(gps_fixed)
//		{
//			gps_get_lat_long(&lat_buf, &long_buf);
//			
//			random_x = (uint8_t) lat_buf;
//			random_y = (uint8_t) long_buf;
//			
//			sensor_data.GPS_state = 0;
//			frame_len = 11;																	//if gps fixed, send 11 bytes frame
//		}
//		else
//		{
//			sensor_data.GPS_state = 1;
//			lat_buf = 0;
//			long_buf = 0;
//			frame_len = 2;																//if no gps fixed, send 2 bytes only
//		}
//		
//		sensor_data.gps_lat = lat_buf;
//		sensor_data.gps_long = long_buf;
//		sensor_data.sensor1_val = 0x00;
//		
//		pack_ul_gps_payload(&sensor_data, bat_5b, message_buf);
//	}
//	
//	s_err = SIGFOX_API_send_frame(message_buf,frame_len,dl_frame,2,downlink_required);
//	
//	if(downlink_required)
//	{
//		if(s_err == 0)
//		{
//			memcpy(&old_dl_payload, &dl_frame, sizeof(dl_frame));
//			unpack_dl_payload(&tracker_data, dl_frame);
//			accel_init_required = 1;
//		}
//	}
//	SIGFOX_API_close();
//	rf_en(OFF);
//	
//	return s_err;
//}

/* --------------------------------------------------------------------------*/
sfx_u8 sigfox_ubi_uldl(_Bool downlink_required)
{
	sfx_u8 s_err = 0;
	uint8_t bat_5b, message_buf[SIGFOX_MSGLEN], dl_frame[8], frame_len;
	
	bat_idle = get_bat();
	
	bat_5b = (bat_idle - 2000)/75;
	
	sigfox_library_setup();	
	rf_en(ON);
	sigfox_set_rf_power(tracker_data.rf_output_power);
	SdkEvalExitShutdown();
	
	
	sensor_data.gps_acq_time = (nowtime/1000)/3;

	if(ubi_state == UBIGNSS_FIX_OK)
	{
		sensor_data.GPS_state = 0;
		frame_len = 12;									//if gps fixed, send 12 bytes frame
	}
	else
	{
		sensor_data.GPS_state = 1;
		frame_len = 2;									//if no gps fixed, send 2 bytes only
	}
	
	pack_ul_ubignss_payload(&sensor_data, bat_5b, ubi_payload, message_buf);
	
	get_bat_en(ON);
	s_err = SIGFOX_API_send_frame(message_buf,frame_len,dl_frame,2,downlink_required);
	get_bat_en(OFF);
	
	if(downlink_required)
	{
		if(s_err == 0)
		{
			memcpy(&old_dl_payload, &dl_frame, sizeof(dl_frame));
			unpack_dl_payload(&tracker_data, dl_frame);
			accel_init_required = 1;
		}
	}
	SIGFOX_API_close();
	rf_en(OFF);
	
	return s_err;
}

/* --------------------------------------------------------------------------*/
sfx_u8 sigfox_gps_uldl(_Bool downlink_required)
{
	sfx_u8 s_err = 0;
	uint32_t  lat_buf = 0x00000000, long_buf = 0x00000000;
	uint8_t bat_5b, message_buf[11], dl_frame[8], frame_len;
	
	bat_idle = get_bat();
	
	bat_5b = (bat_idle - 2000)/75;

	
	sigfox_library_setup();	
	rf_en(ON);
	sigfox_set_rf_power(tracker_data.rf_output_power);
	SdkEvalExitShutdown();
	
	
	if(gps_fixed)
	{
		gps_get_lat_long(&lat_buf, &long_buf);
		
		
		sensor_data.GPS_state = 0;
		frame_len = 11;																	//if gps fixed, send 11 bytes frame
	}
	else
	{
		sensor_data.GPS_state = 1;
		lat_buf = 0;
		long_buf = 0;
		frame_len = 2;																//if no gps fixed, send 2 bytes only
	}
	
//	if(HAL_GPIO_ReadPin(ACCEL_INT2_GPIO_Port, ACCEL_INT2_Pin))
//	{
//		if(tracker_data.no_movement_time < 34)
//		{
//			sensor_data.no_movement = 0;
//		}
//	}
	
	sensor_data.gps_lat = lat_buf;
	sensor_data.gps_long = long_buf;
	sensor_data.gps_acq_time = (nowtime/1000)/3;
	sensor_data.sensor1_val = 0x00;
	
	pack_ul_gps_payload(&sensor_data, bat_5b, message_buf);

	get_bat_en(ON);
	s_err = SIGFOX_API_send_frame(message_buf,frame_len,dl_frame,2,downlink_required);
	get_bat_en(OFF);	
	
	if(downlink_required)
	{
		if(s_err == 0)
		{
			memcpy(&old_dl_payload, &dl_frame, sizeof(dl_frame));
			unpack_dl_payload(&tracker_data, dl_frame);
			accel_init_required = 1;
		}
	}
	SIGFOX_API_close();
	rf_en(OFF);
	
	return s_err;
}
/* --------------------------------------------------------------------------*/
#ifdef INCLUDE_SIGFOX_ADDON_TEST
_Bool sigfox_testmode_dl()
{
		uint32_t bat = get_bat();
		uint8_t dl_frame[8], tx_frame[6] = {0x11,0x11,0x11,0x11, (uint8_t)((bat&0xFF00)>>8), (uint8_t)(bat&0x00FF)};
		_Bool dl_er = 0x00;
		
		sigfox_library_setup();	
		rf_en(ON);
		sigfox_set_rf_power(tracker_data.rf_output_power);
		SdkEvalExitShutdown();
		
		dl_frame[7] = 0x00;
		dl_er = 0;
		get_bat_en(ON);
		if(SIGFOX_API_send_frame(tx_frame,6,dl_frame,2,1) == 0x00)
		{
			
			dl_ths = dl_frame[0];
			ul_time = dl_frame[1]*0x100 + dl_frame[2];
			gps_timeout = dl_frame[3];
			test_mode = dl_frame[4];
			cw_power = dl_frame[5];
			rc_sel = dl_frame[6];
			
			if(dl_frame[7] == 0x08)
			{
				dl_er = 1;
			}			
		}
		get_bat_en(OFF);	
		SIGFOX_API_close();
		rf_en(OFF);		
		

		return dl_er;
}
/* --------------------------------------------------------------------------*/
uint8_t sigfox_rf_test(uint8_t tmode)
{
	static uint8_t last_tmode = 0x00;
	uint8_t ret_state;
//--------------Test Mode No 1-------------//
//--------------Continus Wave-------------//
	if(tmode == 0x01)
	{
		if(last_tmode != tmode)
		{
			rf_en(ON);
			gps_en(OFF, 0);
			SIGFOX_API_close();
			if(cw_power > 107)
			{
				cw_power = 107;
			}
			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
			ST_RF_API_reduce_output_power(cw_power);        // 0x00 - 0x7f
			SIGFOX_API_start_continuous_transmission(RC1_OPEN_UPLINK_CENTER_FREQUENCY, SFX_NO_MODULATION);
			last_tmode = tmode;
		}
		
		led_en(ON, 1);
		ret_state = S_TEST_MODE;
		
	}
	else
	{
		last_tmode = 0;
		ret_state = S_DEEPSLEEP;
	}
	return ret_state;
}
#endif
/* --------------------------------------------------------------------------*/
void eeprom_save_para(void)
{
	uint8_t nm_ft_led;

	tracker_data.product_ver = PRODUCT_VERSION;
	
	HAL_FLASHEx_DATAEEPROM_Unlock();
	
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, 			UL_FREQ_ADDRESS, 					tracker_data.uplink_freq);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,			UL_COUNTER_ADDRESS, 			ul_counter);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,			DL_THS_ADDRESS, 					tracker_data.downlink_freq);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,			GPS_TIMEOUT_ADDRESS, 			tracker_data.gps_timeout);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,			TEST_MODE_ADDRESS, 				test_mode);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,			RC_SEL_ADDRESS, 					rc_sel);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,			CW_POWER_ADDRESS, 				cw_power);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,			MOVE_COUNTER_ADDRESS, 		move_counter);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,			MOVE_THS_ADDRESS, 				move_ths);
//	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,			NOMOVE_COUNTER_ADDRESS,		nomove_counter);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,			NOMOVE_THS_ADDRESS, 			nomove_ths);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, 			PRODUCT_VERSION_ADDRESS,	tracker_data.product_ver);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, 			TIME_REF_ADDRESS,					tracker_data.time_ref);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, 			GPS_MODE_ADDRESS,					tracker_data.gps_mode);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, 			GPS_PWR_MODE_ADDRESS,			tracker_data.gps_power_mode);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, 			ACCEL_MODE_ADDRESS,				tracker_data.acc_mode);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, 			IDLE_MODE_ADDRESS,				tracker_data.idle_mode);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, 			ACCEL_SENSI_ADDRESS,			tracker_data.acc_sensitivity);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, 			NOMOVE_TIME_ADDRESS,			tracker_data.no_movement_time);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, 			HOUR_OF_MSG_ADDRESS,			tracker_data.positioning_freq);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, 			SCHEDULED_HOUR_ADDRESS,		tracker_data.scheduled_hour);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, 			RST_STATE_ADDRESS,				rst_state);
	
	nm_ft_led = (tracker_data.no_movement << 7) + (tracker_data.force_transition << 6) + (tracker_data.led_active << 5);
	
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, 			NOMOVE_FORCET_LED_ADDRESS, 	nm_ft_led		);
	
	
	HAL_FLASHEx_DATAEEPROM_Lock();
	
}

/* --------------------------------------------------------------------------*/
sfx_u8 sigfox_boot_dl(void)
{
		sfx_u8 s_err = 0;
		uint8_t dl_frame[8], boot_frame[1] = {0x00};
	
		sigfox_library_setup();	
		rf_en(ON);
		sigfox_set_rf_power(tracker_data.rf_output_power);
		SdkEvalExitShutdown();
		
		bat_idle = get_bat();
		
		get_bat_en(ON);
		s_err = SIGFOX_API_send_frame(boot_frame,1,dl_frame,2,1);
		get_bat_en(OFF);
//		dl_ths = dl_frame[0];
//		ul_time = dl_frame[1]*0x100 + dl_frame[2];
//		gps_timeout = dl_frame[3];
//		cw_power = dl_frame[5];
//		test_mode = 0x00;
		if(s_err == 0)
		{
			memcpy(&old_dl_payload, &dl_frame, sizeof(dl_frame));
			unpack_dl_payload(&tracker_data, dl_frame);
			accel_init_required = 1;
		}
//		else
//		{
//			eeprom_load_para(&tracker_data);
//		}
		SIGFOX_API_close();
		rf_en(OFF);
		return s_err;
}
/* --------------------------------------------------------------------------*/

/* --------------------------------------------------------------------------*/
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(hadc->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();
  
    /**ADC GPIO Configuration    
    PB1     ------> ADC_IN9 
    */
    GPIO_InitStruct.Pin = BAT_ADC_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BAT_ADC_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }

}
/* --------------------------------------------------------------------------*/
static void MX_ADC_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
    */
  hadc.Instance = ADC1;
  hadc.Init.OversamplingMode = DISABLE;
  hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerFrequencyMode = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel to be converted. 
    */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}
/* --------------------------------------------------------------------------*/
float get_temp(void)
{
	uint16_t count = 0;
	uint32_t temp_buff = 0;
	float temperature = 0;
	HAL_ADC_Start(&hadc);

	ADC1->CFGR1 |= ADC_CFGR1_CONT;
	ADC1->CHSELR = ADC_CHSELR_CHSEL18;
	ADC->CCR |= ADC_CCR_TSEN;
	HAL_ADC_PollForConversion(&hadc, 100);
	HAL_Delay(100);
	for(count = 0; count < 10; count++)
	{
		temp_buff += HAL_ADC_GetValue(&hadc); 
	}
	temp_buff = temp_buff/10;

	HAL_ADC_Stop(&hadc);
	
	temperature = (float)((temp_buff * 0.27) - 190);
//	temperature = ((temp_buff * VDD_APPLI / VDD_CALIB) - (int32_t) *TEMP30_CAL_ADDR );
//	temperature = temperature * (int32_t)(130 - 30);
//	temperature = temperature / (int32_t)(*TEMP130_CAL_ADDR - *TEMP30_CAL_ADDR);
//  temperature = temperature + 30;
	
	return (temperature);
}
/* --------------------------------------------------------------------------*/
void get_bat_en(_Bool en)
{
	bat_count = 0;
	bat_tx_raw = 0;
	
	if(en == ON)
	{
		ADC1->CHSELR = ADC_CHSELR_CHSEL9;

		HAL_GPIO_WritePin(GPIO_RES4_GPIO_Port, GPIO_RES4_Pin, GPIO_PIN_SET);
		delay_seconds(1); //HAL_Delay(1000);
		

	}
	else
	{
		HAL_ADC_Stop(&hadc);

		HAL_GPIO_WritePin(GPIO_RES4_GPIO_Port, GPIO_RES4_Pin, GPIO_PIN_RESET);
	}
}
/* --------------------------------------------------------------------------*/
uint32_t get_bat_adc(void)
{
	return HAL_ADC_GetValue(&hadc); 
}
/* --------------------------------------------------------------------------*/
uint32_t get_bat(void)
{
	uint32_t bat_buff = 0x00, count = 0;
	uint32_t bat = 0;
	
	ADC1->CHSELR = ADC_CHSELR_CHSEL9;

	HAL_GPIO_WritePin(GPIO_RES4_GPIO_Port, GPIO_RES4_Pin, GPIO_PIN_SET);
	
	delay_seconds(1); //HAL_Delay(1000);
	
	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, 10);	
	for(count = 0; count < 100; count++)
	{
		bat_buff += HAL_ADC_GetValue(&hadc); 
		
	}
	bat_buff = bat_buff/100;
	bat = bat_buff*2*VDD/4096 - BATADC_OFFSET;

	HAL_ADC_Stop(&hadc);

	HAL_GPIO_WritePin(GPIO_RES4_GPIO_Port, GPIO_RES4_Pin, GPIO_PIN_RESET);


	return bat;
	
}
/* --------------------------------------------------------------------------*/
void ledblink(uint8_t times)
{
	for(uint8_t count = 0; count < times; count++)
	{
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		SdkDelayMs(100);
	}
}
/* --------------------------------------------------------------------------*/
void rf_en(_Bool en)
{

	if(en == ON)
	{
		HAL_GPIO_WritePin(TCXO_EN_GPIO_Port, TCXO_EN_Pin, GPIO_PIN_SET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(S2LP_SDN_GPIO_Port, S2LP_SDN_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(PA_CSD_GPIO_Port, PA_CSD_Pin, GPIO_PIN_SET);
	}
	else if(en == OFF)
	{
		HAL_GPIO_WritePin(TCXO_EN_GPIO_Port, TCXO_EN_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(S2LP_SDN_GPIO_Port, S2LP_SDN_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(PA_CSD_GPIO_Port, PA_CSD_Pin, GPIO_PIN_RESET);
	}
}
/* --------------------------------------------------------------------------*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Configure LSE Drive Capability 
    */
  HAL_PWR_EnableBkUpAccess();

  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV16;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}
/* --------------------------------------------------------------------------*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(S2LP_SDN_GPIO_Port, S2LP_SDN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_RES4_Pin|S2LP_CSN_Pin|GPS_EN_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, GPS_EN_Pin, GPIO_PIN_SET);
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, TCXO_EN_Pin|PA_CSD_Pin|LED_Pin, GPIO_PIN_RESET);

	

//	GPIO_InitStruct.Pin = GPIO_PIN_11;
//  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
  /*Configure GPIO pin : RS_INT_Pin */
  GPIO_InitStruct.Pin = RS_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(RS_INT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : S2LP_SDN_Pin GPIO_RES4_Pin S2LP_CSN_Pin GPS_EN_Pin */
  GPIO_InitStruct.Pin = S2LP_SDN_Pin|GPIO_RES4_Pin|S2LP_CSN_Pin|GPS_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : TCXO_EN_Pin PA_CSD_Pin LED_Pin */
  GPIO_InitStruct.Pin = TCXO_EN_Pin|PA_CSD_Pin|LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : S2LP_GPIO0_Pin */
  GPIO_InitStruct.Pin = S2LP_GPIO0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(S2LP_GPIO0_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : S2LP_GPIO1_Pin */
  GPIO_InitStruct.Pin = S2LP_GPIO1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(S2LP_GPIO1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ACCEL_INT1_Pin ACCEL_INT2_Pin */
//  GPIO_InitStruct.Pin = ACCEL_INT1_Pin|ACCEL_INT2_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = ACCEL_INT1_Pin|ACCEL_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_1_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);

//  HAL_NVIC_SetPriority(EXTI2_3_IRQn, 1, 0);
//  HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);
	
	HAL_NVIC_SetPriority(EXTI4_15_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}
/* --------------------------------------------------------------------------*/

//void sigfox_geo_bat_ul(void)
//{

//	uint8_t geo_data[12]={0}, bat_time_data[5] ={0}, ns = 0, ew = 0;
//	
//	if(pNS[0] == 'N')
//	{
//		ns = 1;
//	}
//	else 
//	{
//		ns = 0;
//	}
//	
//	if(pEW[0] == 'E')
//	{
//		ew = 1;
//	}
//	else
//	{
//		ew = 0;
//	}
//	
//	
//		if(nowtime < 120000)
//		{
//	
//		geo_data[0] = ew*0x10 + ascii2hex(plong[0]);
//		geo_data[1] = ascii2hex(plong[1])*0x10 + ascii2hex(plong[2]); 
//		geo_data[2] = ascii2hex(plong[3])*0x10 + ascii2hex(plong[4]);
//		geo_data[3] = ascii2hex(plong[6])*0x10 + ascii2hex(plong[7]);
//		geo_data[4] = ascii2hex(plong[8])*0x10 + ascii2hex(plong[9]);
//		geo_data[5] = ascii2hex(plong[10])*0x10 + ns;
//		geo_data[6] = ascii2hex(plat[0])*0x10 + ascii2hex(plat[1]);
//		geo_data[7] = ascii2hex(plat[2])*0x10 + ascii2hex(plat[3]);
//		geo_data[8] = ascii2hex(plat[5])*0x10 + ascii2hex(plat[6]);
//		geo_data[9] = ascii2hex(plat[7])*0x10 + ascii2hex(plat[8]);
//		geo_data[10] = ascii2hex(plat[9])*0x10;
////		geo_data[11] = ascii2hex(plat[10])*0x10;	
//	
//		}

//			bat_time_data[0] = bat/0x100;
//			bat_time_data[1] = bat%0x100;
//			bat_time_data[2] = nowtime/0x1000;
//			bat_time_data[3] = nowtime/0x10;
//			bat_time_data[4] = (nowtime%0x10) * 0x10;
//			
//			s_error = SIGFOX_API_send_frame(geo_data,12,customer_resp,2,0);
//		
//			s_error = SIGFOX_API_send_frame(bat_time_data,5,customer_resp,2,0);	


//}

/* --------------------------------------------------------------------------*/
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
  /* Prevent unused argument(s) compilation warning */
  
//  UNUSED(hrtc);
//	mcu_system_init();
//	HAL_RTC_GetTime(hrtc, &sTime, RTC_FORMAT_BIN);
	
	rtc_int = W_RTC_A;

//	wake_src = W_RTC;

	
  //HAL_PWR_EnterSTANDBYMode();
  
}
/* --------------------------------------------------------------------------*/
void HAL_RTCEx_AlarmBEventCallback(RTC_HandleTypeDef *hrtc)
{
  /* Prevent unused argument(s) compilation warning */
  
//  UNUSED(hrtc);
//	wake_src = W_RTC;
	rtc_int = W_RTC_B;
	sensor_data.no_movement = 1;

  //HAL_PWR_EnterSTANDBYMode();
  
}
/* --------------------------------------------------------------------------*/
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
  /* Prevent unused argument(s) compilation warning */
  
  UNUSED(hrtc);
//	rtc_int = 0xFF;
	
//  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
  //HAL_PWR_EnterSTANDBYMode();
  
}
/* --------------------------------------------------------------------------*/
void standby_init(void)
{
  
  //CLEAR_BIT(PWR->CR, PWR_CR_CWUF);
  SET_BIT(PWR->CR, PWR_CR_CWUF);
  HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
  //HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN2);
}
/* --------------------------------------------------------------------------*/
void standby(_Bool rtc_use)
{
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;		
	
	gps_en(OFF, 0);
	SET_BIT(PWR->CR, PWR_CR_CWUF);
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
	
	if(rtc_use)
	{
		SystemClock_Config();
		
		
		MX_RTC_Init(1);
		
		HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
		HAL_RTC_GetAlarm(&hrtc, &sAlarmA, RTC_ALARM_A, RTC_FORMAT_BIN);
		HAL_RTC_GetAlarm(&hrtc, &sAlarmB, RTC_ALARM_B, RTC_FORMAT_BIN);
	}
	else
	{
		HAL_RTC_DeInit(&hrtc);
	}
//		
		if(tracker_data.acc_mode == 0x01 || tracker_data.acc_mode == 0x02)
		{
			accel_init();
//			accel_set_sensitivity(tracker_data.acc_sensitivity);
			accel_reset_act_int2();
		}
		else
		{
			accel_sleep();
		}
		
		HAL_PWR_EnterSTANDBYMode();
	//	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON,PWR_STOPENTRY_WFI);

}

/* --------------------------------------------------------------------------*/
uint32_t HAL_GetTick(void)
{
  return lSystickCounter;
}
/* --------------------------------------------------------------------------*/
void HAL_IncTick(void)
{
  lSystickCounter++;
}
/* --------------------------------------------------------------------------*/
static void MX_RTC_Init(_Bool reset)
{

	
	
  /* USER CODE BEGIN RTC_Init 0 */
//		HAL_RTCEx_EnableBypassShadow(&hrtc);
	
//		HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
//		HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
//		HAL_RTC_GetAlarm(&hrtc, &sAlarmA, RTC_ALARM_A, RTC_FORMAT_BIN);		
//		HAL_RTC_GetAlarm(&hrtc, &sAlarmB, RTC_ALARM_B, RTC_FORMAT_BIN);


  /* USER CODE END RTC_Init 0 */
//
  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef sDate;
//  RTC_AlarmTypeDef sAlarm;

  /* USER CODE BEGIN RTC_Init 1 */
	rtc_time_reset(&sTime);
	rtc_date_reset(&sDate);
  /* USER CODE END RTC_Init 1 */

    /**Initialize RTC Only 
    */
	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	hrtc.Init.AsynchPrediv = 127;	
	hrtc.Init.SynchPrediv = 255;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	if (HAL_RTC_Init(&hrtc) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
	
	

	
	
	if(reset)
	{

		/* USER CODE BEGIN RTC_Init 2 */

		/* USER CODE END RTC_Init 2 */

			/**Initialize RTC and set the Time and Date 
			*/
		

		sTime.Hours = 0;
		sTime.Minutes = 0;
		sTime.Seconds = 0;
		sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
		sTime.StoreOperation = RTC_STOREOPERATION_SET;
		if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
		{
			_Error_Handler(__FILE__, __LINE__);
		}
		/* USER CODE BEGIN RTC_Init 3 */

		/* USER CODE END RTC_Init 3 */

		sDate.WeekDay = 0;
		sDate.Month = RTC_MONTH_JANUARY;
		sDate.Date = 1;
		sDate.Year = 0;

		if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
		{
			_Error_Handler(__FILE__, __LINE__);
		}
	}
	else
	{
//			HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
//			HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
//		
//		if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
//		{
//			_Error_Handler(__FILE__, __LINE__);
//		}

//		if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
//		{
//			_Error_Handler(__FILE__, __LINE__);
//		}
	}
		
  /* USER CODE BEGIN RTC_Init 4 */
		HAL_RTCEx_EnableBypassShadow(&hrtc);
  /* USER CODE END RTC_Init 4 */

    /**Enable the Alarm A 
    */
	

//	alarm_b_set();

  /* USER CODE BEGIN RTC_Init 5 */
	

  /* USER CODE END RTC_Init 5 */

    /**Enable the WakeUp 
//    */

  /* USER CODE BEGIN RTC_Init 6 */

  /* USER CODE END RTC_Init 6 */

}
/* --------------------------------------------------------------------------*/
void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{

  if(hrtc->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_RTC_ENABLE();
    /* RTC interrupt Init */
//    HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 0, 0);
//    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
    HAL_NVIC_SetPriority(RTC_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(RTC_IRQn);
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }

}
/* --------------------------------------------------------------------------*/



/* --------------------------------------------------------------------------*/
void sigfox_library_setup(void)
{
	/*********************** Sigfox Library Settings*****************************/
  
  /* Put the radio off */
  SdkEvalEnterShutdown();
  
  /* Give the RF offset to the library */
  ST_RF_API_set_freq_offset(S2LPManagementGetOffset());
//	ST_RF_API_set_freq_offset(-6000000);
  
  /* Tell to the library if we are using a TCXO */
  ST_RF_API_set_tcxo(S2LPManagementGetTcxo());  
  
  /* in this case instruct the library to configure the GPIO accordingly */
  if(S2LPManagementGetRangeExtender())
  {
    /* Tell to the library if we are using a range extender */
    ST_RF_API_set_pa(1);
    
    /* Configure the PA driving pins according to the board pinout */
    ST_RF_API_gpio_tx_rx_pin(0);
    ST_RF_API_gpio_rx_pin(1);
    ST_RF_API_gpio_tx_pin(2);

    /* Tell to the library that a -16dB offet should be applied to the RSSI 
    due to the range extender LNA gain */
    ST_RF_API_set_rssi_offset(-16);
  }
  else
  {
    ST_RF_API_set_pa(0);
    ST_RF_API_set_rssi_offset(0);
  }
  
  /* Initialize the blue push button on the board, If at the start-up the button 
  will be pressed, the board will be configured with public key*/

  /*For Nucleo*/ 
  #if  !(defined(BLUENRG2_DEVICE) || defined(BLUENRG1_DEVICE))
    SdkEvalPushButtonInit(BUTTON_KEY, BUTTON_MODE_GPIO);

//    if(!SdkEvalPushButtonGetState(BUTTON_KEY))
//    {
//      use_public_key=1;
//      LedBlink(1);
//      
//      while(!SdkEvalPushButtonGetState(BUTTON_KEY));
//      LedBlink(1);
//    }
  #else /*For IDB00X*/  
    S2LPEvalPushButtonInit(BUTTON_KEY);
    if(!S2LPEvalPushButtonGetState(BUTTON_KEY))
    {
      use_public_key=1;
      LedBlink(1);
      
      while(!S2LPEvalPushButtonGetState(BUTTON_KEY));
      LedBlink(1);
    }
  #endif
 
/*For Nucleo*/  
#if  !(defined(BLUENRG2_DEVICE) || defined(BLUENRG1_DEVICE))
  /* The low level driver uses the internal RTC as a timer while the STM32 is in low power.
  This function calibrates the RTC using an auxiliary general purpose timer in order to 
  increase its precision. */
	if(timer_calibrated == 0)
	{
		ST_MCU_API_TimerCalibration(500);
		timer_calibrated = 1;
  }
  /* Initialize the blue push button on the board as an interrupt */
  SdkEvalPushButtonInit(BUTTON_KEY,BUTTON_MODE_EXTI);
  
#else  /*For IDB00X*/  
   /* Initialize push button 2 on the board as an interrupt */
  S2LPEvalPushButtonIrq(BUTTON_KEY, IRQ_ON_RISING_EDGE);
#endif
  

  /* Here it is necessary to understand if we are using the code for ETSI or FCC */
#if defined(FOR_ETSI)
  /* In case of ETSI we should stuck if RCZ is not 1 */
  if(rcz!=1)
    Fatal_Error();
#elif defined(FOR_FCC)
  if(!(rcz==2 || rcz==4))
    Fatal_Error();
#elif defined(FOR_ARIB)
  if(rcz!=3)
    Fatal_Error();
#endif
  
  if(use_public_key)
  {
    enc_utils_set_public_key(1);
  }	
  
  switch(rcz)
  {
  case 1:
    {
      /* RCZ1 - open the SigFox library */
			

      if(SIGFOX_API_open(&(sfx_rc_t)RC1)!=0)
      {
	/* Stuck in case of error */
	Fatal_Error();
      }
      break;	
    }
    
  case 2:
    {
      /* RCZ2 - open the SigFox library */
      if(SIGFOX_API_open(&(sfx_rc_t)RC2)!=0)
      {
	/* Stuck in case of error */
	Fatal_Error();
      }
      
      /* In FCC we can choose the macro channel to use by a 86 bits bitmask
      In this case we use the first 9 macro channels */
      sfx_u32 config_words[3]={1,0,0};
      
      /* Set the standard configuration */
      if(SIGFOX_API_set_std_config(config_words,1)!=0)
      {
	/* Stuck in case of error */
	Fatal_Error();
      }  
      break;
    }
    
  case 3:
    {
      /* RCZ3 - open the SigFox library */
      if(SIGFOX_API_open(&(sfx_rc_t)RC3C)!=0)
      {
	/* Stuck in case of error */
	Fatal_Error();
      }
      
      /* In ARIB we can choose the  */
      sfx_u32 config_words[3]=RC3C_CONFIG;
      
      if(SIGFOX_API_set_std_config(config_words,0)!=0)
      {
	/* Stuck in case of error */
	Fatal_Error();
      }
      break;
    }
    
  case 4:
    {
      /* RCZ4 - open the SigFox library */
      if(SIGFOX_API_open(&(sfx_rc_t)RC4)!=0)
      {
	/* Stuck in case of error */
	Fatal_Error();
      }
      
      /* In FCC we can choose the macro channel to use by a 86 bits bitmask
      In this case we use 9 consecutive macro channels starting from 63 (920.8MHz) */
      sfx_u32 config_words[3]={0,0x40000000,0};
      
      /* Set the standard configuration */
      if(SIGFOX_API_set_std_config(config_words,1)!=0)
      {
	/* Stuck in case of error */
	Fatal_Error();
      }
      break;
    }
  }
}

/* --------------------------------------------------------------------------*/
void HAL_Delay(__IO uint32_t Delay)
{
//	while(Delay > 1000)
//	{
//		SdkDelayMs(1000);
//		#ifdef INCLUDE_WWDG
//		HAL_WWDG_Refresh(&hwwdg);
//		#endif
//		Delay -= 1000;
//	}
	SdkDelayMs(Delay);
//		#ifdef INCLUDE_WWDG
//		HAL_WWDG_Refresh(&hwwdg);
//		#endif
  
}
/* --------------------------------------------------------------------------*/
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
/* --------------------------------------------------------------------------*/
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(huart->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */

  }
	#ifdef INCLUDE_DEBUG_UART
	else if(huart->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();
  
    /**USART2 GPIO Configuration    
    PA3     ------> USART2_RX
    PA14     ------> USART2_TX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		/* USER CODE BEGIN USART2_MspInit 1 */

		/* USER CODE END USART2_MspInit 1 */
  }
	#endif
}

/* --------------------------------------------------------------------------*/
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}
/* --------------------------------------------------------------------------*/

static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x2000090E;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Analogue filter 
    */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Digital filter 
    */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}
/* --------------------------------------------------------------------------*/
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(hi2c->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */
  
    /**I2C1 GPIO Configuration    
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
    /* I2C1 interrupt Init */
    HAL_NVIC_SetPriority(I2C1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C1_IRQn);
  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
  }

}

/* My Code ------------------------------------------------------------------*/
#ifdef USE_FLASH
#include <string.h>
#endif


/* a flag to understand if the button has been pressed */
static volatile uint8_t but_pressed=0; 

/**
* @brief  Blink the LED indefinitely stucking the application.
* @param  None
* @retval None
*/
void Fatal_Error(void)
{
  /* FOR NUCLEO */
  #if  !(defined(BLUENRG2_DEVICE) || defined(BLUENRG1_DEVICE))
    SdkEvalLedInit(LED1);
  #else /*For IDB00X*/  
    SdkEvalLedInit(LED2);
  #endif
  while(1)
  {
    SdkDelayMs(100);
    /* FOR NUCLEO */
    #if  !(defined(BLUENRG2_DEVICE) || defined(BLUENRG1_DEVICE))
      SdkEvalLedToggle(LED1);
    #else /*For IDB00X*/  
      SdkEvalLedToggle(LED2);
    #endif
  }
}

void Appli_Exti_CB(uint16_t GPIO_Pin)
{
#if  !(defined(BLUENRG2_DEVICE) || defined(BLUENRG1_DEVICE))
  /* In this case the application EXTI event is the button pressed */
  if(GPIO_Pin==GPIO_PIN_13)
  {
    /* set the button pressed flag */
    but_pressed=1;
  }
#else
  if(GPIO_Pin==5)
  {
    /* set the button pressed flag */
    but_pressed=1;
  }
#endif  
}

/**
* @brief  Configure all the GPIOs in low power mode.
* @param  None
* @retval None
*/
void enterGpioLowPower(void)
{
#if  !(defined(BLUENRG2_DEVICE) || defined(BLUENRG1_DEVICE))
  
  /* Set all the GPIOs in low power mode (input analog) */
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStructure.Pin = GPIO_PIN_All;
  
  GPIO_InitStructure.Pin = GPIO_PIN_All;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pin = GPIO_PIN_All;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pin = GPIO_PIN_All;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* de init the SPI */
  SdkEvalSpiDeinit();
  
  /* keep the EXTI on the button pin */
  SdkEvalPushButtonInit(BUTTON_KEY,BUTTON_MODE_EXTI);
  
  /* keep the SDN driven */
  SdkEvalM2SGpioInit(M2S_GPIO_SDN,M2S_MODE_GPIO_OUT);
  
  /* Be sure that it is driving the device to be in shutdown*/
  SdkEvalEnterShutdown();
#endif  
}

/**
* @brief  Configure all the GPIOs to be used by the application.
* @param  None
* @retval None
*/
void exitGpioLowPower(void)
{ 
#if  !(defined(BLUENRG2_DEVICE) || defined(BLUENRG1_DEVICE))
  /* Reinit the SDN pin and SPI */
  SdkEvalM2SGpioInit(M2S_GPIO_SDN,M2S_MODE_GPIO_OUT);
  SdkEvalSpiInit();
  EepromCsPinInitialization();
#endif
}

/**
* @brief  This function is used by the S2LPManagementIdentificationRFBoard to set the xtal frequency.
* @param  xtal_frequency frequency of the oscillator.
* @retval None
*/
void S2LPRadioSetXtalFrequency(uint32_t xtal_frequency)
{
  ST_RF_API_set_xtal_freq(xtal_frequency);
}

/**
* @brief  Blink the application led.
* @param  times Number of toggles.
* @retval None
*/
void LedBlink(uint8_t times)
{
  /* FOR NUCLEO */
  #if  !(defined(BLUENRG2_DEVICE) || defined(BLUENRG1_DEVICE))
	if(tracker_data.led_active == 1)
	{
    SdkEvalLedInit(LED1);
	}
  #else /*For IDB00X*/  
    SdkEvalLedInit(LED2);
  #endif
  for(uint8_t i=0;i<times;i++)
  {
    /* FOR NUCLEO */
    #if  !(defined(BLUENRG2_DEVICE) || defined(BLUENRG1_DEVICE))
      SdkEvalLedToggle(LED1);
    #else /*For IDB00X*/  
      SdkEvalLedToggle(LED2);
    #endif    
    SdkDelayMs(50);
  }
}

/**
* @brief  System main function.
* @param  None
* @retval None
*/
//int wwdg_delay = 0, pclk;
int main(void)
{  
  /* Instance some variables where the SigFox data of the board are stored */	
  
  /* Some variables to store the application data to transmit */

//	RTC_TimeTypeDef myTime;
//	RTC_DateTypeDef myDate;
	time_t nowtimestamp = 0;
	UNUSED(nowtimestamp);
//	HAL_NVIC_DisableIRQ(EXTI0_1_IRQn);
	mcu_system_init();
	MX_RTC_Init(1);
//Tracker data Default value-----------------------------------------------//
	tracker_data.gps_mode 					= 0;
	tracker_data.downlink_freq 			= 2;
	tracker_data.time_ref 					= 1;
	tracker_data.uplink_freq 				= 24;
	tracker_data.gps_timeout 				= 0x78;
	tracker_data.acc_sensitivity 		= 0x2F;
	tracker_data.no_movement_time 	= 90;
	tracker_data.gps_power_mode 		= 0;
	tracker_data.acc_mode 					= 1;
	tracker_data.force_transition 	= 0;
	tracker_data.led_active 				= 1;
	tracker_data.scheduled_hour 		= 5;
//Tracker data Default value-----------------------------------------------//	
//	mcu_read_time();
	rst_state = *(uint32_t*)RST_STATE_ADDRESS;
	
	if(rst_state != 0x00)
	{
		s_state = rst_state;
	}
//	if(tracker_data.gps_timeout == 0)
//	{
//		eeprom_load_para(&tracker_data);
//	}
	
	
	
	MX_GPIO_Init();
	
	MX_RTC_Init(0);

	MX_I2C1_Init();

	accel_sleep();
//	accel_init();
	
	MX_ADC_Init();

	MX_USART1_UART_Init();
	#ifdef INCLUDE_DEBUG_UART
	MX_USART2_UART_Init();
	#endif
	MX_TIM6_Init();
//	htim2.Instance = TIM2;
//	SdkEvalTimersTimConfig(&htim2, 8000, 1000);
//	TIM2->DIER |= TIM_DIER_UIE;
//	gps_en(ON);
//	while(1)
//	{
//		if(gps_fixed == 1)
//		{
//			struct tm *gps_time_temp;
//			gps_get_time_date(&mytime);
//			mytime.tm_year += 100;
//			gps_timestamp = mktime(&mytime) + UTC_OFFSET*60*60;
//			gps_time_temp = localtime(&gps_timestamp);
//			
//			set_time(gps_time_temp);
//			
//			
//			
//			alarm_a_set(2*3600);
//		}
//	}
//	sigfox_library_setup();	

//	ST_RF_API_reduce_output_power(SFX_REDUCED_POWER);        // 0x00 - 0x7f
//	SdkEvalExitShutdown();
//	rf_en(ON);
//	sigfox_setup_uldl();
//	SIGFOX_API_close();
//	MX_RTC_Init(0);
//	stop_mode_enter(1);
//	HAL_RTC_DeInit(&hrtc);
	
//	mcu_system_init();

	/* My Code ------------------------------------------------------------------*/  	

  gps_en(OFF, 0);

	tracker_data.product_ver = *(uint32_t*)PRODUCT_VERSION_ADDRESS; 

//	if(tracker_data.product_ver == 0x00)
//	{
//		s_state = S_BOOT;
//	}


//	sigfox_library_setup();
//	rf_en(ON);
//	ST_RF_API_reduce_output_power(cw_power);        // 0x00 - 0x7f
//	SIGFOX_API_close();
//	SIGFOX_API_start_continuous_transmission(RC1_OPEN_UPLINK_CENTER_FREQUENCY, SFX_NO_MODULATION);
//	while(1);
//	accel_init();
//	stop_mode_enter(1);
//	while(1)
//	SIGFOX_API_close();

//	{
//		SIGFOX_API_stop_continuous_transmission();
//		ST_RF_API_reduce_output_power(cw_power);        // 0x00 - 0x7f
//		SIGFOX_API_start_continuous_transmission(RC1_OPEN_UPLINK_CENTER_FREQUENCY, SFX_NO_MODULATION);
//	}
//	if(HAL_GPIO_ReadPin(RS_INT_GPIO_Port, RS_INT_Pin))
//	{
//		starttime = HAL_GetTick();
//		while(HAL_GPIO_ReadPin(RS_INT_GPIO_Port, RS_INT_Pin))
//		{
//			rs_time = HAL_GetTick() - starttime;
//			if(rs_time > 10000)
//			{
//				break;
//			}
//			led_en(ON, 1);
//			if((rs_time % 1000) == 0)
//			{
//				
//				HAL_Delay(100);
//				led_en(OFF, 1);
//			}

//		}
//		led_en(OFF,1);
//	}
//	
//	if(rs_time > 3000 && rs_time < 5000)
//	{
////		if(test_mode != 0x00)
////		{
////			test_mode = 0x00;
////			eeprom_save_para();
////			HAL_Delay(500);
////			uint8_t tx_test[4] = {0x11,0x11,0x11,0x11}; 
////			gps_en(OFF);
////			
////			SIGFOX_API_send_frame(tx_test,4,customer_resp,2,0);	
////			standby(0);
////			
////		}
//	}
//	else if(rs_time > 5000 && rs_time < 10000)
//	{
//		if(tracker_data.product_ver == 0x00)
//		{
//			tracker_data.info_code = 0x00;
//		}
//		else
//		{
//			tracker_data.info_code = 0x03;
//		}
//		
//		s_state = S_SETUP;

//	}
//	else if(rs_time > 10000)
//	{
//		s_state = S_BOOT;

//	}
//---------------------------------------Ubiscale Test----------------------------------------------------------------//
//	starttime = HAL_GetTick();
//	while(1)
//	{
//		nowtime = HAL_GetTick() - starttime;
//		ubi_count ++;
//		if(ubi_init_required)
//		{
//			gps_en(ON);
//			ubignss_init_after_gps_power_on(UBIGNSS_PAYLOAD_10BYTES, 1);
//			ubi_init_required = 0;
//		}
//	
//		ubi_state = ubignss_fix(ubi_payload, &ubi_time);
//		
//		if(ubi_count > 20000)
//		{
//			if(gps_uart.Init.BaudRate != 115200)
//			{   
//				ubi_init_required = 1;
//				ubi_count = 0;
//				gps_en(OFF);
////				gps_baud_set(115200);
//				HAL_Delay(100);
//			}
//		}

//		
//		if(ubi_state != 0)
//		{
//			led_en(ON, 1);
//			
//			mytimestampA = ubignss_get_time(ubi_time);
//			sigfox_ubi_uldl(0);
//			gps_en(OFF);
//			HAL_Delay(500);
//			NVIC_SystemReset();
//		}
//	}
//---------------------------------------Ubiscale Test----------------------------------------------------------------//	


//		sigfox_addon_test_logic();

		if(!accel_test())						//if accelerometer failed, the LED is on for 10 seconds
		{
			#ifdef INCLUDE_DEBUG_UART
			printf("Accelerometer Error");
			#endif
			led_en(ON, 1);
			delay_seconds(10); //HAL_Delay(10000);
			led_en(OFF, 1);
		}	
		
		HAL_Delay(500);
		gps_en(1, GPS_COLD_START_SEL);
		HAL_Delay(500);
		if(!gps_test())							//if MCU cannot decode the GPS data, the LED is on for 10 seconds
		{
			#ifdef INCLUDE_DEBUG_UART
			printf("GPS Error");
			#endif
			led_en(ON, 1);
			delay_seconds(10); // HAL_Delay(10000);
			led_en(OFF, 1);
			
		}
		
		gps_en(OFF, GPS_COLD_START_SEL);
		
		starttime = HAL_GetTick();	
//		pclk = HAL_RCC_GetPCLK1Freq();
		#ifdef INCLUDE_WWDG
		MX_WWDG_Init();
		#endif
//		while(1)
//		{
//			HAL_Delay(wwdg_delay);
//			HAL_WWDG_Refresh(&hwwdg);
//			led_en(TOG, 1);
//		}
		
  /* application main loop */
	
  while(1)
  {  
			nowtime = HAL_GetTick() - starttime;
			nowtimestamp = get_now_timestamp();
		
//			HAL_RTC_GetTime(&hrtc, &myTime, RTC_FORMAT_BIN);
//			HAL_RTC_GetDate(&hrtc, &myDate, RTC_FORMAT_BIN);
			HAL_RTC_GetAlarm(&hrtc, &sAlarmA, RTC_ALARM_A, RTC_FORMAT_BIN);		
			HAL_RTC_GetAlarm(&hrtc, &sAlarmB, RTC_ALARM_B, RTC_FORMAT_BIN);
			#ifdef INCLUDE_WWDG
			HAL_WWDG_Refresh(&hwwdg);
			#endif
			switch(s_state)
			{
				case S_TEST_MODE:
				{
					s_state = S_IDLE;
					#ifdef INCLUDE_SIGFOX_ADDON_TEST
					#ifdef INCLUDE_DEBUG_UART
					printf("TEST\r\n");
					#endif
					if(last_state != S_TEST_MODE)
					{
						last_state = S_TEST_MODE;
						test_mode = 0x00;
						
						if(!sigfox_testmode_dl())
						{
							s_state = S_DEEPSLEEP;
							break;
						}
					}
					if(test_mode != 0x00)
					{
						s_state = sigfox_rf_test(1);
						if(HAL_GPIO_ReadPin(RS_INT_GPIO_Port, RS_INT_Pin))
						{

							lSystickCounter = 0;
							starttime = HAL_GetTick();
							rs_time = 0x00;
							
							while(HAL_GPIO_ReadPin(RS_INT_GPIO_Port, RS_INT_Pin) && (rs_time < 3000))
							{
								rs_time = HAL_GetTick() - starttime;
							}
							
							if(rs_time >= 3000)
							{
								led_en(OFF, 1);
								SIGFOX_API_stop_continuous_transmission();
								test_mode = 0x00;
							}
						}
					}
					else
					{
						s_state = sigfox_rf_test(0);
					}
					#endif
					break;
				}
				case S_GPS_TEST:
				{
					s_state = S_IDLE;
//					printf("GPS TEST\r\n");
//					if(last_state != S_GPS_TEST)
//					{
//						gps_en(ON, tracker_data.gps_power_mode);
//						last_state = S_GPS_TEST;
//					}
//					if(gps_fixed == 1 || nowtime > tracker_data.gps_timeout*1000)
//					{
//						struct tm *gps_time_temp;
//						time_t gps_timestamp;
//						
//						gps_en(OFF, 0);

//						if(gps_fixed == 1)
//						{
//							gps_get_time_date(&mytime);
//							mytime.tm_year += 100;
//							gps_timestamp = mktime(&mytime) + UTC_OFFSET*60*60;
//							gps_time_temp = localtime(&gps_timestamp);
//							
//							set_time(gps_time_temp);
//							
//						}
//						
//						if(test_mode != 0x01)
//						{
//							HAL_UART_DeInit(&huart1);

//							
//							pstatus[0] = 0;
//							
//							if((get_now_timestamp() > mytimestampB || (time_different(get_now_timestamp(), mytimestampA) > 24*60*60*30))  && accel_int2_next_edge == INT_RISING_EDGE)
//							{
//								alarm_b_set_off();
//								sensor_data.no_movement = 1;
//								accel_init_required = 1;
//								accel_int2_next_edge = INT_FALLING_EDGE;
//							}
//							
//							sigfox_gps_uldl(0);
//							
//							
//							
//							NVIC_SystemReset();
//						}
//					}
					break;
				}
				case S_IDLE:
				{
					#ifdef INCLUDE_DEBUG_UART
					printf("IDLE\r\n");
					#endif
					lp_mode = S_IDLE;
					
					if(last_state != S_IDLE)
					{
						last_state = S_IDLE;
						MX_GPIO_Init();
						MX_RTC_Init(0);
						MX_I2C1_Init();
//						accel_sleep();
						MX_ADC_Init();
						MX_USART1_UART_Init();
					}
					wake_src = W_NONE;
					if(tracker_data.no_movement_time != 0)  		// Accel power mode control
					{
						if(accel_init_required)
						{
							accel_dur_set();
							accel_init_required = 0;
							sensor_data.no_movement = 0;
							alarm_b_set_off();
						}
					}
					else
					{
						alarm_b_set_off();
						accel_init_required = 0;
						sensor_data.no_movement = 0;
						accel_sleep();
					}
					HAL_Delay(100);
//					debug_set_midnight();
//					stop_mode_config();
					stop_mode_enter(1);
					MX_RTC_Init(0);

					mcu_system_init();
					
					s_state = S_WAKEUP;
					break;
				}
				case S_TRACKING:
				{
					#ifdef INCLUDE_DEBUG_UART
					printf("TRACKING\r\n");
					#endif
					struct tm *gps_time_temp;
					time_t gps_timestamp;
					
					if(last_state != S_TRACKING)
					{
						gps_en(ON, tracker_data.gps_power_mode);
						last_state = S_TRACKING;
					}

					if(tracker_data.gps_mode == UBIGNSS_ENABLE)
					{
						if(ubi_init_required)
						{
							gps_en(ON, tracker_data.gps_power_mode);
							ubi_state = 0;
							ubignss_init_after_gps_power_on(UBIGNSS_PAYLOAD_10BYTES, 1);
							ubi_init_required = 0;
						}
						
						ubi_count ++;
						ubi_state = ubignss_fix(ubi_payload, &ubi_time);
						if(ubi_count > 20000)												//Check the uart config during the tracking
						{
							if(gps_uart.Init.BaudRate != 115200)
							{
								ubi_init_required = 1;
								ubi_count = 0;
								gps_en(OFF, 0);
								HAL_UART_DeInit(&huart1);
								delay_seconds(1); //HAL_Delay(100);
							}
						}

						if((ubi_state != UBIGNSS_FIX_RUNNING) || (nowtime > tracker_data.gps_timeout*1000))
						{
							#ifdef INCLUDE_WWDG
							HAL_WWDG_Refresh(&hwwdg);
							#endif
							gps_en(OFF, 0);
							ubi_init_required = 1;
							ubi_count = 0;
							if(ubi_state == UBIGNSS_FIX_OK)
							{
								gps_timestamp = ubignss_get_time(ubi_time) + UTC_OFFSET*60*60;
								gps_time_temp = localtime(&gps_timestamp);
//								gps_timestamp = 1572479700;
//								gps_time_temp = localtime(&gps_timestamp);
								set_time(gps_time_temp);
							}
							
							HAL_UART_DeInit(&huart1);
							
							if((get_now_timestamp() > mytimestampB || (time_different(get_now_timestamp(), mytimestampA) > 24*60*60*30))  && accel_int2_next_edge == INT_RISING_EDGE)
							{
								alarm_b_set_off();
								sensor_data.no_movement = 1;
								accel_init_required = 1;
								accel_int2_next_edge = INT_FALLING_EDGE;
							}
							
							ul_counter ++;

							rst_state = 0x00;
							eeprom_save_para();
							
							
							if(ul_counter == tracker_data.downlink_freq)
							{
								s_state = S_SETUP;
							}
							else
							{
								sigfox_ubi_uldl(0);
								if(get_now_timestamp() > (mytimestampA - TIME_ERROR_BUFFER) || (time_different(get_now_timestamp(), mytimestampA) > 24*60*60*30))
								{
									alarm_a_set(ul_freq_to_sec(tracker_data.time_ref, tracker_data.uplink_freq));
								}
								
								nowtime = 0;
								
								s_state = S_IDLE;
							}
						}
					}
					else
					{
					if(gps_fixed == 1 || nowtime > tracker_data.gps_timeout*1000)
					{
						
						#ifdef INCLUDE_WWDG
						HAL_WWDG_Refresh(&hwwdg);
						#endif
						gps_en(OFF, 0);

						if(gps_fixed == 1)
						{
							gps_get_time_date(&mytime);
							mytime.tm_year += 100;
							gps_timestamp = mktime(&mytime) + UTC_OFFSET*60*60;
							gps_time_temp = localtime(&gps_timestamp);
							
							set_time(gps_time_temp);

						}
						
						if(test_mode != 0x01)
						{
							HAL_UART_DeInit(&huart1);
							
							pstatus[0] = 0;
							
							if((get_now_timestamp() > mytimestampB || (time_different(get_now_timestamp(), mytimestampA) > 24*60*60*30))  && accel_int2_next_edge == INT_RISING_EDGE)
							{
								alarm_b_set_off();
								sensor_data.no_movement = 1;
								accel_init_required = 1;
								accel_int2_next_edge = INT_FALLING_EDGE;
							}
							
							ul_counter ++;

							rst_state = 0x00;
							eeprom_save_para();

							if(ul_counter == tracker_data.downlink_freq)
							{
								s_state = S_SETUP;
							}
							else
							{
								sigfox_gps_uldl(0);
								if(get_now_timestamp() > (mytimestampA - TIME_ERROR_BUFFER) || (time_different(get_now_timestamp(), mytimestampA) > 24*60*60*30))
								{
									alarm_a_set(ul_freq_to_sec(tracker_data.time_ref, tracker_data.uplink_freq));
								}
								
								nowtime = 0;
								
								s_state = S_IDLE;
							}
							rtc_int = 0;
						}
					}
				}
					break;
				}
				case S_BOOT:
				{
					#ifdef INCLUDE_DEBUG_UART
					printf("BOOT\r\n");
					#endif
					if(last_state != S_BOOT)
					{
						last_state = S_BOOT;
					}
					boot_flag = 1;

					
					if(rst_state != S_BOOT)
					{
						rst_state = S_BOOT;
						eeprom_save_para();
						NVIC_SystemReset();
					}								
					
					rs_time = 0x00;
					gps_en(OFF, 0);

					if(sigfox_boot_dl() == 0x00)
					{
						ul_counter = 0x00;
						led_en(ON, 1);
						delay_seconds(5); //HAL_Delay(5000);
						led_en(OFF,1);
					}
					#ifdef INCLUDE_WWDG
					HAL_WWDG_Refresh(&hwwdg);
					#endif
					rst_state = 0x00;
					eeprom_save_para();
					s_state = S_DEEPSLEEP;

					break;
				}
				case S_SETUP:
				{
					#ifdef INCLUDE_DEBUG_UART
					printf("SETUP\r\n");
					#endif
					if(last_state != S_SETUP)
					{
						last_state = S_SETUP;
					}
					eeprom_load_para(&tracker_data);

					rs_time = 0x00;

					gps_en(OFF, 0);
					if(ul_counter == tracker_data.downlink_freq)
					{
						sfx_u8 s_dl_err;
			
						if(tracker_data.gps_mode == UBIGNSS_ENABLE)
						{
							s_dl_err = sigfox_ubi_uldl(1);
						}
						else
						{
							s_dl_err = sigfox_gps_uldl(1);
						}
						if(s_dl_err == 0x00)
						{
							led_en(ON, tracker_data.led_active);
							delay_seconds(5); // HAL_Delay(5000);				//if the downlink frame is received, the LED is on for 5 secs
							led_en(OFF,1);
						
							sigfox_setup_uldl(0);			//UL SETUP frame as an ACK
						}
						nowtime = 0;
					}
					else
					{
						if(sigfox_setup_uldl(1) == 0x00)
						{
							led_en(ON, tracker_data.led_active);
							delay_seconds(5); // HAL_Delay(5000);
							led_en(OFF,1);
						}
					}
					if(get_now_timestamp() > (mytimestampA - TIME_ERROR_BUFFER) || lp_mode == S_DEEPSLEEP || ul_counter == tracker_data.downlink_freq)
					{
						alarm_a_set(ul_freq_to_sec(tracker_data.time_ref, tracker_data.uplink_freq));
					}
					#ifdef INCLUDE_WWDG
					HAL_WWDG_Refresh(&hwwdg);
					#endif
					ul_counter = 0x00;
					rst_state = 0x00;
					eeprom_save_para();			
					s_state = S_IDLE;
					break;
				}
				case S_DEEPSLEEP:
				{
					#ifdef INCLUDE_DEBUG_UART
					printf("DEEPSLEEP\r\n");
					#endif
					if(last_state != S_DEEPSLEEP)
					{
						last_state = S_DEEPSLEEP;
						MX_GPIO_Init();
						MX_RTC_Init(0);
						MX_I2C1_Init();
						accel_sleep();
//						MX_ADC_Init();
						MX_USART1_UART_Init();
					}
					lp_mode = S_DEEPSLEEP;
					wake_src = 0x00;
					if(tracker_data.force_transition == 1)
					{
						s_state = S_SETUP;
						
					}
					else
					{
						alarm_a_set(7*24*60*60);   // one day rtc alarm
						mid_night = 0;						//	avoid mid-night process by DeepSleep RTC
						stop_mode_enter(1);
						mcu_system_init();
						
						s_state = S_WAKEUP;
					}
					
					
					break;
				}
				case S_WAKEUP:
				{
					#ifdef INCLUDE_DEBUG_UART
					printf("WAKEUP\r\n");
					#endif
					boot_flag = 0;

					MX_RTC_Init(0);
					MX_ADC_Init();
					MX_GPIO_Init();
					MX_USART1_UART_Init();
					MX_I2C1_Init();					
					MX_TIM6_Init();

					lSystickCounter = 0;
					SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;


					starttime = HAL_GetTick();
					
					#ifdef INCLUDE_WWDG
					MX_WWDG_Init();
					#endif
					if(wake_src == W_RS)
					{
						if(HAL_GPIO_ReadPin(RS_INT_GPIO_Port, RS_INT_Pin))
						{
							rs_time = 0x00;
				//			starttime = nowtime;
							nowtime = HAL_GetTick();
							while(HAL_GPIO_ReadPin(RS_INT_GPIO_Port, RS_INT_Pin) && (rs_time < 20000))
							{
								#ifdef INCLUDE_WWDG
								HAL_WWDG_Refresh(&hwwdg);
								#endif
								rs_time = HAL_GetTick() - nowtime;
								if(rs_time > 3000 && rs_time < 3500)										//first blink => continuous wave test mode
								{
									led_en(OFF, 1);
								}
								else if(rs_time > 5000 && rs_time < 5500)								//second blink => SETUP mode
								{
									led_en(OFF, 1);
								}
								else if(rs_time > 10000 && rs_time < 10500)							//third blink => BOOT mode
								{
									led_en(OFF, 1);
								}
								else
								{
									led_en(ON, 1);
								}
							}
							led_en(OFF, tracker_data.led_active);
						}

						
						if((rs_time > 5000) && (rs_time < 10000))
						{
							if(lp_mode == S_DEEPSLEEP)
							{
								s_state = S_SETUP;
							}
							else
							{
								s_state = lp_mode;
							}
						}
						else if((rs_time > 3000) && (rs_time < 5000))
						{
							if(lp_mode == S_DEEPSLEEP)
							{
								s_state = S_TEST_MODE;
							}
							else
							{
								s_state = lp_mode;
							}
						}
						else if((rs_time > 10000) && (rs_time < 20000))
						{
							s_state = S_BOOT;
						}
						else
						{
							if(lp_mode == S_DEEPSLEEP)
							{
								s_state = S_DEEPSLEEP;
							}
							else
							{
								s_state = S_IDLE;
							}
						}
						break;
					}
					
//					HAL_Delay(60000);
					if(rtc_int == W_RTC_A)
					{
						if(mid_night)
						{
							uint32_t mid_delay, scheduled_sec;
							mid_delay = tracker_data.scheduled_hour * 3600;
							scheduled_sec = ul_freq_to_sec(tracker_data.time_ref, tracker_data.uplink_freq);

							while(mid_delay > scheduled_sec) 
							{
								mid_delay -= scheduled_sec;
							}
							mid_night = 0;
							alarm_a_set(mid_delay);
							s_state = S_IDLE;
							break;
						}
						else
						{
							if(lp_mode == S_IDLE)
							{
								alarm_a_set(ul_freq_to_sec(tracker_data.time_ref, tracker_data.uplink_freq));
								s_state = S_TRACKING;
							}
							else if(lp_mode == S_DEEPSLEEP)
							{
								s_state = S_BOOT;
							}
							break;
						}
					}
					else if(rtc_int == W_RTC_B)
					{
						sensor_data.no_movement = 1;
						gps_en(ON, tracker_data.gps_power_mode);

						s_state = S_TRACKING;
						accel_init_required = 1;
						break;
					}

					
					if(wake_src == W_ACCEL)
					{
						HAL_Delay(100);
						move_flag = HAL_GPIO_ReadPin(ACCEL_INT2_GPIO_Port, ACCEL_INT2_Pin);
						
						if(move_flag)
						{
							//rising edge
							sensor_data.no_movement = 0;
							accel_int2_next_edge = INT_FALLING_EDGE;
							alarm_b_set_off();
							accel_init_required = 1;
							s_state = S_IDLE;
							
						}
						else if(!move_flag)
						{
							//falling edge
							if(tracker_data.no_movement_time < ACCEL_TIMER_MAX_MINS + 1)						//Set Accelerometer counter only when no movement time = {0,1,2,...33,34} mins
							{
								accel_int2_next_edge = INT_RISING_EDGE; 
								sensor_data.no_movement = 1;
								MX_USART1_UART_Init();
								MX_I2C1_Init();
								gps_en(ON, tracker_data.gps_power_mode);
								accel_init_required = 1;
								
								s_state = S_TRACKING;
							}
							else
							{
//								nomove_counter = 1;
								accel_int2_next_edge = INT_RISING_EDGE; 
								accel_init_required = 0;
								alarm_b_set();
								s_state = S_IDLE;
							}
						}
						mcu_accel_int2_setting(accel_int2_next_edge);
						break;
					}
					else
					{
						s_state = lp_mode;
							
							
						break;
					}
				}
			}
  }
}

void ST_dbg_CB(const char *vectcStr,...){}
void ST_MANUF_report_CB(uint8_t status, int32_t rssi){}

#ifdef  USE_FULL_ASSERT
/**
* @brief  Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param file: pointer to the source file name
* @param line: assert_param error line source number
* @retval : None
*/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number */
  //printf("Wrong parameters value: file %s on line %d\r\n", file, line);
  
  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/******************* (C) COPYRIGHT 2018 STMicroelectronics *****END OF FILE****/
