/**
 * @file    S2LP_EVAL_SPI_DMA.c
 * @author  AMS VMA RF application team
 * @version V1.0.0
 * @date    April 1, 2017
 * @brief   This file provides all the low level API to manage SPI interface for eval board.
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
 * <h2><center>&copy; COPYRIGHT 2017 STMicroelectronics</center></h2>
 */


/* Includes ------------------------------------------------------------------*/
#include "SDK_EVAL_SPI.h"
#include "BlueNRG-1_S2LP_pinmap.h"
#include "MCU_Interface.h"
#include "S2LP_EVAL_Config.h"
#include <stdio.h>

/** @addtogroup SDK_EVAL_S2LP         SDK EVAL S2LP
 * @{
 */


/** @addtogroup S2LP_EVAL_SPI              S2LP EVAL SPI
 * @{
 */


/** @defgroup S2LP_EVAL_SPI_Private_Defines                S2LP EVAL SPI Private Defines
 * @{
 */



#define DELAY_CS_SCLK   0x3f
#define M2S_SPI_GPIO_NUMBER 4

/**
 * @brief  MCU GPIO pin enumeration for GPIO
 */
typedef enum
{
  M2S_SPI_MISO     = 0x00, /*!< MISO selected */
  M2S_SPI_MOSI     = 0x01, /*!< MOSI selected */
  M2S_SPI_CLK     = 0x02, /*!< CLK selected */
  M2S_SPI_CS     = 0x03, /*!< CS selected */
  

}M2SGpioPin;

/**
 * @}
 */


/** @defgroup S2LP_EVAL_SPI_Private_Variable               S2LP EVAL SPI Private Variable
 * @{
 */

/**
 * @brief  M2S SPI Pin array
 */
static const uint32_t s_vectnM2SSpiPin[S2LP_N_PLATFORMS][M2S_SPI_GPIO_NUMBER] = {
  {IDB007_SPI_MISO_PIN,IDB007_SPI_MOSI_PIN,IDB007_SPI_CLK_PIN,IDB007_SPI_CS_PIN},
  {JORJIN_SPI_MISO_PIN,JORJIN_SPI_MOSI_PIN,JORJIN_SPI_CLK_PIN,JORJIN_SPI_CS_PIN},
  {MONARCH_SPI_MISO_PIN,MONARCH_SPI_MOSI_PIN,MONARCH_SPI_CLK_PIN,MONARCH_SPI_CS_PIN},
};

/**
 * @brief  SPI buffers used for DMA application
 */
static volatile uint8_t spi_buffer_tx[128];
static volatile uint8_t spi_buffer_rx[128];



/**
 * @}
 */


/**
 * @defgroup S2LP_EVAL_SPI_Public_Functions                S2LP EVAL SPI Public Functions
 * @{
 */


/**
 * @brief  Configures SPI interface
 * @param  baudrate: SPI clock frequency
 * @retval None.
 */
void S2LPEvalSpiInit(uint32_t baudrate)
{
  SPI_InitType SPI_InitStructure;
  GPIO_InitType GPIO_InitStructure;
  
  /* Enable SPI and GPIO clocks */
  SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_GPIO | CLOCK_PERIPH_SPI, ENABLE);   
  
  /* Configure SPI pins */
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2S_SPI_MOSI];
  GPIO_InitStructure.GPIO_Mode = S2LP_SPI_MOSI_MODE;
  GPIO_InitStructure.GPIO_Pull = S2LP_SPI_MOSI_PUPD;
  GPIO_InitStructure.GPIO_HighPwr = S2LP_SPI_MOSI_HIGH_POWER;
  GPIO_Init(&GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2S_SPI_MISO];
  GPIO_InitStructure.GPIO_Mode = S2LP_SPI_MISO_MODE;
  GPIO_InitStructure.GPIO_Pull = S2LP_SPI_MISO_PUPD;
  GPIO_InitStructure.GPIO_HighPwr = S2LP_SPI_MISO_HIGH_POWER;
  GPIO_Init(&GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2S_SPI_CLK];
  GPIO_InitStructure.GPIO_Mode = S2LP_SPI_CLK_MODE;
  GPIO_InitStructure.GPIO_Pull = S2LP_SPI_CLK_PUPD;
  GPIO_InitStructure.GPIO_HighPwr = S2LP_SPI_CLK_HIGH_POWER;
  GPIO_Init(&GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2S_SPI_CS];
  GPIO_InitStructure.GPIO_Mode = S2LP_SPI_CS_MODE;
  GPIO_InitStructure.GPIO_Pull = S2LP_SPI_CS_PUPD;
  GPIO_InitStructure.GPIO_HighPwr = S2LP_SPI_CS_HIGH_POWER;
  GPIO_Init(&GPIO_InitStructure);
  GPIO_SetBits(s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2S_SPI_CS]);
  
  SPI_DeInit();
  
  /* Configure SPI in master mode */
  SPI_StructInit(&SPI_InitStructure);
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b ;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//SPI_CPHA_2Edge;
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
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)spi_buffer_tx;  
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
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)spi_buffer_rx;  
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
 * @brief  SPI function to read registers from a slave device
 * @param  pBuffer: buffer to retrieve data from a slave
 * @param  RegisterAddr: register address
 * @param  NumByteToRead: number of byte to read
 * @retval ErrorStatus: error status @ref ErrorStatus
 *         This parameter can be: SUCCESS or ERROR.
 */
