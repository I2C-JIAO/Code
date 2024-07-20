#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "shell.h"
#include "dht11.h"
#include "delay.h"
#include "lcd.h"


DHT11_t mydht11;
char show_buffer[100];

int cmd_mydht11(int argc, char *argv[])
{
    shellPrint(shellGetCurrent(), "DHT11 start\r\n");

	int ret = DHT11_Init(&mydht11, GPIOA, GPIO_Pin_6);
    shellPrint(shellGetCurrent(), "ret: %d\r\n", ret);

	while(DHT11_ReadData(&mydht11) != 0);

	memset(show_buffer, 0, sizeof(show_buffer));
	sprintf(show_buffer, "T: %.1f   H: %.1f\r\n", mydht11.temperature, mydht11.humidity);
    shellPrint(shellGetCurrent(), "%s\n", show_buffer);

	LCD_ShowString(18, 6, 240, 30, 24, (u8*)show_buffer);
    Delay_ms(3000);

	char hello[100];
	sprintf(hello, "Welcome to stm32!");
	LCD_ShowString(18, 6, 240, 30, 24, (u8*)hello);
	
	return 0;
}
