#include "stm32f10x.h"  
#include "Delay.h"  
#include "OLED.h"  
#include "MPU6050.h"  
  
//这是STM32的第25个项目，软件模拟I2C通信，读取MPU6050姿态传感器的数据

uint8_t ID;  //定义变量ID存放接收的ID号
int16_t AX, AY, AZ, GX, GY, GZ; //定义6个变量分别存放接收到的加速度计和陀螺仪X、Y、Z轴的数据
  
int main(void)  
{  
	//初始化
    OLED_Init();  
    MPU6050_Init();  
    //OLED显示模板
    OLED_ShowString(1, 1, "ID:");  
	//获取MPU6050设备ID
    ID = MPU6050_GetID();  
    OLED_ShowHexNum(1, 4, ID, 2);  
      
    while (1)  
    {  
		//调用获取寄存器数据函数，该函数通过指针传递参数，这里输入为AX, AY, AZ, GX, GY, GZ的变量地址
        MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);  
		/*
		定义6个变量存放数据转换后的值，方便查看
		计算加速度值转换公式为：AX / 32768 = X / 16g
		*/
        //这里先乘1000，避免AX值太小程序出现和现实算术算法不同的现象
        int32_t scaled_AX = AX * 1000 / 32767 * 16; 
        int32_t scaled_AY = AY * 1000 / 32767 * 16;  
        int32_t scaled_AZ = AZ * 1000 / 32767 * 16;  
  
        int16_t scaled_GX = GX * 2000 / 32767 * 16;  
        int16_t scaled_GY = GY * 2000 / 32767 * 16;  
        int16_t scaled_GZ = GZ * 2000 / 32767 * 16;  
  
		//因值有正负，所以OLED显示带符号数据
        OLED_ShowSignedNum(2, 1, scaled_AX, 5); 
        OLED_ShowSignedNum(3, 1, scaled_AY, 5);  
        OLED_ShowSignedNum(4, 1, scaled_AZ, 5);  
        OLED_ShowSignedNum(2, 8, scaled_GX, 5); 
        OLED_ShowSignedNum(3, 8, scaled_GY, 5);
		OLED_ShowSignedNum(4, 8, scaled_GZ, 5);
	}
}