S2LPStatus S2LPEvalSpiReadRegisters(uint8_t address, uint8_t n_bytes, uint8_t* buffer)
{
  StatusBytes status;
  uint8_t *status_b=(uint8_t*)&status;
  uint8_t cmd[2];
  cmd[0]=0x01;          /* READ register header */
  cmd[1]=address;       /* ADDRESS of the register to read */
  
  (*status_b)=0;
  
  /* Rearm the DMA_CH_SPI_TX */
  DMA_CH_SPI_TX->CMAR = (uint32_t)cmd;
  DMA_CH_SPI_TX->CNDTR = 2;
  DMA_CH_SPI_RX->CMAR = (uint32_t)status_b;
  DMA_CH_SPI_RX->CNDTR = 2;
  
  GPIO_ResetBits(s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2S_SPI_CS]); 
  for(volatile uint32_t i=0;i<DELAY_CS_SCLK;i++);
  
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
  DMA_CH_SPI_RX->CMAR = (uint32_t)buffer;
  DMA_CH_SPI_RX->CNDTR = n_bytes;
  DMA_CH_SPI_TX->CMAR = (uint32_t)spi_buffer_tx;
  DMA_CH_SPI_TX->CNDTR = n_bytes;
  
  /* DMA_CH_SPI_TX enable */
  DMA_CH_SPI_TX->CCR_b.EN = SET;
  DMA_CH_SPI_RX->CCR_b.EN = SET;
  
  while(!DMA_GetFlagStatus(DMA_CH_SPI_RX_IT_TC));
  DMA_ClearFlag(DMA_CH_SPI_RX_IT_TC);
  
  for(volatile uint32_t i=0;i<DELAY_CS_SCLK;i++);
  /* DMA1 finished the transfer of SrcBuffer */
  GPIO_SetBits(s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2S_SPI_CS]);
  
  /* DMA_CH disable */
  DMA_CH_SPI_TX->CCR_b.EN = RESET;
  DMA_CH_SPI_RX->CCR_b.EN = RESET;
  while (SET == SPI_GetFlagStatus(SPI_FLAG_BSY));
  SPI_Cmd(DISABLE);
 
  cmd[0]=status_b[1];
  status_b[1]=status_b[0];
  status_b[0]=cmd[0];
  
  return status;
}


/**
 * @brief  SPI function to write registers of a slave device
 * @param  pBuffer: buffer contains data to write
 * @param  RegisterAddr: register address
 * @param  NumByteToRead: number of byte to write
 * @retval ErrorStatus: error status @ref ErrorStatus
 *         This parameter can be: SUCCESS or ERROR.
 */
