#include "board.h"

void board_init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //设置中断优先级分组4
    //使能GPIOE,GPIOC时钟,使能USART2时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOC, ENABLE); 

}

const led_desc_t led0 = 
{
    .PORT = GPIOE,
    .PIN = GPIO_Pin_5,
    .ON_State = Bit_RESET,
    .OFF_State = Bit_SET,
};

const led_desc_t led1 = 
{
    .PORT = GPIOE,
    .PIN = GPIO_Pin_6,
    .ON_State = Bit_RESET,
    .OFF_State = Bit_SET,
};

const led_desc_t led2 = 
{
    .PORT = GPIOC,
    .PIN = GPIO_Pin_13,
    .ON_State = Bit_RESET,
    .OFF_State = Bit_SET,
};
