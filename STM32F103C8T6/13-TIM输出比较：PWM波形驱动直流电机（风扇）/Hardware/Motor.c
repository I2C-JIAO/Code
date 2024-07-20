#include "stm32f10x.h"    // Device header
#include "PWM.h"		//头文件包含PWM
//电机初始化
void Motor_Init(void)
{
	
	//开启GPIOA的时钟并使能Pin4和Pin5口为推挽输出
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//调用PWM初始化函数
	PWM_Init();
}
//设置电机速度函数：速度大于0时：PA4置高电平，PA5置低电平电流从4口流到5口
void Motor_Speed(int8_t Speed)
{
	if(Speed >= 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_4);
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);
		PWM_SetCompare3(Speed);//调用PWM_SetCompare3函数设置CCR值，传入Speed变量
	}
	//速度小于0时：PA5置高电平，PA4置低电平电流从5口流到4口
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_4);
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
		PWM_SetCompare3(-Speed);//设置转速，因Speed需要为无符号整型，需将负数变为正数，Speed前加负号-
	}
}