S2LPStatus S2LPEvalSpiWriteRegisters(uint8_t address, uint8_t n_bytes, uint8_t* buffer)
{
  StatusBytes status;
  uint8_t *status_b=(uint8_t*)&status;
  uint8_t cmd[2];
  cmd[0]=0x00;       /* WRITE register header */
  cmd[1]=address;    /* ADDRESS of the register to write */
  
  (*status_b)=0;
  
  /* Rearm the DMA_CH_SPI_TX */
  DMA_CH_SPI_TX->CMAR = (uint32_t)cmd;
  DMA_CH_SPI_TX->CNDTR = 2;
  DMA_CH_SPI_RX->CMAR = (uint32_t)status_b;
  DMA_CH_SPI_RX->CNDTR = 2;
  
  GPIO_ResetBits(s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2S_SPI_CS]); 
  for(volatile uint32_t i=0;i<DELAY_CS_SCLK;i++);
  
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
  DMA_CH_SPI_RX->CNDTR = n_bytes;
  DMA_CH_SPI_TX->CMAR = (uint32_t)buffer;
  DMA_CH_SPI_TX->CNDTR = n_bytes;
   
  /* DMA_CH_SPI_TX enable */
  DMA_CH_SPI_TX->CCR_b.EN = SET;
  DMA_CH_SPI_RX->CCR_b.EN = SET;
  
  while(!DMA_GetFlagStatus(DMA_CH_SPI_RX_IT_TC));
  DMA_ClearFlag(DMA_CH_SPI_RX_IT_TC);
  
  for(volatile uint32_t i=0;i<DELAY_CS_SCLK;i++);
  
  /* DMA1 finished the transfer of SrcBuffer */
  GPIO_SetBits(s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2S_SPI_CS]);
  
  /* DMA_CH disable */
  DMA_CH_SPI_TX->CCR_b.EN = RESET;
  DMA_CH_SPI_RX->CCR_b.EN = RESET;
  while (SET == SPI_GetFlagStatus(SPI_FLAG_BSY));
  SPI_Cmd(DISABLE);
 
  cmd[0]=status_b[1];
  status_b[1]=status_b[0];
  status_b[0]=cmd[0];
  
  return status;
}

S2LPStatus S2LPEvalSpiCommandStrobes(uint8_t command)
{
  StatusBytes status;
  uint8_t *status_b=(uint8_t*)&status;
  uint8_t cmd[2];
  cmd[0]=0x80;//cmd register header
  cmd[1]=command;
  
  (*status_b)=0;
  
  /* Rearm the DMA_CH_SPI_TX */
  DMA_CH_SPI_TX->CMAR = (uint32_t)cmd;
  DMA_CH_SPI_TX->CNDTR = 2;
  DMA_CH_SPI_RX->CMAR = (uint32_t)status_b;
  DMA_CH_SPI_RX->CNDTR = 2;
  
  GPIO_ResetBits(s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2S_SPI_CS]); 
  for(volatile uint32_t i=0;i<DELAY_CS_SCLK;i++);
  
  /* DMA_CH_SPI_TX enable */
  DMA_CH_SPI_TX->CCR_b.EN = SET;
  DMA_CH_SPI_RX->CCR_b.EN = SET;
  SPI_Cmd(ENABLE);  
  
  while(!DMA_GetFlagStatus(DMA_CH_SPI_TX_IT_TC));
  DMA_ClearFlag(DMA_CH_SPI_TX_IT_TC);
  
  while(!DMA_GetFlagStatus(DMA_CH_SPI_RX_IT_TC));
  DMA_ClearFlag(DMA_CH_SPI_RX_IT_TC);
  
  /* DMA1 finished the transfer of SrcBuffer */
  for(volatile uint32_t i=0;i<DELAY_CS_SCLK;i++);
  /* DMA1 finished the transfer of SrcBuffer */
  GPIO_SetBits(s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2S_SPI_CS]);
  
  /* DMA_CH disable */
  DMA_CH_SPI_TX->CCR_b.EN = RESET;
  DMA_CH_SPI_RX->CCR_b.EN = RESET;
  
  while (SET == SPI_GetFlagStatus(SPI_FLAG_BSY));
  SPI_Cmd(DISABLE);
 
  cmd[0]=status_b[1];
  status_b[1]=status_b[0];
  status_b[0]=cmd[0];
  return status;
}


