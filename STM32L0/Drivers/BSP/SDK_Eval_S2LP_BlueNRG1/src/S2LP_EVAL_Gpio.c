/**
 * @file    S2LP_EVAL_Gpio.c
 * @author  VMA division - AMS
 * @version 3.2.0
 * @date    May 1, 2016
 * @brief   This file provides all the low level API to manage SDK eval pin to drive GPIOs.
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
 * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
 *
 */

/* Includes ------------------------------------------------------------------*/
#include "S2LP_EVAL_Config.h"
#include "S2LP_EVAL_Gpio.h"
#include "BlueNRG1_conf.h"
#include "hal_types.h"
#include "BlueNRG-1_S2LP_pinmap.h"

//Addedd after Mantis Entry #2574
uint16_t S2LPEvalGpioGetPin(M2SGpioPin xGpio);
/** @addtogroup SDK_EVAL_S2LP         SDK EVAL S2LP
 * @{
 */


/** @addtogroup S2LP_EVAL_Gpio
 * @{
 */



/** @defgroup S2LP_EVAL_Gpio_Private_Variables                    S2LP EVAL Gpio Private Variables
 * @{
 */


/**
 * @brief  M2S GPio Pin array
 */
static const uint32_t s_vectnM2SGpioPin[S2LP_N_PLATFORMS][M2S_GPIO_NUMBER] = {
  {IDB007_M2S_GPIO_0_PIN,IDB007_M2S_GPIO_1_PIN,IDB007_M2S_GPIO_2_PIN,IDB007_M2S_GPIO_3_PIN,IDB007_M2S_GPIO_SDN_PIN},
  {JORJIN_M2S_GPIO_0_PIN,JORJIN_M2S_GPIO_1_PIN,JORJIN_M2S_GPIO_2_PIN,JORJIN_M2S_GPIO_3_PIN,JORJIN_M2S_GPIO_SDN_PIN},
  {MONARCH_M2S_GPIO_0_PIN,MONARCH_M2S_GPIO_1_PIN,MONARCH_M2S_GPIO_2_PIN,MONARCH_M2S_GPIO_3_PIN,MONARCH_M2S_GPIO_SDN_PIN}
};

static const uint32_t s_vectnExtPaGpioPin[3] = {MONARCH_SKY66420_CPS_PIN, MONARCH_SKY66420_CSD_PIN, MONARCH_SKY66420_CTX_PIN};




/**
 * @}
 */





/** @defgroup S2LP_EVAL_Gpio_Private_Functions                            S2LP EVAL Gpio Private Functions
 * @{
 */

/**
 * @brief  Configures MCU GPIO and EXTI Line for GPIOs.
 * @param  xGpio Specifies the GPIO to be configured.
 *         This parameter can be one of following parameters:
 *         @arg M2S_GPIO_0: GPIO_0
 *         @arg M2S_GPIO_1: GPIO_1
 *         @arg M2S_GPIO_2: GPIO_2
 *         @arg M2S_GPIO_3: GPIO_3
 *         @arg M2S_GPIO_SDN: GPIO_SDN
 * @param  xGpioMode Specifies GPIO mode.
 *         This parameter can be one of following parameters:
 *         @arg M2S_MODE_GPIO_IN: MCU GPIO will be used as simple input.
 *         @arg M2S_MODE_EXTI_IN: MCU GPIO will be connected to EXTI line with interrupt
 *         generation capability.
 *         @arg M2S_MODE_GPIO_OUT: MCU GPIO will be used as simple output.
 * @retval None.
 */
