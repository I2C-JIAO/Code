#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
  
// 函数声明  
char* build_expression(int start, int end, int *sum);  
void print_sum_and_expression(const char *expression, int sum);  
void free_expression(char *expression);  
  
int main() {  
    int sum = 0;  
    char *expression = build_expression(1, 1000, &sum);  
  
    // 输出最终的累加表达式和总和  
    print_sum_and_expression(expression, sum);  
  
    // 输出最终的总和（为了完整性）  
    printf("The final sum of 1 to 1000 is: %d\n", sum);  
  
    // 释放动态分配的内存  
    free_expression(expression);  
  
    return 0;  
}  
  
// 函数定义：构建累加表达式并计算总和  
char* build_expression(int start, int end, int *sum) {  
    char *expression = malloc(20); // 初始分配一些内存，足够存放一个数字和等号  
    if (expression == NULL) {  
        perror("Memory allocation failed");  
        exit(1);  
    }  
    snprintf(expression, 20, "%d=", start);  
    *sum = start; // 初始化总和  
  
    for (int i = start + 1; i <= end; i++) {  
        size_t len = strlen(expression);  
        char *new_expression = realloc(expression, len + 3 + 10); // 假设数字不会超过9位  
        if (new_expression == NULL) {  
            free(expression);  
            perror("Memory reallocation failed");  
            exit(1);  
        }  
        expression = new_expression;  
  
        strcat(expression, "+");  
        snprintf(expression + len + 1, 10, "%d", i);  
        *sum += i;  
    }  
  
    // 确保表达式以等号结尾（如果之前添加时没有添加）  
    if (expression[strlen(expression) - 1] != '=') {  
        size_t len = strlen(expression);  
        char *new_expression = realloc(expression, len + 2); // 为额外的等号分配空间  
        if (new_expression == NULL) {  
            free(expression);  
            perror("Memory reallocation failed");  
            exit(1);  
        }  
        expression = new_expression;  
        expression[len] = '=';  
        expression[len + 1] = '\0'; // 添加字符串结束符  
    }  
  
    return expression;  
}  
  
// 函数定义：打印累加表达式和总和  
void print_sum_and_expression(const char *expression, int sum) {  
    printf("%s\n", expression); // 直接打印表达式，因为它已经以等号结尾  
}  
  
// 函数定义：释放动态分配的内存  
void free_expression(char *expression) {  
    free(expression);  
}