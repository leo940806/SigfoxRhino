/*!
 * \file nvm_api.h
 * \brief NVM handler
 * \author  AMG - RF Application team
 * \version 1.1.0
 * \date Jul 6, 2018
 * \copyright COPYRIGHT 2018 STMicroelectronics
 *
 * This file defines the functions to store Sigfox data in EEPROM or FLASH
 */


#ifndef __NVM_API_H
#define __NVM_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/// \cond DO_NOT_DOCUMENT
#ifdef ERASE_VALUE_IS_FF
  #define NVM_BLOCK_INVALID	0xFFFFFFFCFFFFFF0E
  #define NVM_BLOCK_VALID	0xFFFFFF0CFFFFFF0E

  #define NVM_BLOCK_INVALID_1 0xFFFFFFFF
  #define NVM_BLOCK_INVALID_2 0xFFFFFF0E

  #define NVM_BLOCK_VALID_1 	0xFFFFFF0E
  #define NVM_BLOCK_VALID_2 	0xFFFFFF0C
#else
  #define NVM_BLOCK_INVALID	0x00000000000000FE
  #define NVM_BLOCK_VALID 	0x000000FC000000FE

  #define NVM_BLOCK_INVALID_1 0x00000000	
  #define NVM_BLOCK_INVALID_2 0x000000FE

  #define NVM_BLOCK_VALID_1 	0x000000FE
  #define NVM_BLOCK_VALID_2 	0x000000FC
#endif

#define BLOCK_HEADER_SIZE 8 /* To preserve data integrity, 2 words are reserved for record validation */
#define BLOCK_BODY_SIZE   8 /* NVM data size */

#define EEPROM_NVDATA_ADDRESS 0x600
#define RCZ_MASK 0x000000FF
/// \endcond

/**
* @brief Describes NVM Type. Possible values reference FLASH or EEPROM memory
*
*/
typedef enum
{
  NVM_TYPE_EEPROM = 0,
  NVM_TYPE_FLASH = 1
} NVM_TYPE;


/**
* @brief Describes write mode operation.
*
* @detail Flash Write can be be performed with or without erasing the page before write
*/
typedef enum
{	
  NVM_WRITE_MODE_WRITEOVER = 0, /*!< Write data without erase the page */
  NVM_WRITE_MODE_ERASE = 1      /*!< Erase the page before write */
} NVM_WRITE_MODE;


/**
* @brief Return values for NVM Read/Write functions
* 
*/
typedef enum
{
  NVM_RW_OK = 0,
  NVM_WRITE_ERROR,
  NVM_READ_ERROR,
  NVM_WRITE_RECORD_ERROR,
  NVM_READ_RECORD_ERROR,
  NVM_WRITE_HEADER_ERROR,
  NVM_NO_RECORDS
} NVM_RW_RESULTS;

/**
* @brief Board Data update modes
* 
*/
typedef enum
{
  NVM_BOARD_ID  = 0x01,
  NVM_BOARD_RCZ = 0x02,
  NVM_BOARD_PAC = 0x04,
  NVM_BOARD_FREQ = 0x08,
  NVM_BOARD_RSSI = 0x10,
} NVM_BOARD_DATA_UPDATE_WHAT;


/**
* @brief NVM API configuration structure
* 
* @detail Information needed to properly configure the NVM layer for Sigfox functionalities. Default is NVM_TYPE_EEPROM
*/
typedef struct
{
  NVM_TYPE nvmType; /*!< Specify if NVM is EEPROM or FLASH */
  uint32_t dataPageAddress;/*!< Destination address where store NVM_BoardDataType  */
  uint32_t userSpaceAddress;/*!< Destination address where store NVM recurrent data */
  uint8_t  messageSequenceNumber;/*!< Set this value to a specific number if Invalid message sequence error occurs */
} NVM_ConfigType;