void S2LPEvalGpioInit(M2SGpioPin xGpio, M2SGpioMode xGpioMode)
{
  GPIO_InitType GPIO_InitStructure;
  SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_GPIO, ENABLE);

 
  /* Configures Button pin as input */
  if ((uint8_t)xGpio > 4)
    GPIO_InitStructure.GPIO_Pin = s_vectnExtPaGpioPin[xGpio-5];
  else
    GPIO_InitStructure.GPIO_Pin = s_vectnM2SGpioPin[S2LPEvalGetVersion()][xGpio];
  
  if(xGpioMode==M2S_MODE_GPIO_OUT)
    GPIO_InitStructure.GPIO_Mode = GPIO_Output;
  else
    GPIO_InitStructure.GPIO_Mode = GPIO_Input;
    
  GPIO_InitStructure.GPIO_Pull = DISABLE;
  GPIO_InitStructure.GPIO_HighPwr = DISABLE;
  GPIO_Init(&GPIO_InitStructure);
  
  if (xGpioMode == M2S_MODE_EXTI_IN) 
  {
    GPIO_EXTIConfigType GPIO_EXTIStructure;
    
    /* Configures EXTI line */
    if ((uint8_t)xGpio > 4)
      GPIO_EXTIStructure.GPIO_Pin = s_vectnExtPaGpioPin[xGpio-5];
    else 
      GPIO_EXTIStructure.GPIO_Pin = s_vectnM2SGpioPin[S2LPEvalGetVersion()][xGpio];
 
    GPIO_EXTIStructure.GPIO_IrqSense = GPIO_IrqSense_Edge;
    GPIO_EXTIStructure.GPIO_Event = 0; //0 means falling edge, 1 raising
    GPIO_EXTIConfig(&GPIO_EXTIStructure);
    
    /* Clear pending interrupt */
    if ((uint8_t)xGpio > 4)
      GPIO_ClearITPendingBit(s_vectnExtPaGpioPin[xGpio-5]);
    else
      GPIO_ClearITPendingBit(s_vectnM2SGpioPin[S2LPEvalGetVersion()][xGpio]);

    /* Enable the interrupt */
    if ((uint8_t)xGpio > 4)
      GPIO_EXTICmd(s_vectnExtPaGpioPin[xGpio-5], ENABLE);
    else
      GPIO_EXTICmd(s_vectnM2SGpioPin[S2LPEvalGetVersion()][xGpio], ENABLE);
  }
}

/**
 * @brief  Enables or disables the interrupt on GPIO .
 * @param  xGpio Specifies the GPIO whose priority shall be changed.
 *         This parameter can be one of following parameters:
 *         @arg M2S_GPIO_0: GPIO_0
 *         @arg M2S_GPIO_1: GPIO_1
 *         @arg M2S_GPIO_2: GPIO_2
 *         @arg M2S_GPIO_3: GPIO_3
 * @param  nPreemption Specifies Preemption Priority.
 * @param  nSubpriority Specifies Subgroup Priority. Not used in the BlueNRG-1.
 * @param  xNewState Specifies the State.
 *         This parameter can be one of following parameters:
 *         @arg ENABLE: Interrupt is enabled
 *         @arg DISABLE: Interrupt is disabled
 * @retval None.
 */
void S2LPEvalGpioInterruptCmd(M2SGpioPin xGpio, uint8_t nPreemption, uint8_t nSubpriority, FunctionalState xNewState)
{  
  if (xNewState==ENABLE)
  {
    NVIC_InitType NVIC_InitStructure;
    /* Set the GPIO interrupt priority and enable it */
    NVIC_InitStructure.NVIC_IRQChannel = GPIO_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = LOW_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
  }
  
  GPIO_EXTICmd(S2LPEvalGpioGetPin(xGpio), xNewState);
}

/**
 * @brief  Enables or disables trigger on rising edge for that GPIO .
 * @param  xGpio Specifies the GPIO.
 *         This parameter can be one of following parameters:
 *         @arg M2S_GPIO_0: GPIO_0
 *         @arg M2S_GPIO_1: GPIO_1
 *         @arg M2S_GPIO_2: GPIO_2
 *         @arg M2S_GPIO_3: GPIO_3
 * @param  xNewState Specifies the State.
 *         This parameter can be one of following parameters:
 *         @arg ENABLE: Rising trigger is enabled
 *         @arg DISABLE: Rising trigger is disabled
 * @retval None.
 */
void S2LPEvalGpioTriggerRising(M2SGpioPin xGpio, FunctionalState xNewState)
{
  GPIO_EXTIConfigType GPIO_EXTIStructure;
  
  if(xNewState)
  { 
    /* Configures EXTI line */

    GPIO_EXTIStructure.GPIO_Pin = s_vectnM2SGpioPin[S2LPEvalGetVersion()][xGpio];
    GPIO_EXTIStructure.GPIO_IrqSense = GPIO_IrqSense_Edge;
    GPIO_EXTIStructure.GPIO_Event = (uint8_t)1; //0 means falling edge, 1 raising
    GPIO_EXTIConfig(&GPIO_EXTIStructure);
  }
}

