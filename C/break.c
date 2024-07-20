#include<stdio.h>
int main(int argc,char* argv[])
{
    int x=1,y=0,a=0,b=0;
    switch(x)//第一个switch
    {
        case 1:
        switch(y)//第二个switch
        {
            case 0:
            a++;
            break;//跳出第二个switch
            case 1:
            b++;
            break;
        }
        b = 100;
        break;//跳出第一个switch
        case 2:
        a++;
        b++;
        break;
    }
    printf("%d %d\n",a,b);
}