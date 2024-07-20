#include "stm32f10x.h"     // Device header
#include "Delay.h"			//包含Delay.h头文件

//这是STM32的第二个程序：流水灯
int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能APB2总线的RCC时钟
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体
	//GPIO_Mode_Out_PP 推挽输出高低电平都有驱动能力
	//GPIO_Mode_Out_OD 开漏输出模式下高电平为高阻态没有驱动能力，只在低电平有效，灯泡需正极接正极，PA0输出低电平才可正常点灯(插反无效）
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //点灯用推挽输出即可
	//批量选择GPIO口可以用按位或的方式实现如：GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2
	//原理：库函数定义中：GPIO_Pin_0对应的16进制为((uint16_t)0x0001)，转换为二进制为0000 0000 0000 0001
	//GPIO_Pin_1对应的16进制为((uint16_t)0x0002)，转换为二进制为0000 0000 0000 0010
	//按位或（有1为1，全0为0）后，GPIO_Pin_0 | GPIO_Pin_1 = 0000 0000 0000 0011 后两位都置1，都生效
	//在这里只用流水灯演示，可以直接用GPIO_Pin_ALL
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//设置频率为50MHz
	GPIO_Init(GPIOA,&GPIO_InitStructure);//GPIOA初始化
	while(1)
		{	
			//小夜灯GPIO_Write(GPIOA , 0xFF00);
			//因灯泡负极接入PA口，所以PA口输出低电平有效，1为高电平，0为低电平（输出0才能亮）
			//所以0x0001以及其他GPIO口需要按位取反（0变1，1变0）才有效
			/*GPIO_Write(GPIOA , ~0x0001);//对应：0000 0000 0000 0010 取反后为 1111 1111 1111 1110
			Delay_ms(500);
			GPIO_Write(GPIOA , ~0x0002);//对应：0000 0000 0000 0010 取反后为 1111 1111 1111 1101
			Delay_ms(500);
			GPIO_Write(GPIOA , ~0x0004);//对应：0000 0000 0000 0100 取反后为 1111 1111 1111 1011
			Delay_ms(500);
			GPIO_Write(GPIOA , ~0x0008);//对应：0000 0000 0000 1000 取反后为 1111 1111 1111 0111
			Delay_ms(500);
			GPIO_Write(GPIOA , ~0x0010);//对应：0000 0000 0001 0000 取反后为 1111 1111 1110 1111
			Delay_ms(500);
			GPIO_Write(GPIOA , ~0x0020);//对应：0000 0000 0010 0000 取反后为 1111 1111 1101 1111
			Delay_ms(500);
			GPIO_Write(GPIOA , ~0x0040);//对应：0000 0000 0100 0000 取反后为 1111 1111 1011 1111
			Delay_ms(500);
			GPIO_Write(GPIOA , ~0x0080);//对应：0000 0000 1000 0000 取反后为 1111 1111 0111 1111
			Delay_ms(500);*/
			//使用for循环优化了上述代码，提高了可读性，减少了代码量
			int n;
			for(int i = 0 ;i < 8 ;i++)
			{
				n = ~(0x0001 << i);
				GPIO_Write(GPIOA , n);
				Delay_ms(1000);
			}
			//下一节蜂鸣器：接线时不要接到PA15,PB3,PB4这三个端口，这三个为JTAG的调试湍口
		}

}
