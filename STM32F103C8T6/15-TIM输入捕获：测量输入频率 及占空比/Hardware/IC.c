#include "stm32f10x.h"                  // Device header
void IC_Init(void)
{
	//开启TIM3和GPIOA的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//初始化GPIO_Pin6口,模式选择为上拉输入
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//配置内部时钟TIM3
	TIM_InternalClockConfig(TIM3);
	//初始化TIM3时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//1分频（1-1=0）不分频，时钟频率为72MHz
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//计数模式为向上计数
	//（ARR+1）=65536,自动重装器值为65536-1，测周法用于频率较低的波形，误差为±1，计次数越多越降低误差，故ARR值越大越好
	//比如100个里面有1个误差，加满100就得重装，重装的多了，误差也越累加越大，如果加满1000个重装，误差为1也只是千分之一
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;
	//(PSC+1)=72,预分频器值为72-1，标准频率为72MHz/72=1MHz，用于测量最小15Hz最大1MHz的频率
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//重复计数器（高级定时器才用），不用可以初始化为0
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	//初始化输入捕获CCR1
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;//选用为TIM3的通道1
	TIM_ICInitStructure.TIM_ICFilter = 0xF;//滤波
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//上升沿捕获
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//不分频
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//直连输入
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	//PWMIConfig专用于额外配置PWM输入捕获，用于快速配置CCR2，结构体值与通道一正相反
	TIM_PWMIConfig(TIM3, &TIM_ICInitStructure);
	//触发源选择选择为TIM_TS_TI1FP1
	TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);
	//选择从模式为Reset
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
	//使能TIM3运行控制
	TIM_Cmd(TIM3, ENABLE);
}
//获取频率：有测频法和测周法两种，频率较高时用测频法，频率较低时用测周法
//测频法：时间T内计上升沿次数N，频率Fx=N/T
//测周法：两个上升沿内，以标准频率Fc计次得到N，频率Fx=Fc/N
//中界频率（用来衡量频率较高还是较低）：测频法和测周法误差相等的频率点 Fm=根号下Fc/T
//计算标准频率为72MHz/72=1MHz=1000000Hz，用于测量最小15Hz最大1MHz的频率
//根据测周率公式得：1000000/CCR1
uint32_t IC_GetFreq(void)
{
	return 1000000 / (TIM_GetCapture1(TIM3) + 1);
}
//获取占空比：占空比为CCR2/CCR1
uint32_t IC_GetDuty(void)
{
	return (TIM_GetCapture2(TIM3) + 1) *100 / (TIM_GetCapture1(TIM3) + 1);
}
