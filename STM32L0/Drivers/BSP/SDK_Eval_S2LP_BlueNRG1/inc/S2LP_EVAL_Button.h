/**
 * @file    SDK_EVAL_Button.h
 * @author  AMS VMA RF application team
 * @version V1.0.0
 * @date    September 29, 2015
 * @brief   This file provides all the low level API to manage buttons for eval board.
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
 * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __S2LP_EVAL_BUTTON_H
#define __S2LP_EVAL_BUTTON_H

/* Includes ------------------------------------------------------------------*/

#include "hal_types.h"
#include "BlueNRG-1_S2LP_pinmap.h"

/** @addtogroup SDK_EVAL_BlueNRG1         SDK EVAL BlueNRG1
 * @{
 */


/** @addtogroup SDK_EVAL_Button           SDK EVAL Button
 * @{
 */

/** @defgroup SDK_EVAL_Button_Private_TypesDefinitions          SDK EVAL Button Private Types Definitions
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_Button_Private_Defines                   SDK EVAL Button Private Defines
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_Button_Private_FunctionPrototypes                        SDK EVAL Button Private Function Prototypes
 * @{
 */

/**
 * @}
 */



/** @defgroup SDK_EVAL_Button_Public_Functions                                 SDK EVAL Button Public Functions
 * @{
 */


void S2LPEvalPushButtonInit(SdkEvalButton xButton);
void S2LPEvalPushButtonIrq(SdkEvalButton xButton, SdkEvalButtonIrq xButtonIrq);
FlagStatus S2LPEvalPushButtonGetState(SdkEvalButton xButton);
FlagStatus S2LPEvalPushButtonGetITPendingBit(SdkEvalButton xButton);
void S2LPEvalPushButtonClearITPendingBit(SdkEvalButton xButton);


/**
 * @}
 */


/**
 * @}
 */


/**
 * @}
 */

#endif /*__S2LP_EVAL_BUTTON_H*/
/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
