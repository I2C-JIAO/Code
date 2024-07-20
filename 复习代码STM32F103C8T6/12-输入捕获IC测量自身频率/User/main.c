#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "PWM.h"
#include "IC.h"
int main(void)
{
	OLED_Init();
	PWM_Init();
	IC_Init();
	OLED_ShowString(1, 1, "Freq:000000Hz");

	PWM_SetPrescaler(72 - 1);
	PWM_SetDutyCycle(50);
	while(1)
	{
		OLED_ShowNum(1, 6, IC_GetFreq(), 5);
	}
}
