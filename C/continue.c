#include<stdio.h>
int main(int argc, char* argv[])
{
    int a = 5;
    for(int i = 0; i <= 10; i++)
    {
        printf("12\n");
        printf("34\n");
        continue; //返回继续循环，后续语句不执行；
        a = 100;
        printf("%d\n",a);
    }
}