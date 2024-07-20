#include<stdio.h>
int main(int argc, char* argv[])
{
    int a[3][4][5]= {1,2,3,4,5,6,7,8,9,10,11,12,1,2,3,4,5,6,7,8,9,10,11,12,2,4,6,8,1,3,5,7,9};
    for(int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for(int k = 0; k <5; k++)
            printf("%d",a[i][j]);
        }
        
        printf("焦云飞\n");
    }

}