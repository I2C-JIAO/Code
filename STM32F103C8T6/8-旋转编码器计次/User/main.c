#include "stm32f10x.h"     // Device header
#include "Delay.h"			//包含Delay.h头文件
#include "OLED.h"
#include "Encoder.h"
//这是STM32的第8个程序：旋转编码器计次，本程序复习中断的应用
//定义一个Num用来接收获取到的计次数据
int16_t Num;
int main(void)
{
	OLED_Init();
	Encoder_Init();
	OLED_ShowString(1, 1,"CountNum:" );
	while(1)
		{
			Num += Encoder_Num();//累加计次数据
			OLED_ShowSignedNum(2, 1, Num,5);//OLED显示屏输出计次数据（有符号整数）
		}
}
