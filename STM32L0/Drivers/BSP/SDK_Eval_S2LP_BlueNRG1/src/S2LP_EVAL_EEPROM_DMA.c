/**
* @file    S2LP_EVAL_EEPROM_DMA.c
* @author  VMA division - AMS
* @version 3.2.0
* @date    May 1, 2016
* @brief   S2LP EVAL EEPROM management
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
* <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
*/  

/* Includes ------------------------------------------------------------------*/
#include "S2LP_EVAL_Config.h"
#include "S2LP_EVAL_EEPROM.h"

#include "BlueNRG1_conf.h"
#include "hal_types.h"
#include "BlueNRG-1_S2LP_pinmap.h"

/**
* @addtogroup S2LP_EVAL_NUCLEO
* @{
*/

#define M2E_SPI_GPIO_NUMBER 4

/**
 * @brief  MCU GPIO pin enumeration for GPIO
 */
typedef enum
{
  M2E_SPI_MISO     = 0x00, /*!< MISO selected */
  M2E_SPI_MOSI     = 0x01, /*!< MOSI selected */
  M2E_SPI_CLK      = 0x02, /*!< CLK selected */
  M2E_SPI_CS       = 0x03, /*!< CS selected */
  

}M2EGpioPin;
/**
* @defgroup S2LP_EEPROM_Management              S2LP EEPROM Management
* @{
*/


/** @defgroup S2LP_EEPROM_Management_Private_Variable               S2LP EEPROM Management Private Variable
 * @{
 */

/**
 * @brief  SPI buffers used for DMA application
 */

/**
 * @brief  M2S SPI Pin array
 */
static const uint16_t s_vectnM2SSpiPin[S2LP_N_PLATFORMS][M2E_SPI_GPIO_NUMBER] = {
  {IDB007_SPI_MISO_PIN,IDB007_SPI_MOSI_PIN,IDB007_SPI_CLK_PIN,IDB007_EEPROM_SPI_CS_PIN},
  {JORJIN_SPI_MISO_PIN,JORJIN_SPI_MOSI_PIN,JORJIN_SPI_CLK_PIN,JORJIN_EEPROM_SPI_CS_PIN},
};

static volatile uint8_t spi_buffer_rx[128];
static uint8_t s_eeprom  = 0;
/**
 * @}
 */


/** @defgroup S2LP_EEPROM_Functions
* @{
*/


