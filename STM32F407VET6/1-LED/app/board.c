#include "board.h"
const led_desc_t led0 = 
{
    .CLK_Source = RCC_AHB1Periph_GPIOE,
    .PORT = GPIOE,
    .PIN = GPIO_Pin_5,
    .ON_State = Bit_RESET,
    .OFF_State = Bit_SET,
};
const led_desc_t led1 = 
{
    .CLK_Source = RCC_AHB1Periph_GPIOE,
    .PORT = GPIOE,
    .PIN = GPIO_Pin_6,
    .ON_State = Bit_RESET,
    .OFF_State = Bit_SET,
};
const led_desc_t led2 = 
{
    .CLK_Source = RCC_AHB1Periph_GPIOC,
    .PORT = GPIOC,
    .PIN = GPIO_Pin_13,
    .ON_State = Bit_RESET,
    .OFF_State = Bit_SET,
};