/// \cond DO_NOT_DOCUMENT
#define INIT_NVM_CONFIG(X) NVM_ConfigType X = { .nvmType = NVM_TYPE_EEPROM, \
.dataPageAddress= 0, .userSpaceAddress = EEPROM_NVDATA_ADDRESS, .messageSequenceNumber = 0x00 }
/// \endcond

/**
* @brief Record for Sigfox recurrent data, like messages counter.
* 
*/
typedef struct
{
  uint16_t pn;
  uint16_t seqNumber;
  uint16_t fh;
  uint16_t rl;
} NVM_RecurrentDataType;


/**
* @brief Record for board data (Credentials and more...)
* 
*/
typedef struct
{	
  uint32_t id;
  uint32_t rcz;
  uint8_t  pac[8];
  uint8_t  key[16];
  uint8_t  aux[16];
  int32_t freqOffset;
  int32_t rssiOffset;
  uint32_t checkSum;
} NVM_BoardDataType;

/*-------------------------------------FUNCTION PROTOTYPES------------------------------------*/

/**
* @brief  Initialize NVM API
*
* @param  config configuration structure.
*/
void SetNVMInitial(NVM_ConfigType *config);


/**
* @brief  Returns the configured NVM Type
*/
NVM_TYPE GetNVMType(void);


/**
* @brief  Reads data from NVM
*
* @param  nAddress Read start address
* @param  cNbBytes umber of bytes to read
* @param  pcBuffer Returned buffer
* @retval NVM_RW_RESULTS
*/
NVM_RW_RESULTS NVM_Read(uint32_t nAddress, uint8_t cNbBytes, uint8_t* pcBuffer);


/**
* @brief Writes data to NVM
*
* @param  nAddress Write start address
* @param  cNbBytes Number of bytes to write
* @param  pcBuffer Data to write
* @param  writeMode Write mode operation type @see #NVM_WRITE_MODE
* @retval NVM_RW_RESULTS
*/
NVM_RW_RESULTS NVM_Write(uint32_t nAddress, uint8_t cNbBytes, uint8_t* pcBuffer, NVM_WRITE_MODE writeMode);


/**
* @brief Reads last record stored at userSpaceAddress
*
* @param  nvmRecord Returned record
* @param  recordSize Size of record
* @retval NVM_RW_RESULTS
*/
NVM_RW_RESULTS NVM_ReadRecord(uint8_t* nvmRecord, uint32_t recordSize);


/**
* @brief Writes record in the last available slot in the userSpace page. If no space left, the page will be erased before write
*
* @param  nvmRecord Record to write
* @param  recordSize Size of record
* @retval NVM_RW_RESULTS
*/
NVM_RW_RESULTS NVM_WriteRecord(uint8_t* nvmRecord, uint32_t recordSize);


/**
* @brief Reads Board Data information, stored in data page at dataPageAddress @see NVM_ConfigType
*
* @param  data Returned data
* @retval NVM_RW_RESULTS
*/
NVM_RW_RESULTS NVM_ReadBoardData(NVM_BoardDataType *data);


/**
* @brief Writes Board Data information in data page at dataPageAddress @see NVM_ConfigType
*
* @param  data Data to write
* @retval NVM_RW_RESULTS
*/
NVM_RW_RESULTS NVM_WriteBoardData(NVM_BoardDataType *data);


/**
* @brief Update Board Data information in data page at dataPageAddress @see NVM_ConfigType
*
* @param  data Data to write
* @param  updateWhat Specifies what part of Board Data require updates. Valuess are type of NVM_BOARD_DATA_UPDATE_WHAT and can be chainde in OR
* Ex:
* @code
* NVM_RW_RESULTS res = NVM_UpdateBoardData(updatedData, NVM_BOARD_PAC | NVM_BOARD_KEY);
* @endcode
* @retval NVM_RW_RESULTS
*/
NVM_RW_RESULTS NVM_UpdateBoardData(NVM_BoardDataType *data, uint8_t updateWhat);

#ifdef __cplusplus
}
#endif

#endif

/******************* (C) COPYRIGHT 2018 STMicroelectronics *****END OF FILE****/
