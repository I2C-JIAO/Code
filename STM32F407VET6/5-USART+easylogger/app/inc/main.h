#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx.h"
#include "board.h"
#include "delay.h"
#include "my_usart.h"
#include "shell.h"
#include "ringbuffer8.h"
#include "elog.h"


void mem_init(void);
void *mem_malloc(size_t size);
void mem_free(void *ptr);

void easylogger_init(void);
void easylogger_print(void);

#endif /* __MAIN_H */
