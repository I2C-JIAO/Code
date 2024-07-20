#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Console_Usart.h"
#include "shell.h"
#include "ringbuffer8.h"
#include "main.h"

static uint8_t rx_buff[128];
static ringbuffer8_t rxrb;
static Shell shell;
static char shell_buffer[256];


signed short _shell_write(char *data , unsigned short len)
{
	for (unsigned short i = 0; i < len; i++)
	{
		usart_send_byte((uint8_t)data[i]);
	}
	return len;
}

static void usart_rx_handler(uint8_t data)
{
	if(!rb8_full(rxrb))
	{
		rb8_put(rxrb, data);
	}
}

int main(void)
{
	mem_init();

	borad_init();
	usart_init();
	usart_callback_register(usart_rx_handler);

	rxrb = rb8_new(rx_buff, sizeof(rx_buff));

	shell.write = _shell_write;
	shellInit(&shell, shell_buffer, sizeof(shell_buffer));
	
	uint8_t rxdata;
	while(1)
	{
		if(!rb8_empty(rxrb))
		{
			rb8_get(rxrb, &rxdata);
			shellHandler(&shell, rxdata);
		}
	}
}
