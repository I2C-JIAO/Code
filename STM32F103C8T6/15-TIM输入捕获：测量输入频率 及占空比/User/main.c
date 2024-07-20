#include "stm32f10x.h"     // Device header
#include "Delay.h"			//包含Delay.h头文件
#include "OLED.h"
#include "PWM.h"
#include "IC.h"
//这是STM32的第15个程序：TIM的输入捕获，用PA6口捕获PA0口的PWM波形，并计算出频率和占空比
int main(void)
{
	OLED_Init();
	PWM_Init();
	IC_Init();
	
	OLED_ShowString(1, 1, "Freq:00000Hz");
	OLED_ShowString(2, 1, "Duty:00%");
	PWM_SetPrescaler(720-1);//设置PWM频率
	PWM_SetCompare1(30);//设置PWM输出比较器的值（占空比）
	
	while(1)
		{
			OLED_ShowNum(1, 6, IC_GetFreq(),5);//获取输入频率
			OLED_ShowNum(2, 6, IC_GetDuty(),2);//获取占空比
		}
}
