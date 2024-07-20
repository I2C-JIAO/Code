#include <shell.h>
int cmd_helloworld(int argc, char *argv[])
{
    shellPrint(shellGetCurrent(), "%s\n","Hello World!\n");

    shellWriteString(shellGetCurrent(), "Hello World!\n");

    shellPrint(shellGetCurrent(), "we have %d parameters:\n",argc);
    for (int i = 0; i < argc; i++)
    {
        shellPrint(shellGetCurrent(), "argv[%d] = %s\n", i, argv[i]);
    }

    return 0;
}   
//SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), helloworld, cmd_helloworld, this is a helloworld command);
