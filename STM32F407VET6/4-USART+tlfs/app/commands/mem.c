#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "shell.h"


int cmd_mem(int argc, char *argv[])
{
    int size = atoi(argv[1]);
    uint8_t *buffer = malloc(size);

    shellPrint(shellGetCurrent(), "Memory allocated %d bytes at address: 0x%p\n", size, buffer);

    free(buffer);

    return 0;
}
