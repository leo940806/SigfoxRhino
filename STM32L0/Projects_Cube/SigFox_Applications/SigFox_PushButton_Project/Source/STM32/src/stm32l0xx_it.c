/**
  ******************************************************************************
  * @file    stm32l1xx_it.c
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    21-March-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_it.h"
#include "cube_hal.h"

/** @addtogroup Template_Project
  * @{
  */
#include "gps.h"
#include "main.h"
#include "linkdata.h"
#include "libubignsscore.h"
extern uint16_t counter;
extern uint8_t firstinterrupt;
extern _Bool gps_fixed;


extern _Bool rs_trig, accel_init_required, accel_int2_next_edge;
extern uint32_t rs_time;
extern uint32_t starttime, nowtime;
extern uint8_t test_mode, wake_src;

extern sensordata_t sensor_data;
extern trackerparameter_t tracker_data;

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1;
extern RTC_HandleTypeDef hrtc;
extern TIM_HandleTypeDef htim6;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval : None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval : None
  */
extern _Bool ubi_init_required;
extern trackerparameter_t tracker_data;
extern uint8_t s_state, rst_state;
#define RST_STATE_ADDRESS												0x08080048
void HardFault_Handler(void)
{
//This Error Handler is created by ST MCU Driver. 
//Pauline mentioned about the bug in Ubiscale mode. 
//Possibly, the Ubiscale library assigned the invalid address to the uart/timer driver function that cause this error.	
//When this error occurs, the system will reset itself and do the SETUP instead of looping
  /* Go to infinite loop when Hard Fault exception occurs */
	while (1)
  {
		HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, 			RST_STATE_ADDRESS,				0x02);			 
		NVIC_SystemReset();
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval : None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval : None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval : None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval : None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval : None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval : None
  */
void PendSV_Handler(void)
{
}



/*******************************************************************************
* Function Name  : USB_LP_IRQHandler
* Description    : This function handles USB Low Priority interrupts  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//void USB_LP_IRQHandler(void)
//{
//  HAL_PCD_IRQHandler(&hpcd);
//}

void EXTI0_1_IRQHandler(void)
{
  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0) != RESET) 
  {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
		int i = 0;
		for(i = 0; i < 1000; i++);
		if(HAL_GPIO_ReadPin(RS_INT_GPIO_Port, RS_INT_Pin))
		{
//			if(rs_time > 2000)
//			{
//				rf_en(0);				
//				stop_mode_enter(1);
//				NVIC_SystemReset();
//			}
		}
		wake_src = W_RS;
}
  
  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_1) != RESET) 
  {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
  }
}

void EXTI2_3_IRQHandler(void)
{
  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_2) != RESET) 
  {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
  }
  
  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_3) != RESET) 
  {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
  }
}
/**
  * @brief  This function handles External External line 9 to 5 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI4_15_IRQHandler(void)
{
  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_4) != RESET) 
  {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
  }
  else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_5) != RESET) 
  {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
  }
  else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_6) != RESET) 
  {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
  }
  else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_7) != RESET) 
  {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
  }
  else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_8) != RESET) 
  {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
		
		if(HAL_GPIO_ReadPin(ACCEL_INT2_GPIO_Port, ACCEL_INT2_Pin))
		{
			wake_src = W_ACCEL;
			sensor_data.no_movement = 0;									//Rising
			accel_int2_next_edge = INT_FALLING_EDGE;
			accel_init_required = 1;
			
		}
		else
		{
			if(accel_init_required == 0)
			{
				wake_src = W_ACCEL;
				if(tracker_data.no_movement_time < 36)			//Falling
				{	
					sensor_data.no_movement = 1;
					accel_init_required = 1;
				}
				else
				{
					accel_int2_next_edge = INT_RISING_EDGE;
					alarm_b_set();
					accel_init_required = 0;
				}
			}
		}
		
  }
  else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_9) != RESET) 
  {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
  }
  
  else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_10) != RESET) 
  {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
  }
  else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_11) != RESET) 
  {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
  }
  else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_12) != RESET) 
  {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
  }
  else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_13) != RESET) 
  {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
  }
  else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_14) != RESET) 
  {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
  }
  else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_15) != RESET) 
  {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
  }

}

/**
* @brief This function handles TIM6 global interrupt and DAC1/DAC2 underrun error interrupts.
*/
void TIM6_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_DAC_IRQn 0 */

  /* USER CODE END TIM6_DAC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  /* USER CODE BEGIN TIM6_DAC_IRQn 1 */
	if(ubi_timer_callback_id != 0)
	{
		ubi_timer_callback_id();
//		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
	}
  /* USER CODE END TIM6_DAC_IRQn 1 */
}


void I2C1_IRQHandler(void)
{
  /* USER CODE BEGIN I2C1_IRQn 0 */

  /* USER CODE END I2C1_IRQn 0 */
  if (hi2c1.Instance->ISR & (I2C_FLAG_BERR | I2C_FLAG_ARLO | I2C_FLAG_OVR)) {
    HAL_I2C_ER_IRQHandler(&hi2c1);
  } else {
    HAL_I2C_EV_IRQHandler(&hi2c1);
  }
  /* USER CODE BEGIN I2C1_IRQn 1 */

  /* USER CODE END I2C1_IRQn 1 */
}

/**
* @brief This function handles USART2 global interrupt / USART2 wake-up interrupt through EXTI line 26.
*/
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART2_IRQn 1 */
	if(tracker_data.gps_mode == UBIGNSS_ENABLE)
	{
		if(ubi_uart_callback_id != 0)
		{
			ubi_uart_callback_id();
		}
	}
//	else
//	{
		gps_fixed = getfix();
		gps_decode();
//	}
  /* USER CODE END USART2_IRQn 1 */
}

/**
  * @brief This function handles Window watchdog interrupt.
  */
#ifdef INCLUDE_WWDG
extern WWDG_HandleTypeDef hwwdg;
void WWDG_IRQHandler(void)
{
  /* USER CODE BEGIN WWDG_IRQn 0 */

  /* USER CODE END WWDG_IRQn 0 */
  HAL_WWDG_IRQHandler(&hwwdg);
  /* USER CODE BEGIN WWDG_IRQn 1 */
//	HAL_WWDG_Refresh(&hwwdg);

  /* USER CODE END WWDG_IRQn 1 */
}
#endif

/**
 @brief This function handles RTC global interrupt through EXTI lines 17, 19 and 20 and LSE CSS interrupt through EXTI line 19. */
///**/
//void RTC_IRQHandler(void)
//{
//  /* USER CODE BEGIN RTC_IRQn 0 */

//  /* USER CODE END RTC_IRQn 0 */
//  HAL_RTC_AlarmIRQHandler(&hrtc);
//  /* USER CODE BEGIN RTC_IRQn 1 */

//  /* USER CODE END RTC_IRQn 1 */
//}




/******************************************************************************/
/*                 STM32L15x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32l1xx_lp.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval : None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */


/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
