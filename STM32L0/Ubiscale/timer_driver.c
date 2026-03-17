#include "timer_driver.h"

void (*ubi_timer_callback_id)();// = (void*)0x08000575;

_Bool ubignss_timer_create( void (*timer_id), void (*timeout_callback_id)())
{
	

	ubi_timer_callback_id = timeout_callback_id;
	HAL_Delay(50);
	
	return UBI_TIMER_TRUE;
}


_Bool ubignss_timer_start(void (*timer_id), uint32_t timeout_ms)
{
	_Bool ret_state;
	timer_id = UBI_TIMER;
	if(timer_id == UBI_TIMER)
	{
		TIM6->ARR = timeout_ms;
		TIM6->CR1 |= TIM_CR1_CEN;
		TIM6->CNT = 0;
		ret_state = UBI_TIMER_TRUE;
	}
	else
	{
		ret_state = UBI_TIMER_FALSE;
	}
	
	return ret_state;
	
}


_Bool ubignss_timer_stop(void (*timer_id))
{
	_Bool ret_state;
	timer_id = UBI_TIMER;
	if(timer_id == UBI_TIMER)
	{
		TIM6->CR1 &= ~(TIM_CR1_CEN);
		TIM6->CNT = 0;
		ret_state = UBI_TIMER_TRUE;
	}
	else
	{
		ret_state = UBI_TIMER_FALSE;
	}
	return ret_state;
}
