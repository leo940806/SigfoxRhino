/**
  ******************************************************************************
  * @file    BlueNRG-1_S2LP_pinmap.h
  * @author  LowPower RF BU - AMG
  * @version 3.2.0
  * @date    May 1, 2016
  * @brief   This file contains the definition of the pin map of the S2-LP board on the BlueNRG-1 board.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/** @addtogroup SDK_EVAL_S2LP
  * @{
  */ 

/** @addtogroup SDK_EVAL_S2LP
 *  @brief This module contains the peripherial defines to run the SDK_EVAL_S2LP .
 * @{
 */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BLUENRG1_S2LP_PINMAP_H
#define __BLUENRG1_S2LP_PINMAP_H

#ifdef __cplusplus
 extern "C" {
#endif
                                              
/* Includes ------------------------------------------------------------------*/
#include "BlueNRG1_conf.h"
   

/** @defgroup BlueNRG1_S2LP_Exported_Constants Exported Constants  
 * @{
 */ 
   
#define S2LP_N_PLATFORMS     3

#define S2LP_EVAL_IDB007    0
#define S2LP_EVAL_FKI00x    1
#define S2LP_MON_REFERENCE  2
   

#define IDB007_SPI_CS_PIN                             GPIO_Pin_14
#define IDB007_SPI_MOSI_PIN                           GPIO_Pin_2
#define IDB007_SPI_MISO_PIN                           GPIO_Pin_3
#define IDB007_SPI_CLK_PIN                            GPIO_Pin_0
#define IDB007_M2S_GPIO_0_PIN                         GPIO_Pin_12
#define IDB007_M2S_GPIO_1_PIN                         GPIO_Pin_11
#define IDB007_M2S_GPIO_2_PIN                         GPIO_Pin_12
#define IDB007_M2S_GPIO_3_PIN                         GPIO_Pin_13  
#define IDB007_M2S_GPIO_SDN_PIN                       GPIO_Pin_6
#define IDB007_EEPROM_SPI_CS_PIN                      GPIO_Pin_8 


#define JORJIN_SPI_CS_PIN                             GPIO_Pin_1
#define JORJIN_SPI_MOSI_PIN                           GPIO_Pin_2
#define JORJIN_SPI_MISO_PIN                           GPIO_Pin_3
#define JORJIN_SPI_CLK_PIN                            GPIO_Pin_0
#define JORJIN_M2S_GPIO_0_PIN                         GPIO_Pin_12
#define JORJIN_M2S_GPIO_1_PIN                         GPIO_Pin_11
#define JORJIN_M2S_GPIO_2_PIN                         GPIO_Pin_12
#define JORJIN_M2S_GPIO_3_PIN                         GPIO_Pin_13  
#define JORJIN_M2S_GPIO_SDN_PIN                       GPIO_Pin_14
#define JORJIN_EEPROM_SPI_CS_PIN                      GPIO_Pin_6
   
#define MONARCH_SPI_CS_PIN                            GPIO_Pin_20
#define MONARCH_SPI_MOSI_PIN                          GPIO_Pin_2
#define MONARCH_SPI_MISO_PIN                          GPIO_Pin_3
#define MONARCH_SPI_CLK_PIN                           GPIO_Pin_0
#define MONARCH_M2S_GPIO_0_PIN                        GPIO_Pin_12 //Not connected to S2LP
#define MONARCH_M2S_GPIO_1_PIN                        GPIO_Pin_11 //Not connected to S2LP
#define MONARCH_M2S_GPIO_2_PIN                        GPIO_Pin_19 
#define MONARCH_M2S_GPIO_3_PIN                        GPIO_Pin_21  
#define MONARCH_M2S_GPIO_SDN_PIN                      GPIO_Pin_25
#define MONARCH_EEPROM_SPI_CS_PIN                     GPIO_Pin_8  //Not Connecte to Any EEPROM 

//Skyworks 66420 defines
#define MONARCH_SKY66420_CPS_PIN                      GPIO_Pin_23  
#define MONARCH_SKY66420_CSD_PIN                      GPIO_Pin_24
#define MONARCH_SKY66420_CTX_PIN                      GPIO_Pin_22 
   
#define S2LP_SPI_MOSI_HIGH_POWER                      ENABLE
#define S2LP_SPI_MOSI_PUPD                            ENABLE
#define S2LP_SPI_MOSI_MODE                            Serial0_Mode
   
#define S2LP_SPI_MISO_HIGH_POWER                      ENABLE
#define S2LP_SPI_MISO_PUPD                            ENABLE
#define S2LP_SPI_MISO_MODE                            Serial0_Mode

#define S2LP_SPI_CLK_HIGH_POWER                       ENABLE
#define S2LP_SPI_CLK_PUPD                             ENABLE
#define S2LP_SPI_CLK_MODE                             Serial0_Mode
   
#define S2LP_SPI_CS_HIGH_POWER                        ENABLE
#define S2LP_SPI_CS_PUPD                              ENABLE
#define S2LP_SPI_CS_MODE                              GPIO_Output
   
#define EEPROM_SPI_CS_HIGH_POWER                      ENABLE   

#define M2S_GPIO_0_HIGH_POWER                         DISABLE
#define M2S_GPIO_0_PUPD                               DISABLE
#define M2S_GPIO_0_MODE                               GPIO_Input
#define M2S_GPIO_0_EXTI_MODE                          0
#define M2S_GPIO_0_EXTI_PRIORITY                      LOW_PRIORITY


#define IDB007_BUTTON1_PIN                            GPIO_Pin_13
#define IDB007_BUTTON2_PIN                            GPIO_Pin_5
#define MONARCH_BUTTON1_PIN                           GPIO_Pin_13
#define MONARCH_BUTTON2_PIN                           GPIO_Pin_5
#define JORJIN_BUTTON_PIN                             GPIO_Pin_12
   

/**
  * @}
  */


    

#ifdef __cplusplus
}
#endif

#endif /* __BLUENRG1_S2LP_PINMAP_H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
