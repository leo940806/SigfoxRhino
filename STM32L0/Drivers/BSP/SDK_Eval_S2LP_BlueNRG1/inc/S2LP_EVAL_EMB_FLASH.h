/**
* @file    SPIRIT_SDK_EMB_FLASH.h
* @author  AMG - RF Application team
* @version 3.2.1
* @date    March 26, 2018
* @brief   S2LP EVAL FLASH management
* @details This module is an utility to store S2LP info on BlueNRG based boards.
*          Data stored in the BlueNRG flash are mainly some manifacturing infos,
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
#ifndef __S2LP_EVAL_EMB_FLASH_H
#define __S2LP_EVAL_EMB_FLASH_H

/* Includes ------------------------------------------------------------------*/
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
#if BLUENRG2_DEVICE
#define NVM_DATA_PAGE   	124			/* Flash page for user data - BlueNRG2 */ 
#define BOARD_DATA_PAGE   	123			/* Flash page for board info - BlueNRG2 */ 
#else
#define NVM_DATA_PAGE   	76			/* Flash page for user data - BlueNRG1 */ 
#define BOARD_DATA_PAGE   	75			/* Flash page for board info - BlueNRG1 */ 
#endif
  
#define FLASH_USER_START_ADDR   	nvm_data			/* Start @ of user Flash area */ 
#define FLASH_USER_END_ADDR		nvm_data+(sizeof(nvm_data))	/* End @ of user Flash area */
  
#define FLASH_BOARD_START_ADDR		board_data			/* Start @ of board data Flash area */ 
#define FLASH_BOARD_END_ADDR		board_data+(sizeof(board_data))	/* End @ of board data Flash area */		
  
#ifdef ERASE_VALUE_IS_FF
#define FLASH_EMPTY_BLOCK 0xFF
#else
#define FLASH_EMPTY_BLOCK 0x00
#endif
  
/*BLUENRG2*/
#if defined(BLUENRG2_DEVICE)
#define MAX_NO_OF_PAGES		128	/* Pages for sector */
#define FLASH_PAGE_SIZE		8*64*4
  

/*BLUENRG1*/
#else 
#define MAX_NO_OF_PAGES   80 /* Pages for sector */
#define FLASH_PAGE_SIZE   8*64*4
  

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
  
  
  /*Exports data flash page of nvm_data and board data*/
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


#endif /* __S2LP_EVAL_EMB_FLASH_H */
/******************* (C) COPYRIGHT 2018STMicroelectronics *****END OF FILE****/

