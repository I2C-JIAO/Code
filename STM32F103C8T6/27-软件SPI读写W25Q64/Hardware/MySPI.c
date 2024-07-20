/*

SPI通信简介：
SPI（Serial Peripheral Interface）是由Motorola公司开发的一种通用数据总线
四根通信线：SCK（Serial Clock）、MOSI（Master Output Slave Input）、MISO（Master Input Slave Output）、SS（Slave Select）
同步，全双工
支持总线挂载多设备（一主多从）

*/
#include "stm32f10x.h"                  // Device header
/*
SPI四个引脚函数定义，调用库函数进行软件模拟SPI输入输出
本程序PA4模拟SS(从机选择)，PA5模拟SCK(时钟线)，
PA7模拟MOSI(主机输出从机输入)，PA6模拟MISO(主机输入从机输出)，
本程序四个模拟引脚也与引脚定义中SPI外设（硬件电路）的复用引脚一致
方便后续改写为硬件SPI程序
*/
/*
因SS(从机选择)、SCK(时钟线)、MOSI(主机输出从机输入)均为输出，
故调用GPIO_WriteBit函数，函数名称带 _W_ 字样，并以(BitAction)类型强转输入参数BitValue
而MISO(主机输入从机输出)为输入，故定义返回类型为uint8_t的函数，
调用GPIO_ReadInputDataBit获取到PA6口的输入值并返回给函数
*/
void MySPI_W_SS(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)BitValue);
}

void MySPI_W_SCK(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction)BitValue);
}

void MySPI_W_MOSI(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_7, (BitAction)BitValue);
}

uint8_t MySPI_R_MISO(void)
{
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6);
}
/*
SPI初始化，开启APB2上的GPIOA外设时钟，分别设置PA4,PA5,PA7为推挽输出，PA6为上拉输入
SS引脚(从机选择)默认为高电平（置1），有从机进行通信时拉下来为低电平（置0）常用为模式1
*/
void MySPI_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_5 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	MySPI_W_SS(1);
	MySPI_W_SCK(0);
}
/*起始条件：SS从高电平切换到低电平*/
void MySPI_Start(void)
{
	MySPI_W_SS(0);
}
/*终止条件：SS从低电平切换到高电平*/
void MySPI_Stop(void)
{
	MySPI_W_SS(1);
}
/*
交换一个字节函数：SPI通信以交换字节的形式传输
对应四种模式：
交换一个字节（模式0）
CPOL=0：空闲状态时，SCK为低电平
CPHA=0：SCK第一个边沿移入数据，第二个边沿移出数据

交换一个字节（模式1）
CPOL=0：空闲状态时，SCK为低电平
CPHA=1：SCK第一个边沿移出数据，第二个边沿移入数据

交换一个字节（模式2）
CPOL=1：空闲状态时，SCK为高电平
CPHA=0：SCK第一个边沿移入数据，第二个边沿移出数据

交换一个字节（模式3）
CPOL=1：空闲状态时，SCK为高电平
CPHA=1：SCK第一个边沿移出数据，第二个边沿移入数据
本程序演示使用模式1
*/
/*
因软件模拟SPI时序，代码需要一条一条执行，无法在上升沿或下降沿的同时触发接收或发送
所以我们把发送和接受放在时钟信号变换的0和1中间来模拟时序变化
例：MySPI_W_SCK(0);
	MySPI_W_MOSI(ByteSend & 0x80);
	MySPI_W_SCK(1);
	上面代码表示模拟在时钟上升沿发送一位数据（0x80表示一个字节的最高位数据）
例：MySPI_W_SCK(1);
	if (MySPI_R_MISO() == 1){ByteReceive |= 0x80;}
	MySPI_W_SCK(0);
	上面代码表示模拟在时钟下降沿如果输入高电平则接收一位数据（0x80表示一个字节的最高位数据）
*/
uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	uint8_t i, ByteReceive = 0x00;
	
	for(i = 0; i < 8; i++)
	{
		/*
		0x80 是一个十六进制数，其二进制表示为 10000000
		右移操作符 >>:将0x80（128）右移1次为01000000（0x40 = 64），2次为00100000（0x20 = 32）……7次为00000001（0x01）
		位与操作符 &：同1为1，有0则0；已知0x80循环右移7次可以得到8位每一位都为1
		循环将输入的字节和0x80循环右移的结果（每一位为1）按位与得到最高位数据
		例：输入0x4A，0x4A二进制为01001010，
		0x80右移0次为10000000（0x80），0x4A与0x80得：01001010 & 10000000 = 0；
		0x80右移1次为01000000（0x40），0x4A与0x40得：01001010 & 01000000 = 1；
		0x80右移2次为00100000（0x20），0x4A与0x20得：01001010 & 00100000 = 0；
		0x80右移3次为00010000（0x10），0x4A与0x10得：01001010 & 00010000 = 0；
		0x80右移4次为00001000（0x08），0x4A与0x08得：01001010 & 00001000 = 1；
		0x80右移5次为00000100（0x04），0x4A与0x04得：01001010 & 00000100 = 0；
		0x80右移6次为00000010（0x02），0x4A与0x02得：01001010 & 00000010 = 1；
		0x80右移7次为00000001（0x01），0x4A与0x01得：01001010 & 00000001 = 0；
		实现发送字节01001010(0x4A)
		*/
		MySPI_W_MOSI(ByteSend & (0x80 >> i));
		MySPI_W_SCK(1);
		/*
		循环依次从高位到低位用或 | 运算符判断每一位是否为1，有1则该位置1，无1则置0
		例：假如接收到的数据为0x4A，0x4A = 01001010
			因I2C发送和接收顺序为高位先行
		第1位为0不满足条件，i为0，
		第2位为1满足条件，	i为1，执行Byte |= (0x80 >> 1)，Byte = 00000000 | 01000000 （01000000）
		第3位为0不满足条件，i为2，
		第4位为0不满足条件，i为3，
		第5位为1满足条件，	i为4，执行Byte |= (0x80 >> 4)，Byte = 01000000 | 00001000 （01001000）
		第6位为0不满足条件，i为5，
		第7位为1满足条件，	i为6，执行Byte |= (0x80 >> 6)，Byte = 01001000 | 00000010 （01001010）
		第8位为0不满足条件，i为7，此时循环结束，ByteReceive值为01001010，也就是0x4A
		*/
		if (MySPI_R_MISO() == 1){ByteReceive |= (0x80 >> i);}
		MySPI_W_SCK(0);
	}
	
	return ByteReceive;
}
