#include "main.h"                  // Device header

int main(void)
{
	LED_Init(&led0);
	LED_Init(&led1);
	LED_Init(&led2);
	while(1)
	{
		LED_ON(&led0);
		Delay_ms(500);
		LED_OFF(&led0);
		LED_ON(&led1);
		Delay_ms(500);
		LED_OFF(&led1);
		LED_ON(&led2);
		Delay_ms(500);
		LED_OFF(&led2);
		Delay_ms(500);
	}
}
