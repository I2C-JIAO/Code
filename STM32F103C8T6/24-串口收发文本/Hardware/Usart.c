/*
串口简介：
USART（Universal Synchronous/Asynchronous Receiver/Transmitter）通用同步/异步收发器
USART是STM32内部集成的硬件外设，可根据数据寄存器的一个字节数据自动生成数据帧时序，
从TX引脚发送出去，也可自动接收RX引脚的数据帧时序，拼接为一个字节数据，存放在数据寄存器里
自带波特率发生器，最高达4.5Mbits/s
可配置数据位长度（8/9）、停止位长度（0.5/1/1.5/2）
可选校验位（无校验/奇校验/偶校验）
支持同步模式、硬件流控制、DMA、智能卡、IrDA、LIN

STM32F103C8T6 USART资源： USART1、 USART2、 USART3
*/
#include "stm32f10x.h"                  // Device header
#include "stdio.h"
#include "stdarg.h"
//定义一个char类型的足够大的数组Serial_RxPacket用来存放接收到的文本数据包
char Serial_RxPacket[100];
//定义一个变量Serial_RxFlag用来存放串口接收的标志位，本程序在main.c里直接调用，不再封装
uint8_t Serial_RxFlag;

void Usart_Init(void)
{
	//STM32F103C8T6中，USART_Tx复用在PA9引脚上，USART_Rx复用在PA10引脚上
	//RCC开启USART1和GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//初始Pin9为复用推挽输出
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//初始Pin10为上拉输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//串口初始化
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx |USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	//串口中断配置，串口1的接收标志位中断开启
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//配置NVIC嵌套向量中断控制器的优先级分组为2
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//NVIC初始化
	NVIC_InitTypeDef NVIC_InitStructure;
	//中断通道为USART1的中断
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	//使能USART1
	USART_Cmd(USART1, ENABLE);
}
//串口发送一个字节函数
void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}
//串口发送一个字节数组函数
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for(i = 0; i < Length; i++)
	{
		Serial_SendByte(Array[i]);
	}
}
//串口发送一个字符串函数
void Serial_SendString(char *String)
{
	uint8_t i;
	for(i = 0; String[i] != '\0'; i++)
	{
		Serial_SendByte(String[i]);
	}
}
//定义一个Pow函数计算X的Y次方
uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result =1;
	while(Y --)
	{
		Result *=X;
	}
	return Result;
}
//定义一个函数SendNumber将数字以指定长度的字符形式通过串口发送字节发送出去
void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for(i = 0; i < Length; i++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1)%10 + '0');
	}
}

int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}
//串口中断函数，利用中断配合接收
/*函数要实现的功能大概为：S = 0时等待包头，收到包头，置S = 1 ；
							S = 1时，循环接收4个数据，收够四个数据，置	S = 2
							S = 2时，等待包尾，收到包尾，置S = 0*/
void USART1_IRQHandler(void)
{
	//定义一个静态局部变量RxState存放接收状态：用于表示第几步，初始化值为0
	static uint8_t RxState = 0;
	//定义一个静态局部变量指针P，用来指向数据包中的第几个字节数据，初始化值为0，指向第一个字节数据
	static uint8_t P =0;
	//进入if条件判断，如果串口1的接收中断位置1，则执行下列语句
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		//定义一个8位的RxData变量用来接收一个字节数据，调用串口接收库函数，赋值给RxData
		uint8_t RxData = USART_ReceiveData(USART1);
		//switch判断RxState变量的值
		switch(RxState)
		{
			//Switch初始值为0，进入case 0
			//双重判断：
			//1：数据是否为包头；
			//2：串口接收标志位为0；（在本程序中接受完数据手动置1，在主函数中判断完接收到的数据执行完功能手动置0）
			//确保上一个数据已经接收完，且数据包的第一个数据为包头
			case 0:if((RxData == '@') && (Serial_RxFlag == 0))
			{
				RxState = 1;//状态置1，进入下一步
				P = 0;//指针为0，指向数据包中第一个字节
				break ;//跳出当前case 0 ，重新Switch（RxState）
			}
			//在包头正确的情况下，顺利进入case 1
			case 1:
			{
				//这里定义的包尾为\r\n（回车换行）
				//判断接收到的数据是否为\r
				//是则置状态为2，否则继续循环获取数据
				if(RxData != '\r')
				{
					//接收到的一个字节数据，循环放入数据包数组中，P表示第几个数据
					Serial_RxPacket[P] = RxData;
					P++;
				}
				else RxState = 2;
				break;//跳出当前case 1 ，重新Switch（RxState）
				//当未遇到包尾/r字符时，状态仍为1，仍执行case 1（话没说完继续说）
			}
			//状态被置为2时，进入case 2执行
			case 2:if(RxData == '\n')//判断是否为包尾\n
			{
				RxState = 0;//是的话状态置0（为下次判断包头做准备）
				//※：字符串通常是以空字符（null character）\0结尾的。
				//这个空字符是一个特殊的字符，其ASCII值为0，用于标记字符串的结束。
				//C语言中的字符串实际上是以字符数组的形式存储的
				//而\0字符就是用来告诉程序这个字符数组中的哪一部分是一个完整的字符串。
				Serial_RxPacket[P] = '\0';//手动赋字符'\0'给字符串结尾
				Serial_RxFlag = 1;//接受完数据接收标志位手动置1
				break;//跳出当前case 2 ，重新Switch（RxState）
			}
		}
		//清除中断标志位
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}
