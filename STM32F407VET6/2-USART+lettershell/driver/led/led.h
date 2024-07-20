#ifndef __LED_H__
#define __LED_H__

#include "stm32f4xx.h"
#include <string.h>  	// for memset()

typedef struct 
{
	GPIO_TypeDef *PORT;
	uint16_t PIN;
	BitAction ON_State;
	BitAction OFF_State;
}led_desc_t;

void LED_Init(const led_desc_t *led);
void LED_ON(const led_desc_t *led);
void LED_OFF(const led_desc_t *led);
void LED_Toggle(const led_desc_t *led);

#endif /* __LED_H__ */
