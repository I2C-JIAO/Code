#include "stm32f10x.h"                  // Device header
#include "Delay.h"
int main(void)
{
	int n;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	while(1)
	{
		for(int i = 0; i < 8; i ++)
		{
			n = ~(0x0001 << i);
			GPIO_Write(GPIOA, n);
			Delay_ms(500);
		}

	}
}
