#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"						//包含I2C头文件
#include "MPU6050_Reg.h" //（该头文件需自己定义）包含与MPU-6050相关的寄存器定义、宏定义、类型定义、函数原型
/*这里为方便软件模拟，定义从机MPU6050地址为 0xD0，
因最后一位为读写位，所以只用到前7位，实际为0x68和手册中一致*/
#define MPU6050_ADDRESS		0xD0
/*
I2C时序指定地址写：
对于指定设备（Slave Address），在指定地址（Reg Address）下，写入指定数据（Data）
*/
//STM32在MPU6050指定地址写入数据（函数输入为：寄存器地址，数据）
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	//I2C起始
	MyI2C_Start();
	//发送MPU6050的地址
	MyI2C_SendByte(MPU6050_ADDRESS);
	//接收从机应答信号
	MyI2C_ReceiveAck();
	//发送寄存器地址
	MyI2C_SendByte(RegAddress);
	//接收从机应答信号
	MyI2C_ReceiveAck();
	//发送数据
	MyI2C_SendByte(Data);
	//接收从机应答信号
	MyI2C_ReceiveAck();
	//I2C终止
	MyI2C_Stop();
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
	//I2C起始
	MyI2C_Start();
	//发送MPU6050的地址，此时最后一位读写位为0，仍为写入数据
	MyI2C_SendByte(MPU6050_ADDRESS);
	///接收从机应答信号
	MyI2C_ReceiveAck();
	//发送寄存器地址
	MyI2C_SendByte(RegAddress);
	//接收从机应答信号
	MyI2C_ReceiveAck();
	//执行到此已经指定到了寄存器地址，还未进行读写数据
	/*时序后半段：读取数据*/
	//再给个I2C起始信号，为下一个字节发送读写为做铺垫
	MyI2C_Start();
	//发送从机地址+读写位，0xD0 | 0x01后变为0xD1，
	//也就是地址0x68 + 1位的读写位，此时最后一位读写位为1，I2C为读数据
	MyI2C_SendByte(MPU6050_ADDRESS | 0x01);
	//接收从机应答信号
	MyI2C_ReceiveAck();
	//调用接受字节函数接收字节数据并存放到变量Data中
	Data = MyI2C_ReceiveByte();
	//接收完给从机发送应答信号，1为不应答，不再接受数据，0为继续接收数据
	MyI2C_SendAck(1);
	//I2C终止
	MyI2C_Stop();
	//返回接受的数据给函数
	return Data;
}
//MPU6050初始化
void MPU6050_Init(void)
{
	//I2C初始化
	MyI2C_Init();
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
