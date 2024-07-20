#include "stm32f10x.h"     // Device header
#include "Delay.h"			//包含Delay.h头文件
#include "OLED.h"
#include "AD.h"
/*这是STM32的第18个程序：AD多通道模式（无DMA），ADC数模转换器+AD转换值获取函数
来同时测量多个模块的电压：光敏、热敏、红外、电位器的电压*/

uint8_t AD0, AD1, AD2, AD3;//变量用于接收AD转换返回值
float Vo0, Vo1, Vo2, Vo3;//变量用于接收根据返回值计算得来的电压值
int main(void)
{
	OLED_Init();
	AD_Init();
	//设计OLED显示模板，需要改变的值会覆盖显示
	OLED_ShowString(1, 1,"A0:");
	OLED_ShowString(2, 1,"A1:");
	OLED_ShowString(3, 1,"A2:");
	OLED_ShowString(4, 1,"A3:");
	OLED_ShowString(1, 9,"V0:0.00V");
	OLED_ShowString(2, 9,"V1:0.00V");
	OLED_ShowString(3, 9,"V2:0.00V");
	OLED_ShowString(4, 9,"V3:0.00V");
	while(1)
		{
			//调用AD转换值获取函数，依次给定参数ADC_Channel_0、1、2、3传给调用函数（分别代表电位器、光敏、热敏、红外），
			//调用执行后结果返回的转换值分别传给AD0、1、2、3变量
			AD0 = AD_GetValue(ADC_Channel_0);
			AD1 = AD_GetValue(ADC_Channel_1);
			AD2 = AD_GetValue(ADC_Channel_2);
			AD3 = AD_GetValue(ADC_Channel_3);
			//根据输入电压范围：0~3.3V，转换结果范围：0~4095，分别计算ADx转换值，得到对应的电压值并分别传给Vox变量
			Vo0 = (float)AD0 / 4095 *3.3;
			Vo1 = (float)AD1 / 4095 *3.3;
			Vo2 = (float)AD2 / 4095 *3.3;
			Vo3 = (float)AD3 / 4095 *3.3;
			//OLED分别显示ADx的转换值
			OLED_ShowNum(1, 4, AD0, 4);
			OLED_ShowNum(2, 4, AD1, 4);
			OLED_ShowNum(3, 4, AD2, 4);
			OLED_ShowNum(4, 4, AD3, 4);
			//小数部分使用一个整数公式得到：浮点数电压值先乘以100然后强制转换位无符号整型值，
			//再对100取余得到后两位小数值，OLED显示跳过.开始覆盖，长度为两位。这样就实现了显示小数的效果
			//OLED分别显示Vox的转换值
			OLED_ShowNum(1, 12, Vo0, 1);
			OLED_ShowNum(1, 14, (uint16_t)(Vo0 * 100)%100, 2);
			OLED_ShowNum(2, 12, Vo1, 1);
			OLED_ShowNum(2, 14, (uint16_t)(Vo1 * 100)%100, 2);
			OLED_ShowNum(3, 12, Vo2, 1);
			OLED_ShowNum(3, 14, (uint16_t)(Vo2 * 100)%100, 2);
			OLED_ShowNum(4, 12, Vo3, 1);
			OLED_ShowNum(4, 14, (uint16_t)(Vo3 * 100)%100, 2);
			//延时100ms再刷新
			Delay_ms(100);
		}
}
