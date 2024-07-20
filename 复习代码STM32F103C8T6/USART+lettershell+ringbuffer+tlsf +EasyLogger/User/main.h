#ifndef __MAIN_H__
#define __MAIN_H__


#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Console_Usart.h"
#include "shell.h"
#include "ringbuffer8.h"
#include "stddef.h"
#include "elog.h"
#include "elog_cfg.h"
#include "Led.h"
#include "Delay.h"

void mem_init(void);
void *mem_malloc(size_t size);
void mem_free(void *ptr);

void my_elog_init(void);
ElogErrCode elog_port_init(void);
void test_elog(void);
#endif   //__MAIN_H__
