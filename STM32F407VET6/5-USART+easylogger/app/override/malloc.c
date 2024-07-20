#include <stdint.h>
#include <stddef.h>
#include "main.h"

//重写malloc函数
void *malloc(size_t size)
{
    return mem_malloc(size);
}
//重写free函数
void free(void *ptr)
{
    mem_free(ptr);
} 
