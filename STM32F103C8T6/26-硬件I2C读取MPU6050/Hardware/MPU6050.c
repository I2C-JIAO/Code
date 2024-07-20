/*
I2C外设简介：
STM32内部集成了硬件I2C收发电路，可以由硬件自动执行时钟生成、起始终止条件生成、应答位收发、数据收发等功能，减轻CPU的负担
支持多主机模型
支持7位/10位地址模式
支持不同的通讯速度，标准速度(高达100 kHz)，快速(高达400 kHz)
支持DMA
兼容SMBus协议

STM32F103C8T6 硬件I2C资源：I2C1、I2C2
*/
#include "stm32f10x.h"                  // Device header
#include "MPU6050_Reg.h"

//定义从机MPU6050地址为 0xD0，因最后一位为读写位，所以只用到前7位，实际为0x68和手册中一致
#define MPU6050_ADDRESS		0xD0
//等待EV事件函数，输入两个参数：I2Cx和I2C_EVENT的具体事件
void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	//定义超时等待为1s
	uint32_t Timeout;
	Timeout = 10000;
	//等待事件发生，超时不再等待
	while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
	{
		Timeout --;
		if (Timeout == 0)
		{
			break;
		}
	}
}
/*
I2C时序指定地址写：
对于指定设备（Slave Address），在指定地址（Reg Address）下，写入指定数据（Data）
*/
//STM32在MPU6050指定地址写入数据（函数输入为：寄存器地址，数据）
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	//调用I2C库函数产生起始条件
	I2C_GenerateSTART(I2C2, ENABLE);
	//等待EV5事件（主机模式已选择选择）确保起始信号正确发送，发送起始条件后将STM32选择为主机
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	//调用库函数向指定的从设备发送设备地址字
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);
	//等待EV6事件：传输模式确认
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	//调用I2C发送字节函数发送寄存器地址字节
	I2C_SendData(I2C2, RegAddress);
	//等待EV8事件：移位寄存器非空，数据寄存器空（表示可以接收下一个字节：将数据继续放入数据寄存器）
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
	
	//发送一个数据
	I2C_SendData(I2C2, Data);
	//等待EV8_2事件：移位寄存器空，数据寄存器空（移位完成后，没有新的数据可发，通常用在最后一个字节发送结束）
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	//调用I2C库函数产生传输结束条件
	I2C_GenerateSTOP(I2C2, ENABLE);
}
/*
I2C读取MPU6050
I2C时序当前地址读：
对于指定设备（Slave Address），在当前地址指针指示的地址下，读取从机数据（Data）
I2C时序指定地址读：实际为指定地址写前半段时序与当前地址读后半段时序拼接而成
需要指定地址写指定地址+当前地址读读取数据
对于指定设备（Slave Address），在指定地址（Reg Address）下，读取从机数据（Data）
*/
//STM32在MPU6050指定地址读取数据（函数输入为：寄存器地址）
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	//定义一个变量Data存放接收的数据
	uint8_t Data;
	
	/*时序前半段：指定地址*/
	//调用I2C库函数产生起始条件
	I2C_GenerateSTART(I2C2, ENABLE);
	//等待EV5事件（主机模式已选择选择）确保起始信号正确发送，发送起始条件后将STM32选择为主机
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	//调用库函数向指定的从设备发送设备地址和发送位
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);
	//等待EV6事件：传输发送模式确认
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	//调用I2C发送字节函数发送寄存器地址字节
	I2C_SendData(I2C2, RegAddress);
	//等待EV8_2事件：移位寄存器空，数据寄存器空（移位完成后，没有新的数据可发，通常用在最后一个字节发送结束）
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	/*时序后半段：读取数据*/
	//再给个I2C起始信号，为下一个字节发送读写为做铺垫
	I2C_GenerateSTART(I2C2, ENABLE);
	//等待EV5事件（主机模式已选择选择）确保起始信号正确发送，发送起始条件后将STM32选择为主机
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	//调用库函数向指定的从设备发送设备地址和接收位
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Receiver);
	//等待EV6事件：传输接收模式确认
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
	/*根据时序图，接受字节停止需要提前置ACK非应答，STOP置1（在EV6事件响应后提前告知接收完毕），
	如果只接受一个字节在EV6事件响应后需立即产生终止条件，这里并不会截断数据，而是等接受完这一个字节*/
	I2C_AcknowledgeConfig(I2C2, DISABLE);//ACK失能
	I2C_GenerateSTOP(I2C2, ENABLE);//STOP使能
	//等待EV7事件，字节接收完毕
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);
	//在终止条件产生前，EV7事件产生后已经有一个字节数据放在Dr寄存器中，现在只需要读取寄存器即可获取数据
	Data = I2C_ReceiveData(I2C2);
	//函数最后恢复ACK默认值为1，应答状态（方便接收多个字节）
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	//返回接收的数据给函数
	return Data;
}
//MPU6050及I2C硬件初始化
void MPU6050_Init(void)
{
	//RCC开启I2C2和GPIOB的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//根据引脚配置定义表，SCL接在PB10引脚，SDA接在PB11引脚，且统一配置为复用开漏输出
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//初始化I2C
	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;//模式设置为I2C模式
	//时钟频率最大不能超过400KHz，小于100KHz为低速模式，这里设置为50KHz
	I2C_InitStructure.I2C_ClockSpeed = 50000;
	//占空比设置：该参数在快速模式（时钟频率高于100KHz）下才有意义
	//有两个可选值：16：9和2，这里设置2
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	//默认使能ACK应答
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	//设置应答地址为应答7位地址
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	//设置第一个设备（主机）自身地址，可以为7位或12位，这里给0x00
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_Init(I2C2, &I2C_InitStructure);
	//使能I2C2
	I2C_Cmd(I2C2, ENABLE);
	
	//设置电源管理寄存器1，（解除睡眠，选择陀螺仪时钟）设置000为内部时钟，陀螺仪时钟为001，手册建议我们选择陀螺仪时钟
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);
	//设置电源管理寄存器2，（6个轴均不需要待机）全置0不需要待机
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
	//设置采样率分频器，值越小采样率越快，这里我们设置10分频（控制陀螺仪和加速度计的采样率）
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);
	//设置MPU6050的全局配置，外部同步置0，低通滤波（给最大）设置为110
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);
	//设置陀螺仪的配置，自测给0，配置为满量程11，高通滤波0
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);
	//设置加速度计的配置，自测给0，配置为满量程11，高通滤波0 00000110
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);
}
//获取设备ID号
uint8_t MPU6050_GetID(void)
{
	//读取MPU6050_WHO_AM_I寄存器返回读出值
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}
//获取寄存器数据函数
//函数输入6个指针传递接收到的数据
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	//定义两个变量分别接收高八位和低八位的数据
	uint8_t DataH, DataL;
	/*
	分别获取加速度计和陀螺仪的X、Y、Z轴高八位寄存器的值给变量DataH
	分别获取加速度计和陀螺仪的X、Y、Z轴低八位寄存器的值给变量DataL
	高八位数据左移8位 或 | 低八位数据得到完整数据传递给指针指向的地址
	*/
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
	*AccX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	*AccY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	*AccZ = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	*GyroX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	*GyroY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	*GyroZ = (DataH << 8) | DataL;
}