/**
 * @brief  To assert if the rising edge IRQ is enabled for that GPIO .
 * @param  xGpio Specifies the GPIO.
 *         This parameter can be one of following parameters:
 *         @arg M2S_GPIO_0: GPIO_0
 *         @arg M2S_GPIO_1: GPIO_1
 *         @arg M2S_GPIO_2: GPIO_2
 *         @arg M2S_GPIO_3: GPIO_3
 * @retval  Specifies the State.
 *         @arg ENABLE: Rising trigger is enabled
 *         @arg DISABLE: Rising trigger is disabled
 */
FunctionalState S2LPEvalM2SGpioGetTriggerRising(M2SGpioPin xGpio)
{
//  if(EXTI->RTSR & (uint16_t)s_vectnM2SGpioPin[xGpio])
//    return ENABLE;
//  
  return DISABLE;
}

/**
 * @brief  Enables or disables trigger on falling edge for that GPIO .
 * @param  xGpio Specifies the GPIO.
 *         This parameter can be one of following parameters:
 *         @arg M2S_GPIO_0: GPIO_0
 *         @arg M2S_GPIO_1: GPIO_1
 *         @arg M2S_GPIO_2: GPIO_2
 *         @arg M2S_GPIO_3: GPIO_3
 * @param  xNewState Specifies the State.
 *         This parameter can be one of following parameters:
 *         @arg ENABLE: Falling trigger is enabled
 *         @arg DISABLE: Falling trigger is disabled
 * @retval None.
 */
void S2LPEvalM2SGpioTriggerFalling(M2SGpioPin xGpio, FunctionalState xNewState)
{
  GPIO_EXTIConfigType GPIO_EXTIStructure;
  
  if(xNewState)
  {
    /* Configures EXTI line */
    GPIO_EXTIStructure.GPIO_Pin = s_vectnM2SGpioPin[S2LPEvalGetVersion()][xGpio];
    GPIO_EXTIStructure.GPIO_IrqSense = GPIO_IrqSense_Edge;
    GPIO_EXTIStructure.GPIO_Event = (uint8_t)0; //0 means falling edge, 1 raising
    
    GPIO_EXTIConfig(&GPIO_EXTIStructure);
  }
}

/**
 * @brief  To assert if the falling edge IRQ is enabled for that GPIO .
 * @param  xGpio Specifies the GPIO.
 *         This parameter can be one of following parameters:
 *         @arg M2S_GPIO_0: GPIO_0
 *         @arg M2S_GPIO_1: GPIO_1
 *         @arg M2S_GPIO_2: GPIO_2
 *         @arg M2S_GPIO_3: GPIO_3
 * @retval  Specifies the State.
 *         @arg ENABLE: Falling trigger is enabled
 *         @arg DISABLE: Falling trigger is disabled
 */
FunctionalState S2LPEvalM2SGpioGetTriggerFalling(M2SGpioPin xGpio)
{
//  if(EXTI->FTSR & (uint16_t)s_vectnM2SGpioPin[xGpio])
//    return ENABLE;
  
  return DISABLE;
}

/**
 * @brief  Returns the level of a specified GPIO.
 * @param  xGpio Specifies the GPIO to be read.
 *         This parameter can be one of following parameters:
 *         @arg M2S_GPIO_0: GPIO_0
 *         @arg M2S_GPIO_1: GPIO_1
 *         @arg M2S_GPIO_2: GPIO_2
 *         @arg M2S_GPIO_3: GPIO_3
 * @retval FlagStatus Level of the GPIO. This parameter can be:
 *         SET or RESET.
 */
FlagStatus S2LPEvalGpioGetLevel(M2SGpioPin xGpio)
{
  /* Gets the GPIO level */
  FlagStatus ret;
  ret = (FlagStatus) GPIO_ReadBit(GPIO_Pin_21);
  
  return ret;
}

/**
 * @brief  Sets the level of a specified GPIO.
 * @param  xGpio Specifies the GPIO to be set.
 *         This parameter can be one of following parameters:
 *         @arg M2S_GPIO_0: GPIO_0
 *         @arg M2S_GPIO_1: GPIO_1
 *         @arg M2S_GPIO_2: GPIO_2
 *         @arg M2S_GPIO_3: GPIO_3
 * @param  FlagStatus Level of the GPIO. This parameter can be:
 *         SET or RESET.
 * @retval None.
 */