/**
* @brief  Initializes the SPI for the EEPROM.
*         SPI, MISO, MOSI and SCLK are the same used for the SPIRIT1.
*         This function can be replaced by EepromCsPinInitialization if
*         S2LPSpiInit is called.
* @param  None
* @retval None
*/
void EepromSpiInitialization(uint32_t baudrate)
{
  SPI_InitType SPI_InitStructure;
  GPIO_InitType GPIO_InitStructure;
  
  /* Enable SPI and GPIO clocks */
  SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_GPIO | CLOCK_PERIPH_SPI, ENABLE);   
  
  /* Configure SPI pins */
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2E_SPI_MOSI];
  GPIO_InitStructure.GPIO_Mode = S2LP_SPI_MOSI_MODE;
  GPIO_InitStructure.GPIO_Pull = S2LP_SPI_MOSI_PUPD;
  GPIO_InitStructure.GPIO_HighPwr = S2LP_SPI_MOSI_HIGH_POWER;
  GPIO_Init(&GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2E_SPI_MISO];
  GPIO_InitStructure.GPIO_Mode = S2LP_SPI_MISO_MODE;
  GPIO_InitStructure.GPIO_Pull = S2LP_SPI_MISO_PUPD;
  GPIO_InitStructure.GPIO_HighPwr = S2LP_SPI_MISO_HIGH_POWER;
  GPIO_Init(&GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2E_SPI_CLK];
  GPIO_InitStructure.GPIO_Mode = S2LP_SPI_CLK_MODE;
  GPIO_InitStructure.GPIO_Pull = S2LP_SPI_CLK_PUPD;
  GPIO_InitStructure.GPIO_HighPwr = S2LP_SPI_CLK_HIGH_POWER;
  GPIO_Init(&GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pull = ENABLE;  
  GPIO_InitStructure.GPIO_Pin = s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2E_SPI_CS];
  GPIO_InitStructure.GPIO_Mode = GPIO_Output;
  GPIO_InitStructure.GPIO_HighPwr = EEPROM_SPI_CS_HIGH_POWER;
  GPIO_Init(&GPIO_InitStructure);
  GPIO_SetBits(s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2E_SPI_CS]);
  
  /* Configure SPI in master mode */
  SPI_StructInit(&SPI_InitStructure);
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b ;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_BaudRate = baudrate;
  SPI_Init(&SPI_InitStructure);
  
  /* Clear RX and TX FIFO */
  SPI_ClearTXFIFO();
  SPI_ClearRXFIFO();
  
  /* Set null character */
  SPI_SetDummyCharacter(0xFF);
  
  /* Set communication mode */
  SPI_SetMasterCommunicationMode(SPI_FULL_DUPLEX_MODE);
  
  /* Configure DMA peripheral */
  SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_DMA, ENABLE);
  
  DMA_InitType DMA_InitStructure;
  /* Configure DMA SPI TX channel */
  DMA_InitStructure.DMA_PeripheralBaseAddr = SPI_DR_BASE_ADDR;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)0;  
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = (uint32_t)SPI_BUFF_SIZE;  
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;    
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  
  DMA_Init(DMA_CH_SPI_TX, &DMA_InitStructure);
    
  /* Configure DMA SPI RX channel */
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)0;  
  DMA_InitStructure.DMA_BufferSize = (uint32_t)SPI_BUFF_SIZE;  
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_Init(DMA_CH_SPI_RX, &DMA_InitStructure);
  //DMA_Cmd(DMA_CH_SPI_RX, ENABLE);
  
  /* Enable SPI_TX/SPI_RX DMA requests */
  SPI_DMACmd(SPI_DMAReq_Tx | SPI_DMAReq_Rx, ENABLE);
  
  /* Enable DMA_CH_UART_TX Transfer Complete interrupt */
  DMA_FlagConfig(DMA_CH_SPI_TX, DMA_FLAG_TC, ENABLE);
  DMA_FlagConfig(DMA_CH_SPI_RX, DMA_FLAG_TC, ENABLE);
  
  /* Enable SPI functionality */
  //SPI_Cmd(ENABLE);  
}


/**
* @brief  Initialization of the CSn pin of the EEPROM.
*         This function is called internally by EepromCsPinInitialization.
* @param  None
* @retval None
*/
void EepromCsPinInitialization(void)
{
  GPIO_InitType GPIO_InitStructure;
  
  /* Enable SPI and GPIO clocks */
  SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_GPIO, ENABLE);   
  
  /* Configure SPI pins */
  GPIO_StructInit(&GPIO_InitStructure);
 
  GPIO_InitStructure.GPIO_Pull = ENABLE;  
  GPIO_InitStructure.GPIO_Pin = s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2E_SPI_CS];
  GPIO_InitStructure.GPIO_Mode = GPIO_Output;
  GPIO_InitStructure.GPIO_HighPwr = EEPROM_SPI_CS_HIGH_POWER;
  GPIO_Init(&GPIO_InitStructure);
  GPIO_SetBits(s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2E_SPI_CS]);
}

