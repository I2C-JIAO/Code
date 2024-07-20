#include <stdbool.h>
#include <stdint.h>
#include "tlsf.h"
#include "shell.h"

static tlsf_t tlsf;
static uint8_t dyl_mem_pool[64 * 1024];

void mem_init(void)
{
    tlsf = tlsf_create_with_pool(dyl_mem_pool, sizeof(dyl_mem_pool));
    if (tlsf == NULL)
    {
        shellPrint(shellGetCurrent(), "Memory allocation failed!\r\n");
    }
}

void *mem_malloc(size_t size)
{
    return tlsf_malloc(tlsf, size);
}

void mem_free(void *ptr)
{
    tlsf_free(tlsf, ptr);
}
