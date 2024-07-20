#include "stm32f10x.h"                  // Device header
uint16_t Count_Num;
void CountSensor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//开启GPIOB的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//开启AFIO
	//GPIO初始化
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//设置为上拉输入模式，默认输入高电平，默认为1，检测到输入0时，变为低电平
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//AFIO中断引脚选择
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
	//EXIT（边沿检测及控制）外部中断初始化，设置外部中断为下降沿触发，当电平信号由高电平变为低电平时触发中断
	//对射式编码计次器有遮挡物时，电平信号输出0，产生下降沿触发中断
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;//设置中断行为14行
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断行状态使能
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断模式为EXTI_Mode_Interrupt中断，EXTI_Mode_Event为事件中断
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//设置中断触发器为下降沿触发
	EXTI_Init(&EXTI_InitStructure);
	//配置NVIC优先级分组及NVIC嵌套中断向量控制器初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//配置优先级分组为组2
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//因为是14号口，设置中断通道为EXTI15_10_IRQn
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//设置通道使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//设置中断抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//设置中断响应优先级
	NVIC_Init(&NVIC_InitStructure);
}
//计次值获取函数
uint16_t Count_Num_Get(void)
{
	return Count_Num;
}
//配置中断函数
void EXTI15_10_IRQHandler(void)//中断函数从md.s文件中中断向量表得到，名字不能出错
{
	if(EXTI_GetITStatus(EXTI_Line14)== SET)//检查指定的14行是否被中断，中断为SET
	{
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0)//读取GPIO输入值是否为0，成功输入为0则计次＋1
		{
			Count_Num ++;
		}
		EXTI_ClearITPendingBit(EXTI_Line14);//清除中断位
	}
}
