#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Encoder.h"

//int16_t Number;
int main(void)
{
	OLED_Init();
	Encoder_Init();
	OLED_ShowString(1, 1, "Number:");
	
	while(1)
	{	
//		Number += EncoderCount_Get();
		OLED_ShowSignedNum(2, 1, EncoderCount_Get(),5);
	}
}
