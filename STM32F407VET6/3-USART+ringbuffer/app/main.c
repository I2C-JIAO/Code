#include "main.h"                  // Device header


int main(void)
{
	board_init();                  // Initialize board
	my_usart_init(); 	           // Initialize USART

	while(1)
	{	
		// Check if there is any data in the RX buffer
		usart_rx_flush();

	}
}
