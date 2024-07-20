#include "stm32f10x.h"                  // Device header
//初始化光敏传感器
void LightSensor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//开启时钟
	//GPIO初始化
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//设置为上拉输入，默认高电平
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
//设置光敏传感器状态获取函数
uint8_t LightSensor_Get(void)
{
	return GPIO_ReadInputDataBit(GPIOB ,GPIO_Pin_13);//读取13口的输入值，正常为低电平输出0，有光线遮住为高电平输出1
}
