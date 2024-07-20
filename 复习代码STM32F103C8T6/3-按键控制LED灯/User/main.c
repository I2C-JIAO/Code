#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Key.h"
#include "Led.h"
uint8_t KeyNum;
int main(void)
{
	Key_Init();
	LED_Init();
	
	while(1)
	{
		KeyNum = Get_KeyNum();
		if(KeyNum == 1)
		{
			LED_Turn();
		}
		
	}
}
