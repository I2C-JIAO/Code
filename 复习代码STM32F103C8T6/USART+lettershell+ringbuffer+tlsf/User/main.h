#ifndef __MAIN_H__
#define __MAIN_H__

#include "stddef.h"

void mem_init(void);
void *mem_malloc(size_t size);
void mem_free(void *ptr);

#endif   //__MAIN_H__
