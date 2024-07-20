#include "stm32f10x.h"     // Device header
#include "Delay.h"			//包含Delay.h头文件
#include "OLED.h"
#include "Timer.h"
//这是STM32的第10个程序：定时器定时中断，用对射式编码器来学习外部定时器中断的使用
uint16_t Num;
int main(void)
{
	OLED_Init();
	Timer_Init();
	OLED_ShowString(1, 1,"Num:" );
	OLED_ShowString(2, 1,"CNT:" );
	while(1)
		{
			OLED_ShowNum(1, 5, Num, 5);
			OLED_ShowNum(2, 5, TIM_GetCounter(TIM2), 5);
		}
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Num ++;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
