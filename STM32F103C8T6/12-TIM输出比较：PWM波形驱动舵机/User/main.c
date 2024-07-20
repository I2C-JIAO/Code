#include "stm32f10x.h"     // Device header
#include "Delay.h"			//包含Delay.h头文件
#include "OLED.h"
#include "Key.h"
#include "Servo.h"
//这是STM32的第11个程序：TIM的输出比较，PWM驱动舵机，舵机引脚接PA1口，按键接PB1口
uint8_t KeyNum;//定义KeyNum变量接收键值
float Angle;//定义Angle变量接收角度
int main(void)
{
	OLED_Init();
	KEY_Init();//一定要初始化按键，不然就会出现手指靠近按键，舵机角度跳变的情况
	Servo_Init();//舵机初始化函数里包含了PWM初始化，不再需要单独初始化PWM
	OLED_ShowString(1, 1,"Servo Energy" );
	OLED_ShowString(2, 1,"Angle:" );
	while(1)
		{
			//调用获取键值函数
			KeyNum = Key_GetNum();
			if (KeyNum ==1)
			{
				Angle +=30;
				if (Angle > 180)
				{
					Angle =0;
				}
			}
			//调用舵机设置角度函数，Angle值返回给函数
			Servo_SetAngle(Angle);
			OLED_ShowNum(3, 1, Angle, 3);
		}
}
