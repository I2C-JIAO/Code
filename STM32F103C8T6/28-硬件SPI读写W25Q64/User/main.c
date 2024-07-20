#include "stm32f10x.h"     // Device header
#include "Delay.h"			//包含Delay.h头文件
#include "OLED.h"			//包含OLED头文件
#include "W25Q64.h"			//包含W25Q64头文件
//这是STM32的第27个程序：软件SPI读写W25Q64

uint8_t MID;//定义一个8位变量MID接收厂商ID
uint16_t DID;//定义一个8位变量DID接收设备ID

uint8_t ArrayWrite[] = {0xAA, 0xBB, 0xCC, 0xDD};//定义一个8位数组存放要写入的数据
uint8_t ArrayRead[4];//定义一个8位数组用来存放接收到的数据
int main(void)
{
	OLED_Init();
	W25Q64_Init();
	
	OLED_ShowString(1, 1, "MID:   DID:");
	OLED_ShowString(2, 1, "W:");
	OLED_ShowString(3, 1, "R:");
	//调用读取ID函数获取设备ID
	W25Q64_ReadID(&MID, &DID);
	OLED_ShowHexNum(1, 5, MID, 2);
	OLED_ShowHexNum(1, 12, DID, 4);
	//写入前需要先擦除
	W25Q64_SectorErase(0x000000);
	W25Q64_PageProgram(0x000000, ArrayWrite, 4);
	//读取数据
	W25Q64_ReadData(0x000000, ArrayRead, 4);
	
	OLED_ShowHexNum(2, 3, ArrayWrite[0], 2);
	OLED_ShowHexNum(2, 6, ArrayWrite[1], 2);
	OLED_ShowHexNum(2, 9, ArrayWrite[2], 2);
	OLED_ShowHexNum(2, 12, ArrayWrite[3], 2);
	
	OLED_ShowHexNum(3, 3, ArrayRead[0], 2);
	OLED_ShowHexNum(3, 6, ArrayRead[1], 2);
	OLED_ShowHexNum(3, 9, ArrayRead[2], 2);
	OLED_ShowHexNum(3, 12, ArrayRead[3], 2);
	
	while (1)
	{
		
	}
}
