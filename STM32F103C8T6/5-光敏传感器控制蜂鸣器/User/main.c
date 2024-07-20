#include "stm32f10x.h"     // Device header
#include "Delay.h"			//包含Delay.h头文件
#include "Buzzer.h"			//包含蜂鸣器头文件
#include "LightSensor.h"	//包含光敏传感器头文件
//这是STM32的第五个程序：光敏传感器控制蜂鸣器

int main(void)
{
	Buzzer_Init();//蜂鸣器初始化
	LightSensor_Init();//光敏传感器初始化
	while(1)
	{
		if (LightSensor_Get() == 1)//调用光敏传感器状态获取函数，如果光线被遮住返回值为1，则蜂鸣器开启，反之蜂鸣器关闭
		{
			Buzzer_ON();
		}
		else
		{
			Buzzer_OFF();
		}
	}
}
