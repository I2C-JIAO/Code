#include "main.h"                  // Device header

volatile uint8_t rx_data;           // RX buffer
Shell shell;                      // Shell object
char shell_buffer[512]; // Shell buffer

signed short _shell_read(char *data, unsigned short len)
{
	if(rx_data != 0)
	{
		*data = rx_data;
		rx_data = 0;
		return 1;
	}
	return 0;
}

signed short _shell_write(char *data, unsigned short len)
{
	for(unsigned short i = 0; i < len; i++)
	{
		my_usart_send((uint8_t) data[i]);
	}
	return len;
}

int main(void)
{
	board_init();                  // Initialize board
	my_usart_init(); 

	shell.read = _shell_read;      // Set shell read callback
	shell.write = _shell_write;    // Set shell write callback
	shellInit(&shell, shell_buffer, sizeof(shell_buffer)); // Initialize shell
	
	while(1)
	{	
		shellTask(&shell);
	}
}
