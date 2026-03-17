/**
* @file    S2LP_S2LP_Identification.c
* @author  VMA division - AMS
* @version 3.2.1
* @date    February 1, 2015
* @brief   Identification functions for S2LP DK.
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

#ifndef __S2LP_EVAL_UTILS_H
#define __S2LP_EVAL_UTILS_H

/* Includes ------------------------------------------------------------------*/
#include "S2LP_EVAL_EEPROM.h"

/**
* @addtogroup S2LP_DK                   S2LP DK
* @{
*/


/**
* @defgroup SDK_S2LP_MANAGEMENT              SDK S2LP Management
* @{
*/




static uint8_t s_RfModuleBand = 0;
static int32_t s_RfModuleOffset=0;



/**
* @defgroup SDK_S2LP_MANAGEMENT_FUNCTIONS    SDK S2LP Management Functions
* @{
*/

/**
* @defgroup IDENTIFICATION_FUNCTIONS      SDK S2LP Management Identification Functions
* @{
*/



uint8_t S2LPManagementGetBand(void);
uint8_t S2LPManagementGetRangeExtender(void);
void S2LPManagementSetOffset(int32_t value);
int32_t S2LPManagementGetOffset(void);
uint32_t S2LPManagementComputeXtalFrequency(void);
void S2LPManagementIdentificationRFBoard(void);
uint8_t S2LPManagementGetTcxo(void);
uint32_t S2LPManagementGetXtalFrequency(void);


#endif /* __S2LP_EVAL_UTILS_H*/
/**
* @}
*/



/**
* @}
*/


/**
* @}
*/

/**
* @}
*/


/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
