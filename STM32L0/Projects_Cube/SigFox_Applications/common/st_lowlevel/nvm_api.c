#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nvm_api.h"

#if  !(defined(BLUENRG2_DEVICE) || defined(BLUENRG1_DEVICE))
#include "SDK_EVAL_Config.h"
#else
#include "S2LP_EVAL_Config.h" 
#endif   

static volatile NVM_TYPE _nvmType = NVM_TYPE_EEPROM;
static volatile uint8_t  _msgSq = 0x00;
static volatile uint32_t _blockPointer;
static volatile uint32_t _userAddress;
static volatile uint32_t _dataAddress;

static NVM_RW_RESULTS _setBlockState(uint32_t addr, uint64_t state)
{
  NVM_RW_RESULTS tRet = NVM_RW_OK;
  uint64_t wrState;
  
  if(state == NVM_BLOCK_INVALID)
  {
    wrState = NVM_BLOCK_INVALID_2;
    tRet = NVM_Write(addr, BLOCK_HEADER_SIZE/2, (uint8_t*)&wrState, NVM_WRITE_MODE_WRITEOVER);	
  }
  else if(state == NVM_BLOCK_VALID)
  {
    wrState = NVM_BLOCK_VALID_2;
    tRet = NVM_Write(addr+BLOCK_HEADER_SIZE/2, BLOCK_HEADER_SIZE/2, (uint8_t*)&wrState, NVM_WRITE_MODE_WRITEOVER);
  }
  else
    tRet = NVM_WRITE_HEADER_ERROR;
  
  return tRet;
}

static NVM_RW_RESULTS _writeRecordInternal(uint32_t addr, uint8_t* nvmRecord, uint32_t recordSize)
{
  NVM_RW_RESULTS tRet = NVM_RW_OK;
  
  tRet = _setBlockState(addr, NVM_BLOCK_INVALID);
  
  if(tRet == NVM_RW_OK)
    tRet = NVM_Write(addr+BLOCK_HEADER_SIZE, recordSize, (uint8_t *)nvmRecord, NVM_WRITE_MODE_WRITEOVER);	
  
  if(tRet == NVM_RW_OK)
    tRet = _setBlockState(addr, NVM_BLOCK_VALID);
  
  return tRet;
}

void SetNVMInitial(NVM_ConfigType *config)
{
  _nvmType = config->nvmType;
  _userAddress = config->userSpaceAddress;
  _dataAddress = config->dataPageAddress;
  _msgSq = config->messageSequenceNumber;
  
  /* Initialize FLASH user data for first boot */
  if(_nvmType == NVM_TYPE_FLASH)
  {
    uint8_t cleanBuf[BLOCK_BODY_SIZE];
    
    if (NVM_ReadRecord(cleanBuf, BLOCK_BODY_SIZE) == NVM_NO_RECORDS)
    {
      memset(cleanBuf, 0x00, BLOCK_BODY_SIZE);
      FlashErase(_userAddress, 1);
      NVM_WriteRecord(&cleanBuf[0], BLOCK_BODY_SIZE);  
    }
  }
}

NVM_TYPE GetNVMType()
{
  return _nvmType;
}


NVM_RW_RESULTS NVM_Read(uint32_t nAddress, uint8_t cNbBytes, uint8_t* pcBuffer)
{
  NVM_RW_RESULTS tRet = NVM_RW_OK;
  FLS_RW_StatusTypeDef flRet = FLS_RW_OK;
  
  if(_nvmType == NVM_TYPE_EEPROM)
    EepromRead(nAddress, cNbBytes, pcBuffer);
  else
    flRet = FlashRead(nAddress, cNbBytes, &pcBuffer[0]);
  
  if(flRet != FLS_RW_OK)
    tRet = NVM_READ_ERROR;
  
  return tRet;
}

NVM_RW_RESULTS NVM_Write(uint32_t nAddress, uint8_t cNbBytes, uint8_t* pcBuffer, NVM_WRITE_MODE writeMode)
{
  NVM_RW_RESULTS tRet = NVM_RW_OK;
  FLS_RW_StatusTypeDef flRet = FLS_RW_OK;
  
  if(_nvmType == NVM_TYPE_EEPROM)
    EepromWrite(nAddress, cNbBytes, pcBuffer);
  else	
    flRet = FlashWrite(nAddress, cNbBytes, pcBuffer, writeMode);
  
  if(flRet != FLS_RW_OK)
    tRet = NVM_WRITE_ERROR;
  
  return tRet;
}

