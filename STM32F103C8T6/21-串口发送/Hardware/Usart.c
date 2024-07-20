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
#include <stdio.h>				//包含标准输入输出头文件（Standard Input/Output Header File）。
                                //这个头文件包含了用于输入和输出功能的函数、宏和类型定义，比如 printf()、scanf()、fopen()、fclose() 等。
#include <stdarg.h>				//包含可变参数列表的标准库头文件
								//这个头文件定义了一系列宏，允许你编写接受可变数量参数的函数。
//因串口的异步时序特性，软件实现代码量庞大，给CPU造成很大负担，效率和稳定性也较差
//所以一般使用硬件电路完成串口发送
//串口初始化
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
	//设置波特率为9600，这里直接写想要的波特率就行，库函数会自动计算配置
	USART_InitStructure.USART_BaudRate = 9600;
	//硬件控制流一般应用在需要高速数据传输以及实时性要求较高的场景，本程序不需要
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	//串口模式选择，如果只需要接收或发送，配置一种即可，为下个程序做铺垫我配置了两种
	USART_InitStructure.USART_Mode = USART_Mode_Rx |USART_Mode_Tx;
	/*奇偶校验配置：
	奇校验：所有1的个数（包括起始位和停止位）为计数，则校验位为0，为偶数则校验位为1，
	保证所有1的个数为计数，这样当发过去一段数据后首先判断校验位，如果校验位为奇校验且
	数据帧中所有的1的个数不为奇数，则可以判定为数据不正确，可以丢弃
	偶校验：和奇校验同理，只是偶校验判断所有1的个数为偶数个，不够就补1，保证1的个数为偶数个
	注：因奇偶校验的特性，可靠性不是很高，所以只用来一般的数据传输。
	如遇到同时损失两位数据时，奇偶校验便会失效，需要更高的可靠性还需要借助其他的校验方法如CRC冗余校验等*/
	//本程序位串口发送实验现象，电路和程序比较简单，故采用不需要奇偶校验USART_Parity_No
	USART_InitStructure.USART_Parity = USART_Parity_No;
	//停止位设置，停止位为数据中最后一位数据，用于确定数据帧发送完毕并为开始下一个数据帧提供时间间隔
	//停止位一般选择为1位，其他设置分别为0.5、1.5、2个停止位，用于其他特定情况
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	//数据帧的字长，即每个数据帧中数据位的数量设置
	//可选为8位（无校验位）或9位（有校验位），这里我们没有校验位选择为8位数据
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	//使能串口
	USART_Cmd(USART1, ENABLE);
}
//串口发送一个字节函数
void Serial_SendByte(uint8_t Byte)
{
	//调用库函数USART_SendData发送一个字节
	USART_SendData(USART1, Byte);
	//调用库函数USART_GetFlagStatus获取发送寄存器标志位，进入延时等待，标志位为0则正在发送，
	//标志位为1则发送完毕跳出循环，发送标志位1在下一次发送时自动清除为0，无需手动清除
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}
//串口发送一个字节数组函数
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	//定义变量i用来表示循环次数，Length为数组长度
	uint16_t i;
	for(i = 0; i < Length; i++)
	{
		//循环发送字节数组中的字节
		Serial_SendByte(Array[i]);
	}
}
//串口发送一个字符串函数
void Serial_SendString(char *String)
{
	 //定义一个变量i用来表示循环次数
	uint8_t i;
	//持续循环到字符 = 字符‘\0’（‘\0’一般用作字符串结尾字符）
	for(i = 0; String[i] != '\0'; i++)
	{
		//循环输出字符串中的字符
		Serial_SendByte(String[i]);
	}
}
//定义一个计算X的Y次幂的函数用来配合串口发送数字的调用
uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	//定义一个初始化结果变量，任何无符号数的最小次方为0次方，初始化result = 1
	uint32_t Result =1;
	//while循环非0即真即循环，循环Y -- 旨在进入重复循环直到Y = 0跳出循环
	while(Y --)
	{
		/*循环一次X×自身一次，将每次乘自身的结果放入result变量中即可完成X^Y次方运算
			例：2^3,(X=2,Y=3).执行第1次循环为Y = 3，result（1） = 1*2 = 2；result = 2
								执行第2次循环为Y = 2，result（2） = 2*2 = 4；result = 4
								执行第3次循环为Y = 1，result（4） = 4*2 = 8；result = 8
		执行完第三次循环后Y值减为0，跳出循环*/
		Result *=X;
	}
	//返回result的值
	return Result;
}
//定义一个函数将数字以指定长度的字符形式通过串口发送字节发送出去
//函数输入两个变量一个是数值Number，一个是Length表示数值长度
void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	//定义一个变量i用来表示循环次数，持续循环length次 
	uint8_t i;
	for(i = 0; i < Length; i++)
	{
		/*循环调用串口发送一个字节函数输出对应的字符数字
		串口发送一个字节函数的参数为（Number / Serial_Pow(10, Length - i - 1)%10 + '0'）
		原理为：数值本身/10的（数值长度-i-1）次方再对10取余，得到数值从高位到低位每一位的数值
				每一位的数值 + 字符‘\0’ = 每一位数值对应的字符数字
		例如：输入数值9527，该数值长度为4，则循环四次输出；
		第1次：i = 0 ，4 - i - 1 = 3，则10 ^ 3 = 1000, 9527 /1000 = 9, 9 % 10 = 9, 9 + '0' = '9'
		第2次：i = 1 ，4 - i - 1 = 2，则10 ^ 2 = 100, 9527 /100 = 95, 95 % 10 = 5, 5 + '0' = '5'
		第3次：i = 2 ，4 - i - 1 = 1，则10 ^ 1 = 10, 9527 /10 = 952, 952 % 10 = 2, 2 + '0' = '2'
		第4次：i = 3 ，4 - i - 1 = 0，则10 ^ 0 = 1, 9527 /1 = 9527, 9527 % 10 = 7, 7 + '0' = '7'
		则对应输出结果为：‘9’‘5’‘2’‘7’，对应文本显示为：9527*/
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
