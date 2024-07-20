#include "stm32f10x.h"     // Device header
#include "Delay.h"			//包含Delay.h头文件
#include "OLED.h"
#include "Usart.h"
//这是STM32的第22个程序：USART串口发送 + 接收

//定义变量RxData存放串口接收的数据
uint8_t RxData;

int main(void)
{
	OLED_Init();
	OLED_ShowString(1,1,"RxData:");
	
	Usart_Init();
	while(1)
		{
			//调用获取接收标志位函数，判断是否为1
			if(Serial_GetRxFlag() == 1)
			{
				//调用串口获取接收数据函数，把数据传给变量RxData
				RxData = Serial_GetRxData();
				//把数据发送给电脑端方便查看
				Serial_SendByte(RxData);
				//OLED显示串口接收到的数据
				OLED_ShowHexNum(1, 8, RxData, 2);
			}
		}
}
