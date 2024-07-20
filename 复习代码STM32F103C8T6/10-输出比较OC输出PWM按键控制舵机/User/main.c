#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "PWM.h"
#include "Key.h"
uint16_t Angle, KeyNum;
int main(void)
{
	Key_Init();
	OLED_Init();
	PWM_Init();
	OLED_ShowString(1, 1, "Angle:");
	OLED_ShowNum(2, 4, 000, 3);
	OLED_ShowString(2, 7, "gle");
	while(1)
	{
		KeyNum = Get_KeyNum();
		if(KeyNum == 1)
		{
			Angle += 30;
			if(Angle > 180)
			{
				Angle = 0;
			}
		}
		Set_Angle(Angle);
		OLED_ShowNum(2, 4, Angle, 3);
	}
}
