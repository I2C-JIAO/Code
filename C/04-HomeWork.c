/*
4. 例如:
    输入 5  8；
    计算 5 + 6 + 7 + 8；
    输出 26.

注意:
    需要考虑用户提供数据的顺序，存在用户提供数据第一个数值大于第二个.

    法无禁止，即自由
*/
#include <stdio.h>

int main(int argc, char const *argv[])
{
    // 用户输入的第一个数据
    int n1 = 0;
    // 用户提供的第二个数据
    int n2 = 0;
    int sum = 0;

    printf("请输入两个整数:");
    scanf("%d %d", &n1, &n2);

    /*
    方案一: 保证 n1 的数据小于 n2
    */
    if (n1 > n2)
    {
        n1 += n2;
        n2 = n1 - n2;
        n1 = n1 - n2;
    }

    for (int i = n1; i <= n2; i++)
    {
        sum += i;
    }

    printf("sum = %d\n", sum);

    printf("请输入两个整数:");
    scanf("%d %d", &n1, &n2);

    /*
    方案二: 循环保证从较小值到较大值过程
    */
    int min = n1 > n2 ? n2 : n1;
    int max = n1 > n2 ? n1 : n2;

    sum = 0;

    for (int i = min; i <= max; i++)
    {
        sum += i;
    }

    printf("sum = %d\n", sum);

    /*
    方案三: 高斯求和公式
        开发中任何手段都可以使用，包括数学！！！
    */
    printf("请输入两个整数:");
    scanf("%d %d", &n1, &n2);

    if (n1 > n2)
    {
        n1 += n2;
        n2 = n1 - n2;
        n1 = n1 - n2;
    }
    /*
    高斯求和公式/等差数列求和公式
        (首项 + 末项) * 项数 / 2
    */
    sum = (n1 + n2) * (n2 - n1 + 1) / 2;
    printf("sum = %d\n", sum);
    return 0;
}