S2LPStatus S2LPEvalSpiReadFifo(uint8_t n_bytes, uint8_t* buffer)
{
  StatusBytes status;
  uint8_t *status_b=(uint8_t*)&status;
  uint8_t cmd[2];
  cmd[0]=0x01;//READ register header
  cmd[1]=0xFF;
  
  (*status_b)=0;
  
  /* Rearm the DMA_CH_SPI_TX */
  DMA_CH_SPI_TX->CMAR = (uint32_t)cmd;
  DMA_CH_SPI_TX->CNDTR = 2;
  DMA_CH_SPI_RX->CMAR = (uint32_t)status_b;
  DMA_CH_SPI_RX->CNDTR = 2;
  
  GPIO_ResetBits(s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2S_SPI_CS]); 
  for(volatile uint32_t i=0;i<DELAY_CS_SCLK;i++);
  
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
  DMA_CH_SPI_RX->CMAR = (uint32_t)buffer;
  DMA_CH_SPI_RX->CNDTR = n_bytes;
  DMA_CH_SPI_TX->CMAR = (uint32_t)buffer;
  DMA_CH_SPI_TX->CNDTR = n_bytes;
   
  /* DMA_CH_SPI_TX enable */
  DMA_CH_SPI_TX->CCR_b.EN = SET;
  DMA_CH_SPI_RX->CCR_b.EN = SET;
  
  while(!DMA_GetFlagStatus(DMA_CH_SPI_RX_IT_TC));
  DMA_ClearFlag(DMA_CH_SPI_RX_IT_TC);
  
  for(volatile uint32_t i=0;i<DELAY_CS_SCLK;i++);
  /* DMA1 finished the transfer of SrcBuffer */
  GPIO_SetBits(s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2S_SPI_CS]);
  
  /* DMA_CH disable */
  DMA_CH_SPI_TX->CCR_b.EN = RESET;
  DMA_CH_SPI_RX->CCR_b.EN = RESET;
  while (SET == SPI_GetFlagStatus(SPI_FLAG_BSY));
  SPI_Cmd(DISABLE);
 
  cmd[0]=status_b[1];
  status_b[1]=status_b[0];
  status_b[0]=cmd[0];
  
  return status;
}

S2LPStatus S2LPEvalSpiWriteFifo(uint8_t n_bytes, uint8_t* buffer)
{
  StatusBytes status;
  uint8_t *status_b=(uint8_t*)&status;
  uint8_t cmd[2];
  cmd[0]=0x00;//WRITE register header
  cmd[1]=0xff;
  
  (*status_b)=0;
  
  /* Rearm the DMA_CH_SPI_TX */
  DMA_CH_SPI_TX->CMAR = (uint32_t)cmd;
  DMA_CH_SPI_TX->CNDTR = 2;
  DMA_CH_SPI_RX->CMAR = (uint32_t)status_b;
  DMA_CH_SPI_RX->CNDTR = 2;
  
  GPIO_ResetBits(s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2S_SPI_CS]); 
  for(volatile uint32_t i=0;i<DELAY_CS_SCLK;i++);
  
  /* DMA_CH_SPI_TX enable */
  DMA_CH_SPI_TX->CCR_b.EN = SET;
  DMA_CH_SPI_RX->CCR_b.EN = SET;
  SPI_Cmd(ENABLE);  
  
  while(!DMA_GetFlagStatus(DMA_CH_SPI_TX_IT_TC));
  DMA_ClearFlag(DMA_CH_SPI_TX_IT_TC);
  
  while(!DMA_GetFlagStatus(DMA_CH_SPI_RX_IT_TC));
  DMA_ClearFlag(DMA_CH_SPI_RX_IT_TC);
  
  /* DMA finished the transfer of SrcBuffer */
  
  /* DMA_CH disable */
  DMA_CH_SPI_TX->CCR_b.EN = RESET;
  DMA_CH_SPI_RX->CCR_b.EN = RESET;
  
  while (SET == SPI_GetFlagStatus(SPI_FLAG_BSY));
  
  /* Rearm the DMA_CH_SPI_TX */
  DMA_CH_SPI_RX->CMAR = (uint32_t)spi_buffer_rx;
  DMA_CH_SPI_RX->CNDTR = n_bytes;
  DMA_CH_SPI_TX->CMAR = (uint32_t)buffer;
  DMA_CH_SPI_TX->CNDTR = n_bytes;
  
  /* DMA_CH_SPI_TX enable */
  DMA_CH_SPI_TX->CCR_b.EN = SET;
  DMA_CH_SPI_RX->CCR_b.EN = SET;
  
  while(!DMA_GetFlagStatus(DMA_CH_SPI_RX_IT_TC));
  DMA_ClearFlag(DMA_CH_SPI_RX_IT_TC);

  for(volatile uint32_t i=0;i<DELAY_CS_SCLK;i++);
  /* DMA1 finished the transfer of SrcBuffer */
  GPIO_SetBits(s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2S_SPI_CS]);
  
  /* DMA_CH disable */
  DMA_CH_SPI_TX->CCR_b.EN = RESET;
  DMA_CH_SPI_RX->CCR_b.EN = RESET;
  while (SET == SPI_GetFlagStatus(SPI_FLAG_BSY));
  SPI_Cmd(DISABLE);
 
  cmd[0]=status_b[1];
  status_b[1]=status_b[0];
  status_b[0]=cmd[0];
  
  return status;
}

