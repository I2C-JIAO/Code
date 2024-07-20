/*
I2C简介：
I2C（Inter IC Bus）是由Philips公司开发的一种通用数据总线
两根通信线：SCL（Serial Clock）、SDA（Serial Data）
同步，半双工
带数据应答
支持总线挂载多设备（一主多从、多主多从）
*/
#include "stm32f10x.h"                  // Device header
#include "Delay.h"			//包含延时函数
//配置SCL时钟引脚函数
void MyI2C_W_SCL(uint8_t BitValue)
{
	//软件模拟可选择任意引脚，这里SCL选择为PB10和硬件电路I2C的SCL引脚一致
	//调用GPIO_WriteBit函数设置指定GPIO端口指定位状态
	//将BitValue强转为BitAction（枚举类型）类型值：Bit_RESET或Bit_SET
	GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)BitValue);
	// 延迟一段时间，确保SCL信号保持足够的时间
	Delay_us(10);
}
//配置SDA输出引脚函数
void MyI2C_W_SDA(uint8_t BitValue)
{
	//软件模拟可选择任意引脚，这里SDA选择为PB11和硬件电路I2C的SDA引脚一致
	GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)BitValue);
	Delay_us(10);
}
//配置读取SDA函数
uint8_t MyI2C_R_SDA(void)
{
	uint8_t BitValue;//定义变量接收返回值
	//调用读取输入数据库函数读取PB11的值并放入变量中
	BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
	Delay_us(10);
	//返回数据位值
	return BitValue;
}
//I2C初始化
void MyI2C_Init(void)
{
	//开启GPIOB的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//I2C模式规定所有设备SCL和SDA均为开漏输出
	//初始化PB10和PB11为开漏输出；
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//初始化为高电平空闲状态
	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
}
//I2C起始条件：SCL高电平期间，SDA从高电平切换到低电平
void MyI2C_Start(void)
{
	//SDA先由高变低，SCL再由高变低
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);
}
//I2C终止条件：SCL高电平期间，SDA从低电平切换到高电平
void MyI2C_Stop(void)
{
	//先置SDA为低电平，为SDA由低电平变高电平做铺垫
	MyI2C_W_SDA(0);
	//先置SCL为高电平，再置SDA为高电平
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
}
//发送一个字节：SCL低电平期间，主机将数据位依次放到SDA线上（高位先行），
//然后释放SCL，从机将在SCL高电平期间读取数据位，
//所以SCL高电平期间SDA不允许有数据变化，依次循环上述过程8次，即可发送一个字节
void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i ++)
	{
		/*0x80 是一个十六进制数，其二进制表示为 10000000
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
		实现发送字节01001010(0x4A)*/
		MyI2C_W_SDA(Byte & (0x80 >> i));
		//从机将在SCL高电平期间读取收到的数据
		//发送完一个字节后拉高SCL（从机接收数据）再拉低SCL（主机可以继续发送）
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);
	}
}
/*接收一个字节：SCL低电平期间，从机将数据位依次放到SDA线上（高位先行），
然后释放SCL，主机将在SCL高电平期间读取数据位，所以SCL高电平期间SDA不允许有数据变化，
依次循环上述过程8次，即可接收一个字节（主机在接收之前，需要释放SDA）*/
uint8_t MyI2C_ReceiveByte(void)
{
	//初始化一个8位无符号整数Byte为0，用于存储从I2C接收的数据。
	uint8_t i, Byte = 0x00;
	//主机读取数据之前，从机先释放SDA引脚（即置SDA为1）
	MyI2C_W_SDA(1);
	//循环8次（每字节数据有8位）
	for (i = 0; i < 8; i ++)
	{	
		//主机将在高电平期间读取从机发来的数据，读数据前，先置SCL为高电平
		MyI2C_W_SCL(1);
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
		第8位为0不满足条件，i为7，此时循环结束，Byte值为01001010，也就是0x4A
		*/
		if (MyI2C_R_SDA() == 1){Byte |= (0x80 >> i);}
		//设置I2C的时钟线SCL为低电平，为了准备读取下一位数据
		MyI2C_W_SCL(0);
	}
	//函数最后返回Byte，这个变量现在包含了从I2C接收到的完整字节数据。
	return Byte;
}
//发送应答：主机在接收完一个字节之后，在下一个时钟发送一位数据，数据0表示应答，数据1表示非应答
void MyI2C_SendAck(uint8_t AckBit)
{
	/*
	接受完字节后SCL保持为低电平，为了发送应答信号，
	主机需要在时钟信号（SCL）为低电平时将数据线（SDA）拉低（设置为0）
	当SCL变为高电平时，SDA线保持低电平，这表示一个有效的应答（ACK）信号。
	（非应答则置SDA为高电平）
	从设备会在这个时钟周期内检测SDA线的状态。
	*/
	MyI2C_W_SDA(AckBit);
	//发送完应答后，先置SCL为高电平再置为低电平，从机在高电平期间读取应答信号
	MyI2C_W_SCL(1);
	MyI2C_W_SCL(0);
}
//接收应答：主机在发送完一个字节之后，在下一个时钟接收一位数据，判断从机是否应答，
//数据0表示应答，数据1表示非应答（主机在接收之前，需要释放SDA）
uint8_t MyI2C_ReceiveAck(void)
{
	//定义变量AckBit存放应答数据
	uint8_t AckBit;
	//接收应答前先释放SDA
	MyI2C_W_SDA(1);
	//主机在SCL高电平期间读取应答信号
	MyI2C_W_SCL(1);
	//调用读取SDA函数读取
	AckBit = MyI2C_R_SDA();
	//读取完置SCL为低电平
	MyI2C_W_SCL(0);
	//返回接收到的应答数据
	return AckBit;
}
