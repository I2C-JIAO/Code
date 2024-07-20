/*
SPI通信简介：
SPI（Serial Peripheral Interface）是由Motorola公司开发的一种通用数据总线
四根通信线：SCK（Serial Clock）、MOSI（Master Output Slave Input）、MISO（Master Input Slave Output）、SS（Slave Select）
同步，全双工
支持总线挂载多设备（一主多从）
*/
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
#include "stm32f10x.h"                  // Device header

/*
因SS(从机选择)为软件模拟输出高低电平，使用PA4引脚进行模拟
故调用GPIO_WriteBit函数，函数名称带 _W_ 字样，并以(BitAction)类型强转输入参数BitValue
根据STM32F103C8T6引脚定义表:
SCK(时钟线)为PA5复用、MISO(主机输入从机输出)为PA6复用、MOSI(主机输出从机输入)为PA7复用
此三个引脚均由硬件配置
*/
void MySPI_W_SS(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)BitValue);
}
/*
SPI初始化，开启APB2上的GPIOA和SPI1外设时钟
分别设置PA4为推挽输出，PA5和PA7为复用推挽输出，PA6为上拉输入
SS引脚(从机选择)默认为高电平（置1），有从机进行通信时拉下来为低电平（置0）常用为模式1
*/
void MySPI_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI1, &SPI_InitStructure);
	
	SPI_Cmd(SPI1, ENABLE);
	
	MySPI_W_SS(1);
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
交换一个字节函数：
首先调用库函数获取标志位，判断发送标志位TXE是否置1（发送寄存器DR是否为空）
接着调用SPI外设库函数发送数据，因为交换字节是接收和发送同步进行，故接收完成也是发送完成
所以调用库函数获取标志位，判断接收标志位RXNE是否置1（RXNE置1时代表接收移位完成）
调用SPI库函数读取RDR寄存器，将获取到的数据返回给函数
*/
uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != SET);
	SPI_I2S_SendData(SPI1, ByteSend);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != SET);
	return SPI_I2S_ReceiveData(SPI1);
}
