#ifndef __CONSOLE_USART_H
#define __CONSOLE_USART_H

#ifndef STM32F10X_MD
#define STM32F10X_MD  //定义STM32F10X_MD
#endif

#include "stm32f10x.h"                  // Device header

typedef void (*UsartCallback)(uint8_t data);

void usart_callback_register(UsartCallback callback);

extern volatile uint8_t rx_data;
void usart_send_byte(uint8_t data);
void USART1_IRQHandler(void);
void usart_pin_init(void);
void usart_lowlevel_init(void);
void usart_irq_init(void);
void usart_init(void);
void borad_init(void);
#endif
