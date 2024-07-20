#include "stm32f10x.h"     // Device header
#include "Delay.h"			//包含Delay.h头文件
#include "OLED.h"
#include "Timer.h"
#include "Encoder.h"
//这是STM32的第16个程序：TIM编码器接口测速，使用TIM3接收编码器信号，使用TIM2内部中断来减少使用Delay函数
int16_t Speed;//定义速度接收变量
int main(void)
{
	OLED_Init();
	Encoder_Init();
	Timer_Init();
	OLED_ShowString(1, 1,"Speed:" );
	while(1)
		{
			OLED_ShowSignedNum(3, 1, Speed, 5);
		}
}
//中断函数实现1s内计次计算速度用来代替Delay_ms(1000);
//原理：Encoder_Get()函数用来返回计次数，中断函数用来发生一个每隔1s执行一次的中断
//返回值为每s计数器的值，给定一个速度变量Speed用来接收每秒的计次数
//简单来说，Encoder_Get()用来计次，次数累加，TIM2_IRQHandler(void)用来计时每隔多长时间停一次计次
//就实现了：累计次数/单位时间=速度的原理，单位时间的速度也就是频率
//频率就是周期性事件在单位时间内发生的次数
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)//每隔1s中断标志置1，产生中断位
	{
		Speed = Encoder_Get();//计数器给Speed变量
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//清除中断位
	}
}
