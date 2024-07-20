#include "stm32f10x.h"                  // Device header
//定义一个int型Count来计次
uint16_t Count;
//初始化旋转编码器
void Encoder_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//开启APB2片上外设GPIO的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//开启APB2片上外设AFIO的时钟
	//GPIO初始化
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;//选用0号和1号口
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//AFIO：中断引脚配置
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
	//EXTI（边沿检测及控制）外部中断初始化，设置外部中断为下降沿触发，当电平信号由高电平变为低电平时触发中断
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	//配置NVIC优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//NVIC嵌套中断向量控制器初始化
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
}
//设置键值返回函数。返回键值
uint16_t Encoder_Num(void)
{
	uint16_t Temp;
	Temp = Count;
	Count = 0;
	return Temp;
	
}
//配置中断函数
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) == SET)//判断是否可以进入中断，和下方标志位清除为固定格式
	{
		//由旋转计次器波形可知两个引脚都为低电平时才可准确判断为计次-1（反转，由引脚可知先P0为低电平，再P1为低电平）
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)==0)
		{
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)==0)
		{
			Count --;//计次-1
		}
		}
		
		EXTI_ClearITPendingBit(EXTI_Line0);//中断位清除
	}
}
void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1) == SET)//判断是否可以进入中断，和下方标志位清除为固定格式
	{
		//由旋转计次器波形可知两个引脚都为低电平时才可准确判断为计次+1（正传转，由引脚可知先P1为低电平，再P0为低电平）
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)==0)
		{
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)==0)
		{
			Count ++;//计次+1
		}
		}
		
		EXTI_ClearITPendingBit(EXTI_Line1);//中断位清除
	}
}
