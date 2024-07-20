#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Timer.h"

uint16_t Num;
int main(void)
{
	OLED_Init();
	Timer_Init();
	OLED_ShowString(1, 1, "Number:");
	
	while(1) 
	{	
		OLED_ShowNum(2, 6, Num, 6);
		OLED_ShowString(3,3,"Hello Bitch!");
		OLED_ShowNum(4, 6, TIM_GetCounter(TIM2), 6);
	}
}
void TIM2_IRQHandler(void)
{
	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Num ++;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
