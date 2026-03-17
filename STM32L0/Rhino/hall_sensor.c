#include "hall_sensor.h"



/* --------------------------------------------------------------------------*/
void hall_gpio_clk(GPIO_TypeDef* port, _Bool en)
{
  if(port == GPIOA)
  {
      if(en)
      {
        __HAL_RCC_GPIOA_CLK_ENABLE();
      }
      else if(!en)
      {
        __HAL_RCC_GPIOA_CLK_DISABLE();
      }
      
  }
 else if(port == GPIOB)
  {
      if(en)
      {
        __HAL_RCC_GPIOB_CLK_ENABLE();
      }
      else if(!en)
      {
        __HAL_RCC_GPIOB_CLK_DISABLE();
      }

  }
  else if(port == GPIOC)
  {
      if(en)
      {
        __HAL_RCC_GPIOC_CLK_ENABLE();
      }
      else if(!en)
      {
        __HAL_RCC_GPIOC_CLK_DISABLE();
      }
       
  }
  else if(port == GPIOD)
  {
      if(en)
      {
        __HAL_RCC_GPIOD_CLK_ENABLE();
      }
      else if(!en)
      {
        __HAL_RCC_GPIOD_CLK_DISABLE();
      }
       
  }
 else if(port == GPIOH)
  {
      if(en)
      {
        __HAL_RCC_GPIOH_CLK_ENABLE();
      }
      else if(!en)
      {
        __HAL_RCC_GPIOH_CLK_DISABLE();
      }
  }  
}

/* --------------------------------------------------------------------------*/
void hall_gpio_init()
{
  hall_gpio_clk(hall_port, 1);
  
  HAL_GPIO_WritePin(hall_port, hall_en, GPIO_PIN_RESET);
  
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = hall_sense;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(hall_port, &GPIO_InitStruct);
  
  
  GPIO_InitStruct.Pin = hall_en;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(hall_port, &GPIO_InitStruct);
}
/* --------------------------------------------------------------------------*/
void hall_enable(_Bool state)
{
  HAL_GPIO_WritePin(hall_port, hall_en, (GPIO_PinState)state);
}
/* --------------------------------------------------------------------------*/
_Bool hall_get()
{
  return(HAL_GPIO_ReadPin(hall_port, hall_sense));
}

