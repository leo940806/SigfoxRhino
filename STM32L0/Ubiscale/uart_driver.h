#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_


#include "main.h"

extern void (*ubi_uart_callback_id)();


/**@brief 	The uart_configure function configures the UART with the given baudrate and 
	*					associates the callback function that will be called when there are some data in the UART buffer.
	*					The uart_configure function returns true if the UART was successfully configured, 
	*					otherwise it returns false.
	*
	*@param 	uart_callback_id [in] Pointer to the library function to be called 
	*					when there are some data in the UART buffer.
	*@param 	baudrate[in] Baudrate of UART.
	*
	*@retval 	0 Fail
	* 				1	Succeed
	*/
extern _Bool ubignss_uart_configure(void (*uart_callback_id)(), uint32_t baudrate);

/**@brief 	The uart_send function adds the given message to the TX buffer.
	*					The uart_send function returns true if the message was successfully sent, otherwise it returns false.
	*
	*@param		data [in] Pointer to the data to send.
	*@param 	size[in] The size of the message to send.
	*
	*@retval 	0 Fail
	* 				1	Succeed
	*/
extern _Bool ubignss_uart_send(uint8_t *data, uint32_t size);


/**@brief 	The uart_read_char function returns the first character of the RX FIFO. 
	*					This function would be called in the UART callback.
	*					The uart_read_char function returns true if the character was successfully read, 
	*					it returns false if the RX FIFO is empty.
	*
	*@param		char_to_read [out] Pointer the data to read (only one character at once).
	*					
	*@retval 	0 Fail
	* 				1	Succeed			
	*/
extern _Bool ubignss_uart_read_char(uint8_t *char_to_read);









#endif