/**
* @brief  Read a page of the EEPROM.
*         A page size is 32 bytes.
*         The pages are 256.
*         Page 0 address: 0x0000
*         Page 1 address: 0x0020
*         ...
*         Page 255 address: 0x1FE0
* @param  None
* @retval None
*/
void EepromRead(uint16_t nAddress, uint8_t cNbBytes, uint8_t* pcBuffer)
{


  uint8_t cmd[3],rxb[3];

  cmd[0] = EEPROM_CMD_READ; // READ HEADER
  
  for(uint8_t k=0; k<2; k++) {
    cmd[k+1] = (uint8_t)(nAddress>>((1-k)*8));
  }  
  
  /* Rearm the DMA_CH_SPI_TX */
  DMA_CH_SPI_TX->CMAR = (uint32_t)cmd;
  DMA_CH_SPI_TX->CNDTR = 3;
  DMA_CH_SPI_RX->CMAR = (uint32_t)rxb;
  DMA_CH_SPI_RX->CNDTR = 3;
  
  GPIO_ResetBits(s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2E_SPI_CS]); 
  for(volatile uint32_t i=0;i<0x3f;i++);
  
  /* DMA_CH_SPI_TX enable */
  DMA_CH_SPI_TX->CCR_b.EN = SET;
  DMA_CH_SPI_RX->CCR_b.EN = SET;
  SPI_Cmd(ENABLE);  
  
  while(!DMA_GetFlagStatus(DMA_CH_SPI_TX_IT_TC));
  DMA_ClearFlag(DMA_CH_SPI_TX_IT_TC);
  
  while(!DMA_GetFlagStatus(DMA_CH_SPI_RX_IT_TC));
  DMA_ClearFlag(DMA_CH_SPI_RX_IT_TC);
  
  /* DMA1 finished the transfer of SrcBuffer */
  
  /* DMA_CH disable */
  DMA_CH_SPI_TX->CCR_b.EN = RESET;
  DMA_CH_SPI_RX->CCR_b.EN = RESET;
  
  while (SET == SPI_GetFlagStatus(SPI_FLAG_BSY));
  
  /* Rearm the DMA_CH_SPI_TX */
  DMA_CH_SPI_RX->CMAR = (uint32_t)pcBuffer;
  DMA_CH_SPI_RX->CNDTR = cNbBytes;
  DMA_CH_SPI_TX->CMAR = (uint32_t)pcBuffer;
  DMA_CH_SPI_TX->CNDTR = cNbBytes;
   
//  for(volatile uint32_t i=0;i<DELAY_CS_SCLK;i++);
//  /* DMA1 finished the transfer of SrcBuffer */
//  GPIO_SetBits(GPIO_Pin_8);
  
  /* DMA_CH_SPI_TX enable */
  DMA_CH_SPI_TX->CCR_b.EN = SET;
  DMA_CH_SPI_RX->CCR_b.EN = SET;
  
  while(!DMA_GetFlagStatus(DMA_CH_SPI_RX_IT_TC));
  DMA_ClearFlag(DMA_CH_SPI_RX_IT_TC);
  
  
  GPIO_SetBits(s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2E_SPI_CS]);
  
  /* DMA_CH disable */
  DMA_CH_SPI_TX->CCR_b.EN = RESET;
  DMA_CH_SPI_RX->CCR_b.EN = RESET;
  while (SET == SPI_GetFlagStatus(SPI_FLAG_BSY));
 
  SPI_Cmd(DISABLE);


}




/**
* @brief  Wait polling the SPI until the internal WIP flag is RESET.
*         The flag is SET when a write operation is running.
* @param  None
* @retval None
*/
void EepromWaitEndWriteOperation(void)
{

  uint8_t status; 
  /* Polling on status register */
  do
  {
    
    for(volatile uint32_t i=0;i<0x3ff;i++);
    
    status=EepromStatus();
    
  }while(status&EEPROM_STATUS_WIP);
  
}

void EepromWriteEnable(void)
{

  uint8_t cmd = EEPROM_CMD_WREN,rxb;
  
  /* Put the SPI chip select low to start the transaction */
  GPIO_ResetBits(s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2E_SPI_CS]); 
  
  /* Rearm the DMA_CH_SPI_TX */
  DMA_CH_SPI_TX->CMAR = (uint32_t)&cmd;
  DMA_CH_SPI_TX->CNDTR = 1;
  DMA_CH_SPI_RX->CMAR = (uint32_t)&rxb;
  DMA_CH_SPI_RX->CNDTR = 1;
  
  GPIO_ResetBits(s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2E_SPI_CS]); 
  for(volatile uint32_t i=0;i<0x3f;i++);
  
  /* DMA_CH_SPI_TX enable */
  DMA_CH_SPI_TX->CCR_b.EN = SET;
  DMA_CH_SPI_RX->CCR_b.EN = SET;
  SPI_Cmd(ENABLE);  
  
  while(!DMA_GetFlagStatus(DMA_CH_SPI_TX_IT_TC));
  DMA_ClearFlag(DMA_CH_SPI_TX_IT_TC);
  
  while(!DMA_GetFlagStatus(DMA_CH_SPI_RX_IT_TC));
  DMA_ClearFlag(DMA_CH_SPI_RX_IT_TC);
  
  GPIO_SetBits(s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2E_SPI_CS]);
  
  /* DMA_CH disable */
  DMA_CH_SPI_TX->CCR_b.EN = RESET;
  DMA_CH_SPI_RX->CCR_b.EN = RESET;
  while (SET == SPI_GetFlagStatus(SPI_FLAG_BSY));
  SPI_Cmd(DISABLE); 
  
  /* Put the SPI chip select high to end the transaction */
  GPIO_SetBits(s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2E_SPI_CS]); 
 
}

