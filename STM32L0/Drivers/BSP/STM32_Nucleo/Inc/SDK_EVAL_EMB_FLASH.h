/**
* @file    SPIRIT_SDK_EMB_FLASH.h
* @author  AMG - RF Application team
* @version 3.2.1
* @date    March 26, 2018
* @brief   SDK EVAL eeprom management
* @details This module exports API to manage the EEPROM of the eval boards.
*          Data stored in the EEPROM are mainly some manifacturing infos,
*          and informations that can be useful when developing applications 
*          with the daughter board. Some of them are the RF band, the offset
*          of the carrier from the nominal frequency and the XTAL frequency.
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

/* Define to prevent recursive inclusion -------------------------------------*/



/* Includes ------------------------------------------------------------------*/
#include "cube_hal.h"

#define DEBUG_FLASH 0

#ifdef __cplusplus
"C" {
#endif
  
  /**
  * @addtogroup SDK_EVAL_NUCLEO
  * @{
  */
  
  /**
  * @defgroup SDK_EMB_FLASH
  * @brief Management of Software Development Kit eval board FLASH.
  * @details See the file <i>@ref SDK_EMB_FLASH.h</i> for more details.
  * @{
  */
  
#define FLASH_USER_START_ADDR   	nvm_data			/* Start @ of user Flash area */ 
#define FLASH_USER_END_ADDR		nvm_data+(sizeof(nvm_data))	/* End @ of user Flash area */
  
#define FLASH_BOARD_START_ADDR		board_data			/* Start @ of board data Flash area */ 
#define FLASH_BOARD_END_ADDR		board_data+(sizeof(board_data))	/* End @ of board data Flash area */		
  
#ifdef ERASE_VALUE_IS_FF
#define FLASH_EMPTY_BLOCK 0xFF
#else
#define FLASH_EMPTY_BLOCK 0x00
#endif
  
  
  /*--STM32L053xx--*/
#if defined(STM32L053xx) || defined(STM32L051xx)
#define MAX_NO_OF_PAGES		32	/* Pages for sector */
#endif
  
  
  /*--STM32L152xE--*/		
#if defined(STM32L152xE) || defined(STM32L151xBA)
#define MAX_NO_OF_PAGES		16	/* Pages for sector */
#endif		
  
  
  /*--STM32F072xB--*/		
#if defined(STM32F072xB)
#define MAX_NO_OF_PAGES		128	/* Pages for sector */
#endif
  
  
  /*--STM32F401xE--*/		
#if defined(STM32F401xE)
#define MAX_NO_OF_PAGES		32	/* Pages for sector */
#define FLASH_PAGE_SIZE		16*1024
  
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#endif
  
  /** 
  * @brief  FlashRead Status Enum
  */  
  typedef enum 
  {
    FLS_RW_OK		= 0x00,
    FLS_RW_ERROR	= 0x01,
    FLS_RW_OUT_OF_RANGE	= 0x02
  } FLS_RW_StatusTypeDef;
  
  
  extern const uint8_t nvm_data[FLASH_PAGE_SIZE];
  extern const uint8_t board_data[FLASH_PAGE_SIZE];
  
  FLS_RW_StatusTypeDef FlashRead(uint32_t nAddress,  uint16_t cNbBytes,  uint8_t* pcBuffer);
  FLS_RW_StatusTypeDef FlashWrite(uint32_t nAddress, uint16_t cNbBytes, uint8_t* pcBuffer, uint8_t eraseBeforeWrite);
  FLS_RW_StatusTypeDef FlashErase(uint32_t nAddress, uint32_t nPages);
  
  /**
  * @}
  */
  
  /**
  * @}
  */
  
#ifdef __cplusplus
}
#endif


/******************* (C) COPYRIGHT 2018STMicroelectronics *****END OF FILE****/

