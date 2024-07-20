#include "stm32f10x.h"                  // Device header
//初始化蜂鸣器
void Buzzer_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);//开启GPIOB的时钟
	//GPIO初始化
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//选择推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//选择12号口
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//设置GPIO_Pin_12为默认高电平
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
}
//蜂鸣器开启函数
void Buzzer_ON(void)
{
	GPIO_ResetBits(GPIOB , GPIO_Pin_12);//低电平开启
}
//蜂鸣器开启函数
void Buzzer_OFF(void)
{
	GPIO_SetBits(GPIOB , GPIO_Pin_12);
}
//蜂鸣器状态翻转函数
void Buzzer_Turn(void)
{
	if(GPIO_ReadOutputDataBit(GPIOB , GPIO_Pin_12)==0)//读取12号口的输出值，如果输出低电平则设置高电平
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_12);
	}
	else//如果不是低电平，则设置低电平
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	}
}
