#ifndef __MY_USART_H
#define __MY_USART_H

#include "stm32f4xx.h"
#include "ringbuffer8.h"
#include "shell.h"
#include "dht11.h"
#include <stdio.h>

typedef void (*usart_callback_t)(uint8_t data);
void usart_recv_callback_register(usart_callback_t callback);

void my_usart_init(void);
void my_usart_send(uint8_t data);
void usart_rx_flush(void);

#endif /* __MY_USART_H */
