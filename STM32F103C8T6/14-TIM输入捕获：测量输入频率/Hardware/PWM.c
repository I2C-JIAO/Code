#include "stm32f10x.h"                  // Device header
//PWM初始化函数
void PWM_Init(void)
{
	//开启TIM2和GPIOA的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//初始化GPIO_Pin0口,模式选择为复用推挽输出，将P0口复用为TIM2通道一
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//配置内部时钟TIM2,初始化时基单元，计算TIM2的频率为：
	//时钟频率/(预分频器值（PSC）+1)/(自动重装器值(ARR)+1)=72MHz（72000000）/720/100=1000Hz=1KHz
	TIM_InternalClockConfig(TIM2);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//1分频（1-1=0）不分频，时钟频率为72MHz
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//计数模式为向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 100-1;//（ARR+1）=100,自动重装器值为100-1
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720-1;//(PSC+1)=720,预分频器值为720-1
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//重复计数器（高级定时器才用），不用可以初始化为0
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	//初始化输出比较器CCR
	TIM_OCInitTypeDef TIM_OCInitStructure;
	//初始化输出比较器结构体，因只用到部分功能，避免其他未配置功能给任意值影响电平，故需初始化结构体后再给部分功能单独赋值
	TIM_OCStructInit(&TIM_OCInitStructure);//输出比较器结构体初始化
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//输出比较模式选择为PWM模式1，1和2功能正相反
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//模式1向上计数：CNT<CCR时置有效电平
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//设置输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;//设置CCR的值，初始化为0
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);//PA0口对应OC1通道
	
	TIM_Cmd(TIM2, ENABLE);//运行控制，使能TIM2
}
//TIM_SetCompare1函数设置CCR的值
void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM2, Compare);
}
//TIM_PrescalerConfig函数设置预分频器PSC的值
void PWM_SetPrescaler(uint16_t Prescaler)
{
	TIM_PrescalerConfig(TIM2, Prescaler, TIM_PSCReloadMode_Immediate);
}
