#include "stm32f10x.h"     // Device header
#include "Delay.h"			//包含Delay.h头文件
#include "OLED.h"
//这是STM32的第6个程序：OLED显示屏，该程序主要学习OLED函数的应用

int main(void)
{
	OLED_Init();
	OLED_ShowChar(1, 1 , 'A');
	OLED_ShowString(2, 1,"Hello world!" );
	OLED_ShowNum(3, 1, 12345, 5);
	OLED_ShowSignedNum(3, 7, -66, 2);
	OLED_ShowHexNum(4, 1, 0xAA55, 5);
	OLED_ShowBinNum(1, 2, 0xAA55, 16);
	while(1)
		{
		}
}