NVM_RW_RESULTS NVM_ReadRecord(uint8_t* nvmRecord, uint32_t recordSize)
{	
  NVM_RW_RESULTS tRet = NVM_RW_OK;
  
  uint64_t currentBlockState;	
  currentBlockState = NVM_BLOCK_INVALID;
  
  _blockPointer = _userAddress;
  
  if (_nvmType == NVM_TYPE_FLASH)
  {				
    while(currentBlockState != FLASH_EMPTY_BLOCK)
    {		
      if(NVM_Read(_blockPointer, BLOCK_HEADER_SIZE, (uint8_t*)&currentBlockState) == NVM_RW_OK)
      {
	if(_blockPointer == (uint32_t)FLASH_USER_END_ADDR)
	{
	  break;	
	}
	
	_blockPointer += (BLOCK_BODY_SIZE+BLOCK_HEADER_SIZE);
      }
      else
      {
	tRet = NVM_READ_RECORD_ERROR;
	break;
      }
    }
    
    if(tRet == NVM_RW_OK)
    {
      while(currentBlockState != NVM_BLOCK_VALID && tRet == NVM_RW_OK)
      {	
	_blockPointer -= (BLOCK_BODY_SIZE+BLOCK_HEADER_SIZE);
	
	if(_blockPointer < (uint32_t)FLASH_USER_START_ADDR)
	{
	  tRet = NVM_NO_RECORDS;
	  break;
	}
	
	tRet = NVM_Read(_blockPointer, BLOCK_HEADER_SIZE, (uint8_t*)&currentBlockState);
      }
      
      if(tRet == NVM_RW_OK)
	tRet = NVM_Read(_blockPointer+BLOCK_HEADER_SIZE, recordSize, (uint8_t *)nvmRecord);
      else if (tRet != NVM_NO_RECORDS)
	tRet = NVM_READ_RECORD_ERROR;
    }
  }
  else
    tRet = NVM_Read(_blockPointer, recordSize, (uint8_t *)nvmRecord);
  
  return tRet;
}

NVM_RW_RESULTS NVM_WriteRecord(uint8_t* nvmRecord, uint32_t recordSize)
{
  NVM_RW_RESULTS tRet = NVM_RW_OK;
  uint64_t currentBlockState;
  currentBlockState = NVM_BLOCK_INVALID;
  
  _blockPointer = _userAddress;
  
  if (_nvmType == NVM_TYPE_FLASH)
  {
    while(currentBlockState != FLASH_EMPTY_BLOCK)
    {
      if(_blockPointer != (uint32_t)FLASH_USER_END_ADDR)
      {
	if(NVM_Read(_blockPointer, BLOCK_HEADER_SIZE, (uint8_t *)&currentBlockState) == NVM_RW_OK)
	{
	  if(currentBlockState == FLASH_EMPTY_BLOCK)
	  {
	    tRet = _writeRecordInternal(_blockPointer, nvmRecord, recordSize);
	    break;
	  }
	  else			
	    _blockPointer += (BLOCK_BODY_SIZE+BLOCK_HEADER_SIZE);
	}
	else
	{
	  tRet = NVM_READ_RECORD_ERROR;
	  break;
	}				
      }
      else
      {
	_blockPointer = (uint32_t)FLASH_USER_START_ADDR;
	FlashErase(_blockPointer, 1);
	tRet = _writeRecordInternal(_blockPointer, nvmRecord, recordSize);
	
	break;
      }
    }
  }
  else
    tRet = NVM_Write(_blockPointer, recordSize, nvmRecord, NVM_WRITE_MODE_WRITEOVER);
  
  
  return tRet;
}

NVM_RW_RESULTS NVM_ReadBoardData(NVM_BoardDataType *data)
{
  if(_nvmType == NVM_TYPE_EEPROM)
    return NVM_READ_ERROR;
  else
    return NVM_Read(_dataAddress, sizeof(NVM_BoardDataType), (uint8_t*)data);
}

NVM_RW_RESULTS NVM_WriteBoardData(NVM_BoardDataType *data)
{
  return NVM_Write(_dataAddress, sizeof(NVM_BoardDataType), (uint8_t*)data, NVM_WRITE_MODE_ERASE);
}

NVM_RW_RESULTS NVM_UpdateBoardData(NVM_BoardDataType *data, uint8_t updateWhat)
{	
  NVM_BoardDataType _bd;
  NVM_RW_RESULTS res = NVM_ReadBoardData(&(_bd));
  
  if(res == NVM_RW_OK)
  {
    if(updateWhat & NVM_BOARD_ID)
      _bd.id = data->id;
    if(updateWhat & NVM_BOARD_RCZ)
      _bd.rcz = data->rcz;
    if(updateWhat & NVM_BOARD_PAC)
      memcpy(&(_bd.pac[0]), data->pac, 8);
    if(updateWhat & NVM_BOARD_FREQ)
      _bd.freqOffset = data->freqOffset;
    if(updateWhat & NVM_BOARD_RSSI)
      _bd.rssiOffset = data->rssiOffset;
    
    res = NVM_WriteBoardData(&(_bd));
  }
  
  return res;
}

