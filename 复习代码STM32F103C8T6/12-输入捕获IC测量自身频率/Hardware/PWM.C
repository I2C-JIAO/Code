#include "PWM.h"

void PWM_Init(void)
{
    // 使能PWM时钟及GPIO时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    //配置GPIOA_Pin0为复用推挽输出
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //设置时钟源为内部时钟TIM2
    TIM_InternalClockConfig(TIM2);
    // 配置TIM2
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    // 设置时钟源为系统时钟
    TIM_TimeBaseStructure.TIM_Prescaler = 720 - 1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 100 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    // 配置输出比较
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;// 初始化占空比（CCR的值）为0
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);

    // 使能定时器2
    TIM_Cmd(TIM2, ENABLE);
}


void PWM_SetDutyCycle(uint16_t DutyCycle)
{
    // 设置占空比
    TIM_SetCompare1(TIM2, DutyCycle);
}


void PWM_SetPrescaler(uint16_t Prescaler)
{
    // 设置预分频值
    TIM_PrescalerConfig(TIM2, Prescaler, TIM_PSCReloadMode_Immediate);

}