/**
* @brief  Write a page of the EEPROM.
*         A page size is 32 bytes.
*         The pages are 256.
*         Page 0 address: 0x0000
*         Page 1 address: 0x0020
*         ...
*         Page 255 address: 0x1FE0
*         It is allowed to write only a page for each operation. If the bytes
*         exceed the single page location, the other bytes are written at the 
*         beginning.
* @param  None
* @retval None
*/
void EepromWrite(uint16_t nAddress, uint8_t cNbBytes, uint8_t* pcBuffer)
{
  uint8_t cmd[3];
    

  /* we want to protect the first quarter of the flash. Allow to write in the portion > 0x500 */
//  if(nAddress<0x500) return;
  
  EepromWaitEndWriteOperation();
  /* SET the WREN flag */
  EepromWriteEnable();
  EepromWaitEndWriteOperation();
  
  cmd[0] = EEPROM_CMD_WRITE; // READ HEADER
  
  for(uint8_t k=0; k<2; k++) 
  {
    cmd[k+1] = (uint8_t)(nAddress>>((1-k)*8));
  }  
  
  /* Rearm the DMA_CH_SPI_TX */
  DMA_CH_SPI_TX->CMAR = (uint32_t)cmd;
  DMA_CH_SPI_TX->CNDTR = 3;
  DMA_CH_SPI_RX->CMAR = (uint32_t)spi_buffer_rx;
  DMA_CH_SPI_RX->CNDTR = 3;
  
  GPIO_ResetBits(s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2E_SPI_CS]); 
  for(volatile uint32_t i=0;i<0x3f;i++);
  
  /* DMA_CH_SPI_TX enable */
  DMA_CH_SPI_TX->CCR_b.EN = SET;
  DMA_CH_SPI_RX->CCR_b.EN = SET;
  SPI_Cmd(ENABLE);  
  
  while(!DMA_GetFlagStatus(DMA_CH_SPI_TX_IT_TC));
  DMA_ClearFlag(DMA_CH_SPI_TX_IT_TC);
  
  while(!DMA_GetFlagStatus(DMA_CH_SPI_RX_IT_TC));
  DMA_ClearFlag(DMA_CH_SPI_RX_IT_TC);
  
  /* DMA1 finished the transfer of SrcBuffer */
  
  /* DMA_CH disable */
  DMA_CH_SPI_TX->CCR_b.EN = RESET;
  DMA_CH_SPI_RX->CCR_b.EN = RESET;
  
  while (SET == SPI_GetFlagStatus(SPI_FLAG_BSY));
  
  /* Rearm the DMA_CH_SPI_TX */
  DMA_CH_SPI_RX->CMAR = (uint32_t)spi_buffer_rx;
  DMA_CH_SPI_RX->CNDTR = cNbBytes;
  DMA_CH_SPI_TX->CMAR = (uint32_t)pcBuffer;
  DMA_CH_SPI_TX->CNDTR = cNbBytes;
  
  /* DMA_CH_SPI_TX enable */
  DMA_CH_SPI_TX->CCR_b.EN = SET;
  DMA_CH_SPI_RX->CCR_b.EN = SET;
  
  while(!DMA_GetFlagStatus(DMA_CH_SPI_RX_IT_TC));
  DMA_ClearFlag(DMA_CH_SPI_RX_IT_TC);
  
  GPIO_SetBits(s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2E_SPI_CS]);
  
  /* DMA_CH disable */
  DMA_CH_SPI_TX->CCR_b.EN = RESET;
  DMA_CH_SPI_RX->CCR_b.EN = RESET;
  while (SET == SPI_GetFlagStatus(SPI_FLAG_BSY));
 
  SPI_Cmd(DISABLE);
  
  EepromWaitEndWriteOperation();

}


