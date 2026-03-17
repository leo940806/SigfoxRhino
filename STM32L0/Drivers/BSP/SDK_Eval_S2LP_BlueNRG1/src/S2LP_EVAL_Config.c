/**
* @file    S2LP_EVAL_Config.c
* @author  VMA division - AMS
* @version 3.2.0
* @date    May 1, 2016
* @brief   S2LP EVAL EEPROM management
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
*/  

/* Includes ------------------------------------------------------------------*/
#include "S2LP_EVAL_Config.h"
#include "SDK_EVAL_Config.h"
#include "S2LP_EVAL_SPI.h"
#include "S2LP_EVAL_Gpio.h"
#include "S2LP_EVAL_EEPROM.h"
#include "clock.h"

/**
* @addtogroup S2LP_EVAL_NUCLEO
* @{
*/


/**
* @defgroup S2LP_EVAL_Config              S2LP Config
* @{
*/


/** @defgroup S2LP_EVAL_Config               S2LP Config Private Variable
 * @{
 */

/**
 * @brief  SPI buffers used for DMA application
 */


static volatile uint8_t s_S2LPEvalVersion = S2LP_EVAL_IDB007;
/**
 * @}
 */


/** @defgroup S2LP_EVAL_Config_Functions
* @{
*/


/**
* @brief  Identifies the current motherboard.
* @param  None.
* @retval None.
*/
void S2LPEvalIdentification(void)
{
  uint8_t version,header;
  
  for(uint8_t i=0;i<S2LP_N_PLATFORMS;i++)
  {
    S2LPEvalSetVersion(i);
    S2LPEvalGpioInit(M2S_GPIO_SDN,M2S_MODE_GPIO_OUT);
    S2LPEvalEnterShutdown();
    S2LPEvalSpiInit(8000000);
    EepromCsPinInitialization();
    Clock_Wait(3);
    EepromRead(0, 1, &header);
    if(header==0 || header==0xff) continue;
    
    EepromRead(14, 1, &version);
    
    switch(version)
    {
    case 0x81:
    case 0x82:
      S2LPEvalSetVersion(S2LP_EVAL_FKI00x);
      return;  
    case 0:
      S2LPEvalSetVersion(S2LP_EVAL_IDB007);
      return;
    }
  }
}

void S2LPEvalSetVersion(uint8_t ver)
{
  s_S2LPEvalVersion=ver;
}

/**
* @brief  Returns the version of the current motherboard.
* @param  None.
* @retval None.
*/
uint8_t S2LPEvalGetVersion(void)
{
  return s_S2LPEvalVersion;
}


/**
* @}
*/

/**
* @}
*/


/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
