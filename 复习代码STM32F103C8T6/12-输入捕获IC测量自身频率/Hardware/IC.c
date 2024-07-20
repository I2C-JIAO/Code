#include "IC.h"

void IC_Init(void)
{
    //开启TIM3时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    //设置GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    //初始化GPIOA_Pin6为上拉输入
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //设置时钟源为内部时钟TIM3
    TIM_InternalClockConfig(TIM3);

    //初始化时基单元TIM3
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 65536 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    //初始化输入捕获单元IC1
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0xF;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);

    //触发源选择
    TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);

    //选择从模式
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);

    //使能TIM3
    TIM_Cmd(TIM3, ENABLE);
}

//获取IC1捕获的频率
uint32_t IC_GetFreq(void)
{
    //获取IC1捕获的频率
    return 1000000 / (TIM_GetCapture1(TIM3) + 1);
}