/**
* @brief  Read the status register.
* @param  None
* @retval Status
*/
uint8_t EepromStatus(void)
{
    
  uint8_t cmd = EEPROM_CMD_RDSR;
  uint8_t status;

  /* Rearm the DMA_CH_SPI_TX */
  DMA_CH_SPI_TX->CMAR = (uint32_t)&cmd;
  DMA_CH_SPI_TX->CNDTR = 1;
  DMA_CH_SPI_RX->CMAR = (uint32_t)&status;
  DMA_CH_SPI_RX->CNDTR = 1;
  
  GPIO_ResetBits(s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2E_SPI_CS]); 
  
  
  /* DMA_CH_SPI_TX enable */
  DMA_CH_SPI_TX->CCR_b.EN = SET;
  DMA_CH_SPI_RX->CCR_b.EN = SET;
  SPI_Cmd(ENABLE);  
  
  while(!DMA_GetFlagStatus(DMA_CH_SPI_TX_IT_TC));
  DMA_ClearFlag(DMA_CH_SPI_TX_IT_TC);
  
  
  while(!DMA_GetFlagStatus(DMA_CH_SPI_RX_IT_TC));
  DMA_ClearFlag(DMA_CH_SPI_RX_IT_TC);
  
  
  GPIO_SetBits(s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2E_SPI_CS]);
  
  /* DMA_CH disable */
  DMA_CH_SPI_TX->CCR_b.EN = RESET;
  DMA_CH_SPI_RX->CCR_b.EN = RESET;
  while (SET == SPI_GetFlagStatus(SPI_FLAG_BSY));
  SPI_Cmd(DISABLE);

return status;
}



/**
* @brief  Set the ERSR status bit.
* @param  None
* @retval Status
*/
uint8_t EepromSetSrwd(void)
{
  uint8_t status;
  uint8_t cmd[] = {EEPROM_CMD_WRSR, EEPROM_STATUS_SRWD};
  
 /* Rearm the DMA_CH_SPI_TX */
  DMA_CH_SPI_TX->CMAR = (uint32_t)&cmd;
  DMA_CH_SPI_TX->CNDTR = 2;
  DMA_CH_SPI_RX->CMAR = (uint32_t)&status;
  DMA_CH_SPI_RX->CNDTR = 1;
  
  GPIO_ResetBits(s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2E_SPI_CS]); 
  
  
  /* DMA_CH_SPI_TX enable */
  DMA_CH_SPI_TX->CCR_b.EN = SET;
  DMA_CH_SPI_RX->CCR_b.EN = SET;
  SPI_Cmd(ENABLE);  
  
  while(!DMA_GetFlagStatus(DMA_CH_SPI_TX_IT_TC));
  DMA_ClearFlag(DMA_CH_SPI_TX_IT_TC);
  
  
  while(!DMA_GetFlagStatus(DMA_CH_SPI_RX_IT_TC));
  DMA_ClearFlag(DMA_CH_SPI_RX_IT_TC);
  
  
  GPIO_SetBits(s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2E_SPI_CS]);
  
  /* DMA_CH disable */
  DMA_CH_SPI_TX->CCR_b.EN = RESET;
  DMA_CH_SPI_RX->CCR_b.EN = RESET;
  while (SET == SPI_GetFlagStatus(SPI_FLAG_BSY));
  SPI_Cmd(DISABLE);
    
  return status;
}
/**
* @brief  This function is to query if EEPROM is present or not.
* @param  None
* @retval 1 (yes) or 0 (no).
*/
uint8_t S2LPEvalGetHasEeprom(void)
{
  return s_eeprom;
}


/**
* @brief  This function is to set if EEPROM is present or not.
* @param  1 (yes) or 0 (no).
* @retval None
*/
void S2LPEvalSetHasEeprom(uint8_t eeprom)
{
  s_eeprom = eeprom;
}

/**
* @}
*/

/**
* @}
*/

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
