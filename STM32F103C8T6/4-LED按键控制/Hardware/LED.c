#include "stm32f10x.h"                  // Device header
//LED初始化
void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);//开启GPIOA的时钟
	//GPIO初始化
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//设置推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;//选用1，2号口
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//设置默认为高电平(LED正极接VCC负极接GPIO，GPIO输出高电平无电位差，不产生电压没有电流)，所以LED默认不亮
	GPIO_SetBits(GPIOA,GPIO_Pin_1 |GPIO_Pin_2);
}
//LED1开启函数
void LED1_ON(void)
{
	GPIO_ResetBits(GPIOA , GPIO_Pin_1);
}
//LED2开启函数
void LED2_ON(void)
{
	GPIO_ResetBits(GPIOA , GPIO_Pin_2);
}
//LED1关闭函数
void LED1_OFF(void)
{
	GPIO_SetBits(GPIOA , GPIO_Pin_1);
}
//LED2关闭函数
void LED2_OFF(void)
{
	GPIO_SetBits(GPIOA , GPIO_Pin_2);
}
//LED1状态翻转函数
//读取GPIO输出值，如果为0（低电平状态开灯）则设置为1（高电平状态关灯），如果不是0（低电平状态开灯）则设置为0（低电平状态开灯）
void LED1_Turn(void)
{
	if(GPIO_ReadOutputDataBit(GPIOA , GPIO_Pin_1)==0)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_1);
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	}
}
//LED2状态翻转函数
//读取GPIO输出值，如果为0（低电平状态开灯）则设置为1（高电平状态关灯），如果不是0（低电平状态开灯）则设置为0（低电平状态开灯）
void LED2_Turn(void)
{
	if(GPIO_ReadOutputDataBit(GPIOA , GPIO_Pin_2)==0)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_2);
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	}
}
