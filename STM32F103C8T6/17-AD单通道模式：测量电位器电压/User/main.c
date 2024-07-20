#include "stm32f10x.h"     // Device header
#include "Delay.h"			//包含Delay.h头文件
#include "OLED.h"
#include "AD.h"
//这是STM32的第17个程序：AD单通道模式，ADC数模转换器用来测量电位器的电压
uint16_t ADValue;//变量用于接收AD转换返回值
float Voltage;//变量用于接收根据返回值计算得来的电压值
int main(void)
{
	OLED_Init();
	AD_Init();
	OLED_ShowString(1, 1,"ADValue:");//设计OLED显示模板，需要改变的值会覆盖显示
	OLED_ShowString(2, 1,"Voltage:0.00V");
	while(1)
		{
			ADValue = AD_GetValue();//调用AD转换值获取函数
			//根据输入电压范围：0~3.3V，转换结果范围：0~4095，计算得到对应的电压值并传给Voltage变量
			Voltage = (float)ADValue / 4095 *3.3;
			OLED_ShowNum(1, 9, ADValue, 4);
			//首先设置1位用来显示电压的整数部分
			OLED_ShowNum(2, 9, Voltage, 1);
			//小数部分使用一个整数公式得到：浮点数电压值先乘以100然后强制转换位无符号整型值，
			//再对100取余得到后两位小数值，OLED显示跳过.开始覆盖，长度为两位。这样就实现了显示小数的效果
			OLED_ShowNum(2, 11, (uint16_t)(Voltage * 100)%100, 2);
			//延时100ms再刷新
			Delay_ms(100);
		}
}
