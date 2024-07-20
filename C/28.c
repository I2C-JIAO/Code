#include <stdio.h>  
  
// 函数声明  
double calculate_average(int start, int end, int *count, int is_even);  
  
int main() {  
    int cnt1 = 0, cnt2 = 0; // 偶数和奇数的个数  
    double ave1, ave2;      // 偶数和奇数的平均值  
  
    // 计算偶数的平均值和个数  
    ave1 = calculate_average(1, 100, &cnt1, 1);  
    // 计算奇数的平均值和个数（注意，我们传递0作为is_even的参数来指示我们正在寻找奇数）  
    ave2 = calculate_average(1, 100, &cnt2, 0);  
  
    // 输出结果  
    printf("Even numbers count: %d\n", cnt1);  
    printf("Even numbers average: %.2f\n", ave1);  
    printf("Odd numbers count: %d\n", cnt2);  
    printf("Odd numbers average: %.2f\n", ave2);  
  
    return 0;  
}  
  
// 计算给定范围内（start到end）的数字的平均值和个数  
// is_even参数指示是否仅计算偶数（1表示是，0表示否）  
double calculate_average(int start, int end, int *count, int is_even) {  
    int sum = 0;  
    for (int i = start; i <= end; i++) {  
        if ((i % 2 == 0 && is_even) || (i % 2 != 0 && !is_even)) {  
            sum += i;  
            (*count)++;  
        }  
    }  
    return (double)sum / (*count);  
}