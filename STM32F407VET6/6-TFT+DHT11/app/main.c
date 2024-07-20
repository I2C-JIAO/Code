#include "main.h"                  // Device header


int main(void)
{
	mem_init();                 // Initialize memory
	board_init();                  // Initialize board
	my_usart_init(); 
	LCD_Init();                  // Initialize LCD
    LCD_Clear(BLUE);
    Delay_ms(1000);
    LCD_Clear(WHITE);
    Delay_ms(1000);

	char hello[100];
	sprintf(hello, "Welcome to stm32!");
	LCD_ShowString(18, 6, 240, 30, 24, (u8*)hello);
	while(1)
	{	
		// Check if there is any data in the RX buffer
		usart_rx_flush();

	}
}
