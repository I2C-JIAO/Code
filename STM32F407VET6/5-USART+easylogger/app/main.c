#include "main.h"                  // Device header

extern Shell shell;                      // Shell object
extern char shell_buffer[512]; // Shell buffer
extern signed short _shell_write(char *data, unsigned short len);
int main(void)
{
	mem_init();                 // Initialize memory
	board_init();                  // Initialize board
	my_usart_init(); 	           // Initialize USART

	shell.write = _shell_write;    // Set shell write callback
	shellInit(&shell, shell_buffer, sizeof(shell_buffer)); // Initialize shell
	while(1)
	{	
		// Check if there is any data in the RX buffer
		usart_rx_flush();

	}
}
