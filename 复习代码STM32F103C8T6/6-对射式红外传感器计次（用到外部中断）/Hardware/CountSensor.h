#ifndef __COUNTSENSOR_H
#define __COUNTSENSOR_H

void CountSensor_Init(void);
uint8_t Count_Get(void);
void EXTI15_10_IRQHandler(void);
#endif
