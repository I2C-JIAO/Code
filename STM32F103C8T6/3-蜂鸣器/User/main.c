#include "stm32f10x.h"     // Device header
#include "Delay.h"			//包含Delay.h头文件

//这是STM32的第三个程序：蜂鸣器，硬件准备：接入PB12口（其他口也可）不要接到PA15,PB3,PB4这三个端口，这三个为JTAG的调试湍口
int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能APB2总线的RCC时钟，PB12为GPIOB
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体
	//GPIO_Mode_Out_PP 推挽输出高低电平都有驱动能力
	//GPIO_Mode_Out_OD 开漏输出模式下高电平为高阻态没有驱动能力，只在低电平有效，灯泡需正极接正极，PA0输出低电平才可正常点灯(插反无效）
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  //蜂鸣器用推挽输出和开漏都可，这里使用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//设置频率为50MHz
	GPIO_Init(GPIOB,&GPIO_InitStructure);//GPIOA初始化
	while(1)
		{
			//蜂鸣器模块规定低电平有效，所以置低电平时报警
			/*GPIO_ResetBits(GPIOB , GPIO_Pin_12);
			Delay_ms(500);
			GPIO_SetBits(GPIOB , GPIO_Pin_12);
			Delay_ms(500);*/
			GPIO_WriteBit(GPIOB , GPIO_Pin_12 , Bit_RESET);//GPIO输出低电平报警
			Delay_ms(500);
			GPIO_WriteBit(GPIOB , GPIO_Pin_12 , Bit_SET);//GPIO输出高电平静音
			Delay_ms(500);
		}

}
