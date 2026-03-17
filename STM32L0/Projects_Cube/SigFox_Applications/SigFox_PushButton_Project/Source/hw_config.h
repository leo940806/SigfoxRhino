#ifndef __SYSTEM_INIT_H
#define __SYSTEM_INIT_H

/* BSP includes */
#include "SDK_EVAL_Config.h" // Be Careful! this file has 2 separate meanings (IDB00X or FKIXXX support)

void MCU_BSP_Init(void);

/* Switch from FKIXXX-STM32 solution to BlueNRG based sol. */
#if  !(defined(BLUENRG2_DEVICE) || defined(BLUENRG1_DEVICE))

  /* BSB FOR NUCLEO - STKEVAL */
  #include "MCU_Interface.h"
  #include "S2LP_SDK_Util.h"  
  #include "SDK_EVAL_Config.h"

  void NucleoUartGpioInit(void);

#else 

  #include "SDK_EVAL_Button.h"
  /*Exported from hw_config.c*/
  void EEPromToFlashProcedure(void);

  /* S2LP - IDB00X BSP Adaptation Layer includes */
  #include "S2LP_EVAL_Gpio.h"
  #include "S2LP_EVAL_EEPROM.h"
  #include "S2LP_EVAL_Config.h"
  #include "S2LP_EVAL_SPI.h"
  #include "S2LP_EVAL_Utils.h"

  /* BLE - BLE BSP */
  #include "BlueNRG1_conf.h"
  #include "clock.h"
  #include "SDK_EVAL_SPI.h"
  #include "SDK_EVAL_Com.h"
  #include "hal_types.h"

  /* Fix the Function names compatibility */
  #define SdkEvalEnterShutdown S2LPEvalEnterShutdown
  #define SdkEvalExitShutdown  S2LPEvalExitShutdown 
  #define SdkDelayMs           Clock_Wait 
  #define SdkEvalCheckShutdown S2LPEvalCheckShutdown 
  #define SdkEvalGetHasEeprom  S2LPEvalGetHasEeprom
  #define SdkEvalSetHasEeprom  S2LPEvalSetHasEeprom
  
  /* Allign BUTTON name Between Nucleo and IDB00X*/
  #define BUTTON_KEY BUTTON_2
#endif

#endif  /*__SYSTEM_INIT_H*/

