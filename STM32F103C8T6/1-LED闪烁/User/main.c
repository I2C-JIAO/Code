#include "stm32f10x.h"     // Device header
#include "Delay.h"			//包含Delay.h头文件

//这是STM32的第一个程序：点灯，单片机需要保持工作状态，所以功能都在循环里进行，程序总是以循环结尾
int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能APB2总线的RCC时钟
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体
	//GPIO_Mode_Out_PP 推挽输出高低电平都有驱动能力
	//GPIO_Mode_Out_OD 开漏输出模式下高电平为高阻态没有驱动能力，只在低电平有效，灯泡需正极接正极，PA0输出低电平才可正常点灯(插反无效）
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //点灯用推挽输出即可
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//选用PA0口点灯
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//设置频率为50MHz
	GPIO_Init(GPIOA,&GPIO_InitStructure);//GPIOA初始化
	while(1)
	{	//因灯泡正极接电源正极，负极接PA0口，所以PA0口输出低电平，连通电路，点灯成功
		//当灯泡负极接电源负极，正极接PA0口时，PA0口输出高电平才可以连通电路，灯泡才会亮
		//原理是：必须一个高电平一个低电平才有电压差，才会有电流通过
		//也可以把GPIO输出低电平理解为接负极，输出高电平理解为接正极，所以正负极都有电路才会正常连通
		GPIO_WriteBit(GPIOA , GPIO_Pin_0 , Bit_RESET);//低电平点灯
		Delay_ms(500);//利用延时函数模拟灯泡闪烁
		GPIO_WriteBit(GPIOA , GPIO_Pin_0 , Bit_SET);//高电平灭灯
		Delay_ms(500);
		//如果想用0或1直接表示高低电平，直接把数据写成0或1的话编译器会有警告：枚举类型中混入了其他类型的变量！
		//因为Bit_SET类型为枚举类型，需加入强制类型转换为枚举类型才能使用如：(BitAction)0
		GPIO_WriteBit(GPIOA , GPIO_Pin_0 , (BitAction)0);//低电平点灯
		Delay_ms(500);//利用延时函数模拟灯泡闪烁
		GPIO_WriteBit(GPIOA , GPIO_Pin_0 , (BitAction)1);//高电平灭灯
		Delay_ms(500);
		//GPIO_WriteBit和GPIO_SetBits以及GPIO_ResetBits的区别主要是：
		//GPIO_WriteBit：提供了最大的灵活性，因为它可以单独设置每个引脚为高电平或低电平。适用于需要精确控制每个引脚状态的场景。
		//GPIO_SetBits以及GPIO_ResetBits：适用于需要同时将多个引脚设置为高电平或低电平的场景。在需要批量设置引脚状态的场合，它可以提高编程效率。
		//下节流水灯会适合GPIO_SetBits以及GPIO_ResetBits （因为有多个GPIO口需要同时设置）
		GPIO_ResetBits(GPIOA , GPIO_Pin_0);
		Delay_ms(500);
		GPIO_SetBits(GPIOA , GPIO_Pin_0);
		Delay_ms(500);
	}

}
