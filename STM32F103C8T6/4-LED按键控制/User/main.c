#include "stm32f10x.h"     // Device header
#include "Delay.h"			//包含Delay.h头文件
#include "LED.h"			//包含LED头文件
#include "Key.h"			//包含按键头文件
//这是STM32的第四个程序：LED的按键控制
uint8_t KeyNum;//定义一个int型的全局变量KeyNum

int main(void)
{
	LED_Init();//初始化LED
	KEY_Init();//初始化按键
	
	while(1)
		{
			KeyNum = Key_GetNum();//获取按键值
			if (KeyNum ==1)//如果按键值为1
			{
				LED1_Turn();//LED1状态翻转（开灯变关灯，关灯变开灯）
			}
			if(KeyNum ==2)//如果按键值为2
			{
				LED2_Turn();//LED2状态翻转（开灯变关灯，关灯变开灯）
			}
		}
}
