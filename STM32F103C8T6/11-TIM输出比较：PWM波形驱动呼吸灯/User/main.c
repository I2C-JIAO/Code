#include "stm32f10x.h"     // Device header
#include "Delay.h"			//包含Delay.h头文件
#include "OLED.h"
#include "Timer.h"
#include "PWM.h"
//这是STM32的第11个程序：TIM的输出比较，PWM驱动呼吸灯，另一种物理方法为串联电阻实现呼吸灯
uint16_t i;//定义i的值返回CCR
int main(void)
{
	OLED_Init();
	PWM_Init();
	OLED_ShowString(3, 1,"Breath LED" );
	while(1)
		{
			for(i=0; i<=100; i++)//因ARR为100，故CCR最大值为100，循环输出i的值返回给CCR
			{
				PWM_SetCompare1(i);
				Delay_ms(20);
			}
			for(i=0; i<=100; i++)
			{
				PWM_SetCompare1(100-i);
				Delay_ms(20);
			}
		}
}
