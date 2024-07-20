#include "stm32f10x.h"     // Device header
#include "Delay.h"			//包含Delay.h头文件
#include "OLED.h"
#include "Timer.h"
//这是STM32的第9个程序：定时器定时中断，学习内部定时器中断的使用
uint16_t Num;
int main(void)
{
	OLED_Init();
	Timer_Init();
	OLED_ShowString(1, 1,"Num:" );
	while(1)
		{
			OLED_ShowNum(2, 1, Num, 5);//OLED显示计数值
			OLED_ShowNum(3, 1, TIM_GetCounter(TIM2), 5);//OLED显示计数器值
		}
}
//定时器中断函数，设置变量Num++,因定时频率为1s,故计数值每隔1s+1
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)//每隔1s中断标志置1，产生中断位
	{
		Num ++;//数值加1
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//清除中断位
	}
}
