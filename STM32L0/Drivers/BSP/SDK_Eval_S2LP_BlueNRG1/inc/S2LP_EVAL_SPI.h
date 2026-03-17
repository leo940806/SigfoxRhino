/**
 * @file    SDK_EVAL_SPI.h
 * @author  AMS VMA RF application team
 * @version V1.0.0
 * @date    September 29, 2015
 * @brief   Header file for the SPI of S2-LP interface.
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


/* Includes ------------------------------------------------------------------*/
#include "SDK_EVAL_SPI.h"
#include "MCU_Interface.h"
#include "hal_types.h"


/** @addtogroup SDK_EVAL_BlueNRG1         SDK EVAL BlueNRG1
 * @{
 */


/** @addtogroup SDK_EVAL_SPI              SDK EVAL SPI
 * @{
 */

/** @defgroup SDK_EVAL_SPI_Private_TypesDefinitions       SDK EVAL SPI Private Types Definitions
 * @{
 */

/** @defgroup SPI_Exported_Types        SPI Exported Types
 * @{
 */

/**
 * @}
 */


/** @defgroup SDK_EVAL_SPI_Private_Defines                SDK EVAL SPI Private Defines
 * @{
 */

void S2LPEvalSpiInit(uint32_t baudrate);
S2LPStatus S2LPEvalSpiReadRegisters(uint8_t address, uint8_t n_bytes, uint8_t* buffer);
S2LPStatus S2LPEvalSpiWriteRegisters(uint8_t address, uint8_t n_bytes, uint8_t* buffer);
S2LPStatus S2LPEvalSpiCmdStrobe(uint8_t command);
S2LPStatus S2LPEvalSpiReadFifo(uint8_t n_bytes, uint8_t* buffer);
S2LPStatus S2LPEvalSpiWriteFifo(uint8_t n_bytes, uint8_t* buffer);
void S2LPEvalSpiRaw(uint8_t n_bytes, uint8_t* in_buffer, uint8_t* out_buffer, uint8_t can_return_bef_tx);

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
