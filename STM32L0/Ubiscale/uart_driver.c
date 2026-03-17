#include "uart_driver.h"
#include "gps.h"

void (*ubi_uart_callback_id)();

_Bool ubignss_uart_configure(void (*uart_callback_id)(), uint32_t baudrate)
{
 	ubi_uart_callback_id = uart_callback_id;
	gps_baud_set(baudrate);
	HAL_Delay(10);
	
	return 1;
}
	
_Bool ubignss_uart_send(uint8_t *data, uint32_t size)
{
	if(size != 0)
	{
		gps_uart_tx(data, size);

		return 1;
	}
	else 
	{
		return 0;
	}
}

_Bool ubignss_uart_read_char(uint8_t *char_to_read)
{
//	if((gps_uart.Instance->ISR & USART_ISR_RXNE) == USART_ISR_RXNE)
//	{
	*char_to_read = gps_uart.Instance -> RDR;

	gps_uart.Instance->CR1 |= USART_CR1_RXNEIE;
	return 1;
//	}	
}

