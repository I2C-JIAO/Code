#include "stm32f10x.h"                  // Device header
#include "OLED.h"

int main(void)
{
	OLED_Init();
	
	OLED_ShowString(3, 3, "Hello Bitch!");
	while(1)
	{
		
	}
}
