#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Timer.h"
uint16_t i;
int main(void)
{
	OLED_Init();
	Timer_Init();
	OLED_ShowString(3, 3, "Hello LED!");
	while(1)
	{
		for( i = 0; i <= 100; i++)
		{
			PWM_SetCompare1(i);
			Delay_ms(20);
		}
		for( i = 0; i <= 100; i++)
		{
			PWM_SetCompare1(100 - i);
			Delay_ms(20);
		}
	}
}
