#include "stm32f10x.h"     // Device header
#include "Delay.h"			//包含Delay.h头文件
#include "OLED.h"
#include "MyDMA.h"
//这是STM32的第19个程序：DMA（Direct Memory Access）直接存储器存取，使用DMA来转运数据
uint8_t A[] ={01 ,02, 03, 04};//定义数组A并写入数据
uint8_t B[] ={};//定义空数组B
int main(void)
{
	OLED_Init();
	//DMA初始化将数组A的地址作为传输源，传输到数组B的地址，地址变量前加(uint32_t)，传输长度为4个数据
	MyDMA_Init((uint32_t)A, (uint32_t)B, 4);
	//设置OLED显示屏模板
	OLED_ShowString(1, 1,"A:" );
	OLED_ShowString(3, 1,"B:" );
	OLED_ShowHexNum(1, 4, (uint32_t) A, 8);//显示A数组地址
	OLED_ShowHexNum(3, 4, (uint32_t) B, 8);//显示B数组地址
	while(1)
		{
			//A组数据各自加1然后显示出来
			A[0]++; 
			A[1]++; 
			A[2]++; 
			A[3]++;
			OLED_ShowNum(2, 1, A[0], 2);
			OLED_ShowNum(2, 4, A[1], 2);
			OLED_ShowNum(2, 7, A[2], 2);
			OLED_ShowNum(2, 10, A[3], 2);
			Delay_ms(1000);
			//延时1s后调用DMA转运函数，重新赋值计数器进行转运：在循环里执行效果为重复转运A组数据到B组
			DMA_Transfer();
			OLED_ShowNum(4, 1, B[0], 2);
			OLED_ShowNum(4, 4, B[1], 2);
			OLED_ShowNum(4, 7, B[2], 2);
			OLED_ShowNum(4, 10, B[3], 2);
			Delay_ms(1000);
			//最后程序效果为B组数据和A组保持延时后同步
		}
}
