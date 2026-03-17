#ifndef __HALL_SENSOR_H
#define __HALL_SENSOR_H

#ifdef __cplusplus
 extern "C" {
#endif


#include "stm32l1xx_hal.h"

   
   
#define hall_port       GPIOC
#define hall_en         GPIO_PIN_10           
#define hall_sense      GPIO_PIN_11
   
   

   
void hall_gpio_init();
void hall_gpio_clk(GPIO_TypeDef* port, _Bool en);
void hall_enable(_Bool state);
_Bool hall_get();

#endif