#include "stm32f10x.h"                  // Device header
//初始化PWM
void PWM_Init(void)
{
	//开启APB1的TIM2外设以及APB2的GPIOA的外设
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//初始化GPIOA,将Pin_2口设置为复用推挽输出模式，复用为TIM2的CH3通道
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//配置TIM2的内部时钟，并初始化时基单元
	TIM_InternalClockConfig(TIM2);
	//为了避免电机噪声，根据人耳的听力频率范围20Hz到20000Hz之间，需要将电机驱动频率设置为20000Hz
	//时钟频率(72MHz)/（预分频器（PSC）+1）/（自动重装器（ARR）+1）=20000Hz即可，
	//PSC和ARR的值可在计算结果不变的情况下任意分配，为了方便计算，将自动重装器（ARR）值设置为100-1
	//预分频器（PSC）值设置为36-1，以达到消除电机噪声的目的
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//分频模式选择不分频，依然是72Mhz
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//计数模式选择向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 100-1;//自动重装器（ARR）值设置为100-1
	TIM_TimeBaseInitStructure.TIM_Prescaler = 36-1;//预分频器（PSC）值设置为36-1
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//重复计数器为高级定时器所需，用不到初始化为0即可
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	//输出比较寄存器初始化
	TIM_OCInitTypeDef TIM_OCInitStructure;
	//初始化输出比较寄存器结构体，因只用到输出比较的部分功能，其他功能未初始化会给任意值，
	//避免影响程序正常运行，先初始化结构体，再将用到的部分功能单独赋值
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//设置输出比较为PWM1模式
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//设置向上计数：CNT<CCR时，REF置高电平
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//设置输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;//设置CCR默认值为0
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);//PA2口对应为通道3
	//运行控制：使能TIM2
	TIM_Cmd(TIM2, ENABLE);
}
//定义PWM_SetCompare3函数设置CCR值
void PWM_SetCompare3(uint16_t Compare)
{
	TIM_SetCompare3(TIM2, Compare);//设置通道3的CCR
}
