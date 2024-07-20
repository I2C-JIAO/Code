#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"

void PWM_Init(void);
void PWM_SetDutyCycle(uint16_t DutyCycle);
void PWM_SetPrescaler(uint16_t Prescaler);

#endif