void S2LPEvalGpioSetLevel(M2SGpioPin xGpio, FlagStatus xLevel)
{
  /* Sets the GPIO level */
  if ((uint8_t) xGpio>4)
    GPIO_WriteBit(s_vectnExtPaGpioPin[xGpio-5], (BitAction)xLevel);
  else
    GPIO_WriteBit(s_vectnM2SGpioPin[S2LPEvalGetVersion()][xGpio], (BitAction)xLevel);

}

/**
 * @brief  Puts at logic 1 the SDN pin.
 * @param  None.
 * @retval None.
 */
void S2LPEvalEnterShutdown(void)
{
  /* Puts high the GPIO connected to shutdown pin */
  GPIO_SetBits(s_vectnM2SGpioPin[S2LPEvalGetVersion()][M2S_GPIO_SDN]);
}


/**
 * @brief  Put at logic 0 the SDN pin.
 * @param  None.
 * @retval None.
 */
void S2LPEvalExitShutdown(void)
{
  /* Puts low the GPIO connected to shutdown pin */
  GPIO_ResetBits(s_vectnM2SGpioPin[S2LPEvalGetVersion()][M2S_GPIO_SDN]);

  /* Delay to allow the circuit POR */
  for(volatile uint32_t i=0;i<0x1E00;i++);
}

/**
 * @brief  check the logic(0 or 1) at the SDN pin.
 * @param  None.
 * @retval FlagStatus.
 */
FlagStatus S2LPEvalCheckShutdown(void)
{
  return  S2LPEvalGpioGetLevel(M2S_GPIO_SDN);
}

/**
 * @brief  Gets the GPIO_PIN of the M2SGpioPin.
 * @param  xGpio: M2S GPIO.
 * @retval uint16_t GPIO_PIN value.
 */
uint16_t S2LPEvalGpioGetPin(M2SGpioPin xGpio)
{
  if ((uint8_t)xGpio > 4)
    return s_vectnExtPaGpioPin[xGpio-5];
  else
    return s_vectnM2SGpioPin[S2LPEvalGetVersion()][xGpio];
}

  
/**************************************************************************/
//
//                      EXTERNAL PA MICRO MANAGEMENT
//
/**************************************************************************/
  
//To be called befor all functions reported below
void ExternalPaGpioInit(void)
{
  S2LPEvalGpioInit(M2PA_CPS, M2S_MODE_GPIO_OUT);
  S2LPEvalGpioInit(M2PA_CSD, M2S_MODE_GPIO_OUT);
  S2LPEvalGpioInit(M2PA_CTX, M2S_MODE_GPIO_OUT);
}


void ExternalPaTxBypass(void)
{
  S2LPEvalGpioSetLevel(M2PA_CPS, RESET);  //Bypass mode select 
  S2LPEvalGpioSetLevel(M2PA_CSD, SET);    //Turned on CSD=1
  S2LPEvalGpioSetLevel(M2PA_CTX, SET);    //TX state = 1
}

void ExternalPaTxActive(void)
{
  S2LPEvalGpioSetLevel(M2PA_CPS, SET);
  S2LPEvalGpioSetLevel(M2PA_CSD, SET);    //Turned on CSD=1 
  S2LPEvalGpioSetLevel(M2PA_CTX, SET);    //TX state = 1
}

void ExternalPaRx(void)
{
  S2LPEvalGpioSetLevel(M2PA_CPS, RESET);  //Don't care, but we have to assign a value
  S2LPEvalGpioSetLevel(M2PA_CSD, SET);    //Turned on CSD=1
  S2LPEvalGpioSetLevel(M2PA_CTX, RESET);  //TX state = 0
}

void ExternalPaShutdown(void)
{
  S2LPEvalGpioSetLevel(M2PA_CPS, RESET); //Don't care, but we have to assign a value
  S2LPEvalGpioSetLevel(M2PA_CSD, RESET); //Shutdown PIN=0
  S2LPEvalGpioSetLevel(M2PA_CTX, RESET); //Don't care, but we have to assign a value 
}


/**
 * @}
 */


/**
 * @}
 */


/**
 * @}
 */


/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
