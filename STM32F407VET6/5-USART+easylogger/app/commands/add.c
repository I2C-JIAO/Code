#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "shell.h"


int cmd_add(int argc, char *argv[])
{
    if (argc!= 3)
    {
        shellPrint(shellGetCurrent(),"The pramaters are not correct\n");
        return -1;
    }
    
    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    int c = a + b;
    shellPrint(shellGetCurrent(), "The sum of %d and %d is %d\n", a, b, c);
    return 0;
}
