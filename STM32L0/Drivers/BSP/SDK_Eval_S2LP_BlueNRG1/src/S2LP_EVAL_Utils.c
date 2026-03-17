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


/* Includes ------------------------------------------------------------------*/
#include "S2LP_EVAL_EEPROM.h"
#include "S2LP_EVAL_Gpio.h"
#include "clock.h"
/**
* @addtogroup S2LP_DK                   S2LP DK
* @{
*/


/**
* @defgroup SDK_S2LP_MANAGEMENT              SDK S2LP Management
* @{
*/




/**
* @brief This flag is used to synchronize the TIM3 ISR with the XtalMeasurement routine.
*/
static volatile uint8_t s_RfModuleBand = 0, s_RfRangeExtender = 0, s_Tcxo=0;
static volatile int32_t s_RfModuleOffset=0;
static volatile uint32_t s_RfXtalFrequency=50000000;


/**
* @defgroup SDK_S2LP_MANAGEMENT_FUNCTIONS    SDK S2LP Management Functions
* @{
*/

/**
* @defgroup IDENTIFICATION_FUNCTIONS      SDK S2LP Management Identification Functions
* @{
*/
void S2LPRadioSetXtalFrequency(uint32_t lXtalFrequency);

uint8_t S2LPManagementGetRangeExtender(void)
{
  return s_RfRangeExtender;
}

uint8_t S2LPManagementGetBand(void)
{
  return s_RfModuleBand;
}

void S2LPManagementSetOffset(int32_t value)
{
  s_RfModuleOffset=value;
}

int32_t S2LPManagementGetOffset(void)
{
  return s_RfModuleOffset;
}

uint32_t S2LPManagementComputeXtalFrequency(void)
{
  s_RfXtalFrequency=50000000;
  return s_RfXtalFrequency;
}

uint32_t S2LPManagementGetXtalFrequency(void)
{
  return s_RfXtalFrequency;
}

uint8_t S2LPManagementGetTcxo(void)
{
  return s_Tcxo;
}

__weak void S2LPRadioSetXtalFrequency(uint32_t xtal)
{

}



/**
* @brief  Read the status register.
* @param  None
* @retval Status
*/
uint8_t EepromIdentification(void)
{
  uint8_t status=0;
  
  /* Enable EEPROM CS */
  EepromCsPinInitialization();
        
  /* try to get the status of the EEPROM */
  status = EepromStatus();
  if((status&0xF0) == EEPROM_STATUS_SRWD) {
    /* if it is EEPROM_STATUS_SRWD, ok the EEPROM is present and ready to work */
    status=1;
  }
  else
  {
    EepromWriteEnable();
    Clock_Wait(10);
    /* else the bit may be not set (first time we see this EEPROM), try to set it*/
    status = EepromSetSrwd();
    Clock_Wait(10);
    /*check again*/
    status = EepromStatus();

    if((status&0xF0) == EEPROM_STATUS_SRWD) { // 0xF0 mask [SRWD 0 0 0]
      /* if it is EEPROM_STATUS_SRWD, ok the EEPROM is present and ready to work */
      status=1;
    }
    else
    {
      /* else no EEPROM is present */
      status = 0;
    }
  
  }

  return status;
}


void S2LPManagementIdentificationRFBoard(void)
{

  uint8_t was_in_sdn=1;
  
  if(S2LPEvalCheckShutdown()==RESET)
  {
    /* if reset it was not in SDN */
    S2LPEvalEnterShutdown();
    was_in_sdn=0;
  }
  
  //read the memory and set the variable
  uint8_t tmpBuffer[32];
  EepromRead(0x0000, 32, tmpBuffer);

  float foffset=0;
  if(tmpBuffer[0]==0 || tmpBuffer[0]==0xFF) {
    S2LPManagementComputeXtalFrequency();
    return;
  }
  switch(tmpBuffer[1]) {
  case 0:
    s_RfXtalFrequency = 24000000;
    break;
  case 1:
    s_RfXtalFrequency = 25000000;
    break;
  case 2:
    s_RfXtalFrequency = 26000000;
    break;
  case 3:
    s_RfXtalFrequency = 48000000;
    break;
  case 4:
    s_RfXtalFrequency = 50000000;
    break;
  case 5:
    s_RfXtalFrequency = 52000000;
    break;
  default:
    s_RfXtalFrequency=S2LPManagementComputeXtalFrequency();
    
    break;
  }
  S2LPRadioSetXtalFrequency(s_RfXtalFrequency);
  s_RfModuleBand=tmpBuffer[3];
  s_RfRangeExtender=tmpBuffer[5];
  
  EepromRead(0x0021,4,tmpBuffer);
  for(uint8_t i=0;i<4;i++)
  {
    ((uint8_t*)&foffset)[i]=tmpBuffer[3-i];
  }
  S2LPManagementSetOffset((int32_t)foffset);
  
  if(was_in_sdn==0)
  {
    S2LPEvalExitShutdown();
  }
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

/**
* @}
*/


/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
