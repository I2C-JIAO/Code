#include "stm32f10x.h"                  // Device header
/*DMA（Direct Memory Access）直接存储器存取
DMA可以提供外设和存储器或者存储器和存储器之间的高速数据传输，无须CPU干预，节省了CPU的资源
12个独立可配置的通道： DMA1（7个通道）， DMA2（5个通道）
每个通道都支持软件触发和特定的硬件触发

STM32F103C8T6 DMA资源：DMA1（7个通道）
*/
uint16_t MyDMA_Size;//定义一个变量接收要传输的长度
//DMA初始化参数有：数组a的地址，数组b的地址，要传输的长度size
void MyDMA_Init(uint32_t a, uint32_t b, uint16_t size)
{
	//长度传给全局变量MyDMA_Size，方便后续函数使用
	MyDMA_Size = size;
	//RCC开启AHB外设的DMA1时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	//定义DMA初始化结构体类型变量名
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_BufferSize = size; //传输计数器值设为size变量方便函数调用
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//外设作为传输源
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;//触发源设置软件触发
	DMA_InitStructure.DMA_MemoryBaseAddr = b;//DMA存储器地址为变量b的地址，方便函数调用
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器接收大小为字节
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器地址自增：填充完一个数据后，自动跳到下一个地址
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//模式为单次传输
	DMA_InitStructure.DMA_PeripheralBaseAddr = a;//DMA外设地址为变量a的地址，方便函数调用
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//DMA外设传输数据大小为字节
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;//DMA外设地址自增：填充完一个数据后，自动跳到下一个地址
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//优先级为中等优先级
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);//DMA初始化
	//开启DMA
	DMA_Cmd(DMA1_Channel1, ENABLE);
	//DMA要正常转运数据需要满足三个条件：	1、传输计数器>0;
//									2、触发源有信号;
//									3、DMA使能;
}
//DMA传输函数
void DMA_Transfer(void)
{
	//DMA传输计数器赋值前必须在传输计数器清0时，也就是非工作状态时
	//为了避免处在工作状态时给计数器赋值出错，在赋值前需给DMA手动失能
	DMA_Cmd(DMA1_Channel1, DISABLE);
	//调用设置计数器函数，给传输计数器写入数据
	DMA_SetCurrDataCounter(DMA1_Channel1, MyDMA_Size); 
	//写入数据完成后使能DMA进入工作状态
	DMA_Cmd(DMA1_Channel1, ENABLE);
	//调用获取标志位函数，获取转运完成标志位，标志位为RESET，则转运未完成，保持循环，标志位为SET转运完成，跳出循环
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
	//标志位清除
	DMA_ClearFlag(DMA1_FLAG_TC1);
}
