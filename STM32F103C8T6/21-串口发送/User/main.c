#include "stm32f10x.h"     // Device header
#include "Delay.h"			//包含Delay.h头文件
#include "OLED.h"
#include "Usart.h"
//这是STM32的第21个程序：USART串口发送

int main(void)
{
	OLED_Init();
	Usart_Init();
	Serial_SendByte(0x41);
	uint8_t Array[] = {0x42, 0x43, 0x44, 0x45};
	Serial_SendArray(Array, 4);
	Serial_Printf("\r\n");
	Serial_SendString("你好世界！\r\n");
	Serial_SendNumber(9527, 4);
	Serial_Printf("\r\n");
	printf("Hello World! \r\n number = %d\r\n", 1);
	char String[100];
	sprintf(String, "去他妈的世界！ \r\n number = %d\r\n", 2);
	Serial_SendString(String);
	
	Serial_Printf("\nO(∩_∩)O");
	Serial_Printf("\r\n");
	while(1)
		{
			
		}
}
