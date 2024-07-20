#include "stm32f10x.h"         // Device header
#include "Delay.h"			//包含延时函数头文件
//按键初始化
void KEY_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);//RCC开启APB2片上外设的GPIOB的时钟
	//GPIO初始化
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//设置输入模式为上拉输入，默认高电平，当按键按下时为低电平
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 |GPIO_Pin_11;//选择1号和11号口
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}
//定义返回值为int型的键值获取函数
uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;//默认键值为0
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1) == 0)//读取1号口输入的电平信号，如果为低电平则代表按键按下
		{
			Delay_ms(20);//按键按下瞬间有20ms的抖动，需要延时函数消抖
			while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1) == 0);//用当循环判断按键是否被持续按下
			Delay_ms(20);//当按键被松开瞬间有20ms的抖动，需要延时函数消抖
			KeyNum = 1;//上述代码段模拟整个按键从按下到松开后的过程，如果P1口被按下，键值返回1
		}
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11) == 0)//11号口按键原理同上
		{
			Delay_ms(20);
			while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11) == 0);
			Delay_ms(20);
			KeyNum = 2;//如果P1口被按下，键值返回1
		}
	return KeyNum;//返回按键值给函数
}
