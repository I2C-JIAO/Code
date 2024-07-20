#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "PWM.h"
#include "Delay.h"
#include "Key.h"
uint16_t KeyNum, i, Speed;
int main(void)
{
	Key_Init();
	OLED_Init();
	PWM_Init();
	
	OLED_ShowString(2, 2, "Xiao Feng Shan");
	OLED_ShowString(3, 2, "Speed:");
	OLED_ShowNum(3, 8, 000, 3);
	while(1)
	{
		KeyNum = Get_KeyNum();
		if(KeyNum == 1)
		{
			Speed += 25;
			if(Speed > 100)
			{
				Speed = 0;
			}
			Set_Speed(Speed);
			OLED_ShowNum(3, 8, Speed, 3);
		}
	}
}
