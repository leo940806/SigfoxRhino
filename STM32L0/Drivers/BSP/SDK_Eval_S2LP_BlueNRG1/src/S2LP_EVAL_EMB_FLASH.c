/**
* @file    SDK_EVAL_EMB_FLASH.c
* @author  AMG division - AMS
* @version 1.0.0
* @date    Mar 23, 2018
* @brief   SDK EVAL flash management
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
* <h2><center>&copy; COPYRIGHT 2018 STMicroelectronics</center></h2>
*/  

/* Includes ------------------------------------------------------------------*/
#include "BlueNRG1_flash.h"
#include "S2LP_EVAL_EMB_FLASH.h"
#include "string.h"

#if defined(__ICCARM__) || defined(__IAR_SYSTEMS_ASM__)
	#pragma data_alignment=FLASH_PAGE_SIZE
#endif

		
#if defined(__ICCARM__) || defined(__IAR_SYSTEMS_ASM__)
	__root __no_init const uint8_t nvm_data[FLASH_PAGE_SIZE] @(_MEMORY_FLASH_BEGIN_ + NVM_DATA_PAGE*FLASH_PAGE_SIZE);    
	__root __no_init const uint8_t board_data[FLASH_PAGE_SIZE] @(_MEMORY_FLASH_BEGIN_ + BOARD_DATA_PAGE*FLASH_PAGE_SIZE);  
     
#else        
#ifdef __CC_ARM
	const uint8_t nvm_data[FLASH_PAGE_SIZE] __attribute(( at(_MEMORY_FLASH_BEGIN_ + NVM_DATA_PAGE*FLASH_PAGE_SIZE), used, aligned(FLASH_PAGE_SIZE) ));    		
	const uint8_t board_data[FLASH_PAGE_SIZE] __attribute(( at(_MEMORY_FLASH_BEGIN_ + BOARD_DATA_PAGE*FLASH_PAGE_SIZE), used, aligned(FLASH_PAGE_SIZE) ));   
#else

#error Neither ICCARM nor CC ARM detected. Define your macros.	
#endif
#endif

/**
* @addtogroup SDK_EVAL_NUCLEO
* @{
*/

/**
* @defgroup SDK_FLASH              SDK FLASH Management
* @{
*/

uint32_t  PAGEError = 0;

/**
* @defgroup FLASH Functions
* @{
*/

static uint32_t GetNumberOfPagesByBytes(uint32_t nBytesCount)
{
	uint32_t nRet, nTmp;
	
	nTmp = nBytesCount % 2048;	//2KB for BlueNRG1/2
	nRet = (!nTmp)?(nBytesCount/2048):((nBytesCount/2048)+1);
	
	return nRet;
}

FLS_RW_StatusTypeDef FlashRead(uint32_t nAddress, uint16_t cNbBytes, uint8_t* pcBuffer)
{
	FLS_RW_StatusTypeDef frRetStatus = FLS_RW_OK;
	volatile uint32_t tmp;
	uint8_t i, count4;	
	uint16_t pageIdx;
	
#if DEBUG_FLASH
	nAddress = FLASH_USER_START_ADDR;
#endif
        
        //Address should be word alligned
	if ((nAddress%4)>0)
              frRetStatus = FLS_RW_ERROR;

	if(pcBuffer == NULL)
		frRetStatus = FLS_RW_ERROR;
	
	if(frRetStatus == FLS_RW_OK) 
	{
		for(i=0; i<cNbBytes; i++)
		{
			count4  = (i-((i/4)*4)); 	 /* Counts 0...3 and restarts */
			pageIdx = (i/4)*4; 			 /* Every 4 bytes moves ahead */
			
			tmp = *((__IO uint32_t *)(nAddress+pageIdx));

			pcBuffer[i] = (tmp&(0xFF000000>>(count4*8)))>>(24-(count4*8));
		}
	}
	
	return frRetStatus;
}

FLS_RW_StatusTypeDef FlashWrite(uint32_t nAddress, uint16_t cNbBytes, uint8_t* pcBuffer, uint8_t eraseBeforeWrite)
{	
	uint8_t i, count4;
	uint16_t pageIdx;
	uint32_t temp_word;
	uint32_t __attribute__ ((unused)) error_code;
	
	FLS_RW_StatusTypeDef frRetStatus = FLS_RW_OK;

	if(GetNumberOfPagesByBytes(cNbBytes) > MAX_NO_OF_PAGES)
		frRetStatus = FLS_RW_OUT_OF_RANGE;

	if(pcBuffer == NULL)
		frRetStatus = FLS_RW_ERROR;

        //Address must be word alligned (4 byte for each row)
	if ((nAddress%4)>0)
              frRetStatus = FLS_RW_ERROR;	
	
#if DEBUG_FLASH
	nAddress = FLASH_USER_START_ADDR;
#endif

	if (eraseBeforeWrite)	
		frRetStatus = FlashErase(nAddress, GetNumberOfPagesByBytes(cNbBytes));
	
	if(frRetStatus == FLS_RW_OK)
	{
		temp_word=0;
		
	
		for(i=0; i<cNbBytes; i++)
		{
			count4  = i-((i/4)*4);	/* Counts 0...3 and restarts */
			pageIdx = (i/4)*4; 			/* Every 4 bytes writes page */
			
			temp_word |= ((uint32_t)pcBuffer[i])<<(24-(8*count4));
			
			if((i==cNbBytes-1) || count4 == 3 )/* Write every 4 bytes or if bytes in args are less than 4 */
			{

				FLASH_ProgramWord(nAddress+pageIdx, temp_word);
				if (!FLASH_GetFlagStatus(Flash_CMDERR))
				{					
					temp_word = 0;
				}
				else
				{
					//printf("> ERROR WHILE WRITING PAGE: %d", error_code);
					frRetStatus = FLS_RW_ERROR;
					break;
				}			
			}
		}
	}	
	
	return frRetStatus;
}



FLS_RW_StatusTypeDef FlashErase(uint32_t nAddress, uint32_t nPages)
{
	uint32_t __attribute__ ((unused)) error_code;
	FLS_RW_StatusTypeDef frRetStatus = FLS_RW_ERROR;
	
	/* To compute the number of byte from nAddres and the start of the flash */
	uint32_t num_byte = nAddress - _MEMORY_FLASH_BEGIN_;

        //Address must be word alligned
	if ((nAddress%4)>0)
              frRetStatus = FLS_RW_ERROR;
        
	for (int i=0; i<nPages; i++)
	{
		FLASH_ErasePage(GetNumberOfPagesByBytes(num_byte)+i);
	}

	if(FLASH_GetFlagStatus(Flash_CMDERR))
	{
		frRetStatus=FLS_RW_ERROR;	//printf("> ERROR WHILE ERASING PAGE: %d", error_code);
	}
	else
	{
		frRetStatus = FLS_RW_OK;
	}

	return frRetStatus;
}

/**
* @}
*/


/******************* (C) COPYRIGHT 2018 STMicroelectronics *****END OF FILE****/
