#include "main.h"
#include "hw_config.h"

/******************************************************************************/
//                              COMMON PART
/******************************************************************************/

void MCU_BSP_Init(void)
{
/***************STM32 Implementation******************/
#if  !(defined(BLUENRG2_DEVICE) || defined(BLUENRG1_DEVICE))

  /* System initialization function */
  HAL_Init();
//  ST_MCU_API_SetSysClock();
    
  /* Enable the GPIO Clock, reset S2LP */
  SdkEvalM2SGpioInit(M2S_GPIO_SDN,M2S_MODE_GPIO_OUT);

  /* Enable SPI */
  SdkEvalSpiInit();
  
/***************BLE Implementation******************/
#else
  
  /* System initialization function */
  SystemInit();
  Clock_Init();
  
  /* Init S2LP SDN, reset S2LP  */
  S2LPEvalGpioInit(M2S_GPIO_SDN,M2S_MODE_GPIO_OUT);
  
  /* Identify BlueNRG1 platform */
  SdkEvalIdentification();
  
  /* Enable SPI */
  S2LPEvalSpiInit(8000000);
  
  /* Enable EEPROM CS */
  EepromCsPinInitialization();
#endif
}

/******************************************************************************/
//                         STM32 SPECIFIC FUNCTIONS
/******************************************************************************/
#if  !(defined(BLUENRG2_DEVICE) || defined(BLUENRG1_DEVICE))
/* Code for STM32 here */

/******************************************************************************/
//                         BLUENRG SPECIFIC FUNCTIONS
/******************************************************************************/
#else
/* Code for BLUENRG here */
void BLE_Activity(void){}
#endif /*Ifdef end*/

