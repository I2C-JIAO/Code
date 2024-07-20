#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"


int cmd_log(int argc, char *argv[])
{
    shellPrint(shellGetCurrent(), "Printing log...\n");
    easylogger_init();
    easylogger_print();
    return 0;
}
