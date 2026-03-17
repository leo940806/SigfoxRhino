#ifndef TIMER_DRIVER_H_
#define TIMER_DRIVER_H_


#include "main.h"

#define UBI_TIMER_TRUE	1
#define UBI_TIMER_FALSE	0
#define UBI_TIMER TIM6

extern void (*ubi_timer_callback_id)();

/**@brief 	The timer_create function creates a timer that will run endlessly. It also associates the callback function that will be executed when the timer expires.
	*					
	*@param 	timer_id [in] Pointer to the timer id.
	*@param 	timeout_callback_id[in] Pointer to the library function to be called when the timer expires.
	*
	*@retval 	0 Fail
	* 				1	Succeed			
	*/
extern _Bool ubignss_timer_create( void (*timer_id), void (*timeout_callback_id)());
	

/**@brief 	The timer_start function starts the timer that will expires after the given time and calls the UbiGNSS callback.
	*						
	*@param 	timer_id [in] Pointer to the timer id.
	*@param 	timeout_ms[in] Timeout of timer in milliseconds.
	*	
	*@retval 		0 Fail
	* 					1	Succeed	
	* 				
	*/
extern _Bool ubignss_timer_start(void (*timer_id), uint32_t timeout_ms);
	

/**@brief 	The timer_stop function stops the timer.
	*					
	*@param 		timer_id [in] Pointer to the timer id.
	*		
	*@retval 		0 Fail
	* 					1	Succeed						
	* 				
	*/
extern _Bool ubignss_timer_stop(void (*timer_id));










#endif

