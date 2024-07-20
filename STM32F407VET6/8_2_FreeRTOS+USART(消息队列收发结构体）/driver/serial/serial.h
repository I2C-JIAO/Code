#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>
#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"


void Serial_Init(uint32_t baud_rate);
void Serial_Send(uint8_t data);

#endif
