#include "stm32f10x.h"                  // Device header
//定时器初始化函数
void Timer_Init(void)
{
	//RCC开启APB1片上外的TIM2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	//配置内部时钟为TIM2
	TIM_InternalClockConfig(TIM2);
	//时基单元初始化：定时频率=计数器溢出频率=72M/（PSC+1）/(ARR+1)
	//这里我们需要一个1s定时的定时频率，每隔1s中断一下
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 10000-1;//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200-1;//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	//运行控制：TIM2启动向上计次
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	//清除TIM2向上计次标志位（使复位后初始值为0）
	TIM_ClearFlag(TIM2, TIM_IT_Update);
	//配置中断优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//NVIC(嵌套中断向量控制器)初始化
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	//使能TIM2
	TIM_Cmd(TIM2, ENABLE);
}
//中断调用函数最好放在需要调用函数的文件里，方便函数调用
//这里我们注释并保留，方便其他文件调用
/*void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
*/
