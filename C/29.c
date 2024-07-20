#include <stdio.h>
int isPrime(int n);

int main() {
    for (int num = 1; num <= 50; num++) {
        if (isPrime(num)) {
            printf("%d\n", num);  // 打印素数
        }
    }
    return 0;
}
// 函数用于判断一个数是否为素数
int isPrime(int n) {
    if (n <= 1) {
        return 0;  // 0和1都不是素数
    }
    for (int i = 2;  i*i <= n; i++) {
        if (n % i == 0) {
            return 0;  // 能被整除，不是素数
        }
    }
    return 1;  // 其他情况都是素数
}