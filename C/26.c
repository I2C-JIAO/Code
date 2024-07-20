#include <stdio.h>  
#include <string.h>  
/*int main(int argc, char  *argv[])
{
    int a = 0;
    for(int i = 1; i <= 1000; i++)
    {
        a += i;
        printf("%d\n", a);
    }
    return 0;
}*/
/*int main() {  
    int sum = 0;  // 初始化求和变量为0  
    int i;  
  
    // 使用for循环从1加到1000，并输出累加过程和每个阶段的累加值  
    for (i = 1; i <= 1000; i++) {  
        printf("Adding %d to the sum...\n", i);  // 输出当前要加的数  
        sum += i;  // 累加每个整数到sum  
        printf("The current sum is: %d\n", sum);  // 输出当前累加值  
    }  
  
    // 输出最终的总和（虽然循环结束时已经有了这个值，但这里为了完整性）  
    printf("The final sum of 1 to 1000 is: %d\n", sum);  
  
    return 0;  
}*/
int main() {  
    int sum = 0;  // 初始化求和变量为0  
    int i;  
    char expression[2048] = ""; // 假设表达式不会太长，分配足够的空间  
  
    // 使用for循环从1加到1000，并输出累加过程和每个阶段的累加值  
    for (i = 1; i <= 1000; i++) {  
        // 添加加号（除了第一次）  
        if (strlen(expression) > 0) {  
            strcat(expression, "+");  
        }  
        // 添加当前数字  
        char num_str[16]; // 假设数字转换为字符串后不会太长  
        sprintf(num_str, "%d", i);  
        strcat(expression, num_str);  
  
        // 计算当前和  
        sum += i;  
  
        // 输出当前表达式和累加值  
        printf("%s = %d\n", expression, sum);  
    }  
  
    // 输出最终的总和（虽然循环结束时已经有了这个值，但这里为了完整性）  
    printf("The final sum of 1 to 1000 is: %d\n", sum);  
  
    return 0;  
}