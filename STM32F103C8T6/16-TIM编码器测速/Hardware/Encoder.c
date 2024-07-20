#include "stm32f10x.h"                  // Device header
//初始化编码器
void Encoder_Init(void)
{
	//RCC开启APB1片上外设的TIM3的时钟和APB2片上外设的GPIOA的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//初始化GPIO，选用P6和P7口专用CH1和CH2，选择上拉输入：默认高阻态，高电平，低电平输入
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//时基单元初始化：这里时钟选择不分频，模式为向上计数，自动重装器重装值拉满，满量程计数，这样计数范围最大，方便切换为负数
	//预分频器控制CNT计数器的时钟，这里我们设置为1，不分频，编码器时钟直接驱动计数器
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1-1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	//输入捕获初始化，这里我们只用到通道选择和滤波器
	TIM_ICInitTypeDef TIM_ICInitStructure;
	//因只用到结构体部分成员功能，所以给其他功能一个默认初始值，初始化结构体，避免出现变量给任意值的情况
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	//专用的编码器接口配置函数，配置为12通道都计数，设置为上升沿，如果想更换相反方向，则更换为Falling
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	//运行控制，使能TIM3
	TIM_Cmd(TIM3, ENABLE);
}
//获取编码器计数函数，实现测频法测速
//测频法：时间T内计上升沿次数N Fx = N / T
int16_t Encoder_Get(void)
{
	//定义一个变量用来接收计次数，接收完之后计次数清0
	int16_t Temp;
	Temp = TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3, 0);
	return Temp;//返回计次累加数
}
