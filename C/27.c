#include <stdio.h>  
  
// 函数声明：计算1+1/2+1/3+...+1/n的和  
double calculate_sum(int n);  
  
int main() {  
    int n = 100; // 序列中的项数  
    double sum = calculate_sum(n);  
    printf("The sum of 1+1/2+1/3+...+1/%d is: %f\n", n, sum);  
    return 0;  
}  
  
// 函数定义：计算1+1/2+1/3+...+1/n的和  
double calculate_sum(int n) {  
    double sum = 0.0;  
    for (int i = 1; i <= n; ++i) {  
        sum += 1.0 / i; // 注意：将1强制转换为double以避免整数除法  
    }  
    return sum;  
}