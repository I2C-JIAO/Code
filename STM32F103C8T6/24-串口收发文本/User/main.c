#include "stm32f10x.h"     // Device header
#include "Delay.h"			//包含Delay.h头文件
#include "OLED.h"			//包含OLED头文件
#include "LED.h"			//包含LED头文件
#include "Usart.h"			//包含串口头文件
#include "string.h"			//包含字符串头文件用于使用字符串比较函数

//这是第24个程序，串口收发文本数据，并控制LED灯点亮熄灭
int main(void)
{
	//LED OLED 串口初始化
	LED_Init();
	OLED_Init();
	Usart_Init();
	//OLED显示模板
	OLED_ShowString(1,1,"RxPacket:");
	OLED_ShowString(3,1,"TxPacket:");

	
	while(1)
		{
			//循环判断接收标志位，接收标志位为1代表串口接收完毕，执行下列功能语句
			if(Serial_RxFlag == 1)
			{
				//先显示一段空白再显示接收到的数据包为了避免数据覆盖的现象，优化视觉效果
				OLED_ShowString(2,1,"                ");
				OLED_ShowString(2,1,Serial_RxPacket);
				//使用字符串比较函数（相同为0，不同为1）比较接收到的数据包是否为LED_ON
				if(strcmp(Serial_RxPacket, "LED_ON") == 0)
				{
					LED1_ON();//打开LED灯
					//调用字符串发送函数发送LED_ON_OK!
					Serial_SendString("LED_ON_OK!");
					//显示发送的数据
					OLED_ShowString(4,1,"                ");
					OLED_ShowString(4,1,"LED_ON_OK!");
				}
				//使用字符串比较函数（相同为0，不同为1）比较接收到的数据包是否为LED_OFF
				else if(strcmp(Serial_RxPacket, "LED_OFF") == 0)
				{
					LED1_OFF();//关闭LED灯
					//调用字符串发送函数发送LED_OFF_OK!
					Serial_SendString("LED_OFF_OK!");
					//显示发送的数据
					OLED_ShowString(4,1,"                ");
					OLED_ShowString(4,1,"LED_OFF_OK!");
				}
				//如果既不是LED_ON也不是LED_OFF就输出指令错误
				else
				{
					Serial_SendString("Command Error!");
					OLED_ShowString(4,1,"                ");
					OLED_ShowString(4,1,"Command Error!");
				}
				//执行完功能置接收标志位为0，可继续接收下一个数据包指令
				Serial_RxFlag = 0;
			}

		}
}
