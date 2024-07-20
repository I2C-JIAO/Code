#include<stdio.h>
int main(void)
{
int **dynamic2D;  
dynamic2D = (int **)malloc(3 * sizeof(int *)); // 分配指针数组  
for(int i = 0; i < 3; i++) {  
    dynamic2D[i] = (int *)malloc(4 * sizeof(int)); // 为每个一维数组分配内存  
}  
// ... 使用dynamic2D ...  
// 记得释放内存  
for(int i = 0; i < 3; i++) { 
    printf("%d\n",&dynamic2D); 
    free(dynamic2D[i]);  
}  
free(dynamic2D);
}