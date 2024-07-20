#include "stm32f10x.h"     // Device header
#include "Delay.h"			//包含Delay.h头文件
#include "OLED.h"
#include "CountSensor.h"
//这是STM32的第7个程序：对射式编码计次，学习了中断的应用

int main(void)
{
	OLED_Init();
	CountSensor_Init();
	OLED_ShowString(1, 1,"CountNum:" );
	while(1)
		{
			OLED_ShowNum(3, 7, Count_Num_Get(), 5);//在OLED屏幕上输出计次值
		}
}
