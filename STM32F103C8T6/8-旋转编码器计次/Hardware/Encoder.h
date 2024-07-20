#ifndef __ENCODER_H
#define __ENCODER_H
void Encoder_Init(void);
uint16_t Encoder_Num(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
#endif