__weak void S2LPEvalSpiRawTc(void){}

void DMA_Handler(void)
{
  /* Check Transfer Complete interrupt */
  if(DMA_GetFlagStatus(DMA_CH_SPI_RX_IT_TC))
  {
    DMA_ClearFlag(DMA_CH_SPI_RX_IT_TC);
    
    /* DMA_CH disable */
    DMA_CH_SPI_TX->CCR_b.EN = RESET;
    DMA_CH_SPI_RX->CCR_b.EN = RESET;
    
    while (SET == SPI_GetFlagStatus(SPI_FLAG_BSY));
    
    GPIO_SetBits(s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2S_SPI_CS]);
    
    SPI_Cmd(DISABLE);
    
    NVIC_Init(&(NVIC_InitType){DMA_IRQn,MED_PRIORITY,DISABLE});
  
//    DMA_ClearFlag(DMA_CH_SPI_TX_IT_TC);
    
    S2LPEvalSpiRawTc();
  }
  
  if(DMA_GetFlagStatus(DMA_CH_SPI_TX_IT_TC))
  {
    DMA_ClearFlag(DMA_CH_SPI_TX_IT_TC);
  }
  
}

void S2LPEvalSpiRaw(uint8_t n_bytes, uint8_t* in_buffer, uint8_t* out_buffer, uint8_t can_return_bef_tx)
{ 
  DMA_ClearFlag(DMA_CH_SPI_RX_IT_TC);
  DMA_ClearFlag(DMA_CH_SPI_TX_IT_TC);
    
  /* Rearm the DMA_CH_SPI_TX */
  DMA_CH_SPI_TX->CMAR = (uint32_t)in_buffer;
  DMA_CH_SPI_TX->CNDTR = n_bytes;
  if(out_buffer!=NULL)
  {
    DMA_CH_SPI_RX->CMAR = (uint32_t)out_buffer;
  }
  else
  {
    DMA_CH_SPI_RX->CMAR = (uint32_t)spi_buffer_rx;
  }
  DMA_CH_SPI_RX->CNDTR = n_bytes;
  
  if(can_return_bef_tx)
  {
    NVIC_Init(&(NVIC_InitType){DMA_IRQn,MED_PRIORITY,ENABLE});
  }
  else
  {
    NVIC_Init(&(NVIC_InitType){DMA_IRQn,MED_PRIORITY,DISABLE});
  }
  
  
  GPIO_ResetBits(s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2S_SPI_CS]); 
  for(volatile uint32_t i=0;i<DELAY_CS_SCLK;i++);
  
  /* DMA_CH_SPI_TX enable */
  DMA_CH_SPI_RX->CCR_b.EN = SET;
  DMA_CH_SPI_TX->CCR_b.EN = SET;
  SPI_Cmd(ENABLE);
  
  if(!can_return_bef_tx)
  {
    while(!DMA_GetFlagStatus(DMA_CH_SPI_RX_IT_TC));
    DMA_ClearFlag(DMA_CH_SPI_RX_IT_TC);
    
    /* DMA_CH disable */
    DMA_CH_SPI_TX->CCR_b.EN = RESET;
    DMA_CH_SPI_RX->CCR_b.EN = RESET;
    while (SET == SPI_GetFlagStatus(SPI_FLAG_BSY));
    
    
    GPIO_SetBits(s_vectnM2SSpiPin[S2LPEvalGetVersion()][M2S_SPI_CS]);
    
    SPI_Cmd(DISABLE);
    
    S2LPEvalSpiRawTc();
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



/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/
