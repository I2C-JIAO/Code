#include "stm32f10x.h"     // Device header
#include "Delay.h"			//包含Delay.h头文件
#include "OLED.h"
#include "Motor.h"
#include "Key.h"
//这是STM32的第13个程序：TIM的输出比较，PWM驱动直流电机:控制风扇速度及转向
uint8_t Num;//定义Num接收键值
int8_t Speed;//定义Speed用来传入CCR的值
int main(void)
{
	OLED_Init();
	Motor_Init();
	KEY_Init();
	OLED_ShowString(2, 1,"Electric fan" );
	OLED_ShowString(3, 1,"Speed:" );
	while(1)
		{
			//调用按键获取函数
			Num = Key_GetNum();
			if(Num == 1)//如果按键按下，速度加25
			{
				Speed +=25;
				if(Speed >100)//Speed值代表CCR的值，因为ARR设置为100，所以Speed加到最大为100
				{
					Speed = 0;//超过100，速度归0，自己改编的没有用到反转，更符合实际生活中的电风扇功能
				}
			}
			//调用电机速度函数，将Speed变量传入函数
		Motor_Speed(Speed);
		OLED_ShowNum(3, 7, Speed, 3);
		}
}
