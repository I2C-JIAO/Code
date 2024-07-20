#include "led.h"

void LED_Init(const led_desc_t *led)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    memset (&GPIO_InitStructure, 0, sizeof(GPIO_InitStructure)); 
	GPIO_InitStructure.GPIO_Pin = led->PIN;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_Init(led->PORT, &GPIO_InitStructure); 

	GPIO_WriteBit(led->PORT, led->PIN, led->OFF_State);
}

void LED_ON(const led_desc_t *led)
{   
    GPIO_ResetBits(led->PORT, led->PIN); 
}

void LED_OFF(const led_desc_t *led)
{
    GPIO_SetBits(led->PORT, led->PIN); 
}

void LED_Toggle(const led_desc_t *led)
{
    GPIO_ToggleBits(led->PORT, led->PIN); 
}
