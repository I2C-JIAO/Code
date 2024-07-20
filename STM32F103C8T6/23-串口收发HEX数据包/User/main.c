#include "stm32f10x.h"     // Device header
#include "Delay.h"			//包含Delay.h头文件
#include "OLED.h"			//包含OLED头文件
#include "Usart.h"			//包含串口头文件
#include "Key.h"			//包含按键头文件 

//这是第23个程序，串口收发HEX数据包

uint8_t KeyNum;  //定义一个KeyNum变量接收按键值

int main(void)
{
	KEY_Init();
	OLED_Init();
	Usart_Init();
	OLED_ShowString(1,1,"RxPacket:");
	OLED_ShowString(3,1,"TxPacket:");
	//定义数据包中四个数据的值
	Serial_TxPacket[0] = 0x01;
	Serial_TxPacket[1] = 0x02;
	Serial_TxPacket[2] = 0x03;
	Serial_TxPacket[3] = 0x04;
	
	while(1)
		{
			//调用键值获取函数获取键值赋给变量KeyNum
			KeyNum = Key_GetNum();
			if(KeyNum == 1)
			{
				//按键按下数据包中每个数据＋1
				Serial_TxPacket[0]++;
				Serial_TxPacket[1]++;
				Serial_TxPacket[2]++;
				Serial_TxPacket[3]++;
				//发送数据包并显示在OLED上
				Serial_SendPacket();
				OLED_ShowHexNum(2,1,Serial_TxPacket[0],2);
				OLED_ShowHexNum(2,4,Serial_TxPacket[1],2);
				OLED_ShowHexNum(2,7,Serial_TxPacket[2],2);
				OLED_ShowHexNum(2,10,Serial_TxPacket[3],2);
			}
			//调用获取串口接收标志位函数，如果标志位为1，中断产生
			if(Serial_GetRxFlag() == 1)
			{
				//OLED显示接收到数据
				OLED_ShowHexNum(4,1,Serial_RxPacket[0],2);
				OLED_ShowHexNum(4,4,Serial_RxPacket[1],2);
				OLED_ShowHexNum(4,7,Serial_RxPacket[2],2);
				OLED_ShowHexNum(4,10,Serial_RxPacket[3],2);
			}
		}
}
