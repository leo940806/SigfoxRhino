#ifndef UBIGNSSCORE_H_
#define UBIGNSSCORE_H_

/** 
 *  @file      libubignsscore.h
 *  @brief     Header file for ubignss library
 *
 *  @author    Floran Jolivel
 *  @copyright UBISCALE 2018
 */

#include "timer_driver.h"
#include "uart_driver.h"


//==============================================================================
// Public Types
//==============================================================================
/**@brief ubignss states. */
typedef enum
{
	UBIGNSS_FIX_RUNNING,
	UBIGNSS_FIX_OK,
	UBIGNSS_FIX_TIME_OUT,
	UBIGNSS_FIX_WAIT_INIT,
	UBIGNSS_FIX_CONF_ERROR
} UBIGNSS_FIX_STATE;

/**@brief time states. */
typedef enum 
{ 
	UBIGNSS_TIME_NOT_VALID, 
	UBIGNSS_TIME_TOW_ONLY,
	UBIGNSS_TIME_VALID 
} UBIGNSS_TIME_STATE;

/**@brief ubignss payload size. */
typedef enum
{
	UBIGNSS_PAYLOAD_10BYTES,
	UBIGNSS_PAYLOAD_16BYTES
}UBIGNSS_PAYLOAD_SIZE;
	
//==============================================================================
// Public structures
//==============================================================================
/**@brief time structure. */
typedef struct 
{ 
	uint16_t week_number; 
	uint32_t tow;
	uint8_t leap_second; //time GPS - time UTC
	UBIGNSS_TIME_STATE time_validity;
} gps_time_s;

//==============================================================================
// Public Functions Declarations
//==============================================================================
//==============================//
//	Integration functions
//==============================//
/**@brief Function to check the basic integration of the library.
 *
 * @param	test_value, parameter to test the function output
 *
 * @retval	1 if test_value = 0xAF, 0 if not
 */
extern uint8_t test_integration_basic(uint8_t test_value);


/**@brief Function to test the timer integration.
 *		  A 1s timer is created.
 *		  Not blocking function, have to be called in a while.
 *		  Needs at least 1s to return true.
 *
 * @param	void
 *
 * @retval  1 if timer integration is ok, 0 if not
 */
extern uint8_t test_integration_timer(void);


/**@brief Function to test the uart sending integration at 9600bauds
 *		  Frames must be seen with an oscilloscope when the function returns 1, sending must be every 500ms 
 *		  Check the baudrate : 9600
 *		  Not blocking function, have to be called in a while.
 *
 * @param  	void
 *
 * @retval  1 if the function has sent datas, 0 if not
 */
extern uint8_t test_integration_uart_tx_9600(void);


/**@brief Function to test the uart sending integration at 115200bauds
 *		  Frames must be seen with an oscilloscope when the function returns 1, sending must be every 500ms
 *		  Check the baudrate : 115200
 *		  Not blocking function, have to be called in a while.
 *
 * @param  	void
 *
 * @retval  1 if the function has sent datas, 0 if not
 */
extern uint8_t test_integration_uart_tx_115200(void);


/**
 *  @brief  Function to test the uart rx reading
 *  @param[out] test_char is the read charater
 *				First call of function will configure the uart baudrate and return 0.
 *  
 *  @retval 1 = Character read
 *  @retval 0 = RX FIFO empty.
 */
extern uint8_t test_integration_uart_rx_9600(uint8_t *test_char);


/**@brief Function to test the gps reading integration
 *		  Gps should be switched on before calling this function
 *		  Not blocking function, have to be called in a while.
 *
 * @param  	void
 *
 * @retval  1 if gps uart integration is ok, 0 if not
 */
extern uint8_t test_integration_gps_1(void);


/**@brief Function to check the gps firmware version (must be 3.0.1)
 *		  Gps should be switched on before calling this function
 *
 * @param  	void
 *
 * @retval  1 if the gps firmware version is 3.0.1, 0 if not
 */
extern uint8_t test_integration_gps_version(void);


//==============================//
//	Library functions
//==============================//
/**@brief Function to initialize the library before each fix request
 *		  Gps should be switched on before calling this function
 *
 * @param  	payload_size [in]		The requested size of the UbiGNSS payload.
 * @param  	force_time_sync [in]	Boolean to force the time synchronization.
 *									False: no time synchronization.
 *									True: force time synchronization.
 *
 * @retval  none
 */
extern void ubignss_init_after_gps_power_on(UBIGNSS_PAYLOAD_SIZE payload_size, _Bool force_time_sync);


/**@brief Function to configure the gps, process gps messages
 *		  and provide a generated payload and the time
 *		  Gps should be switched on before calling this function
 *
 * @param  	ubignss_payload [out]	Pointer to the payload generated 
 *									by the function. It is a 12 bytes array
 *			gps_time [out]			Pointer to the structure gps_time_s.		
 *
 * @retval  the function status of type UBIGNSS_FIX_STATE
 */
extern UBIGNSS_FIX_STATE ubignss_fix(uint8_t *ubignss_payload, gps_time_s *gps_time);



#endif
