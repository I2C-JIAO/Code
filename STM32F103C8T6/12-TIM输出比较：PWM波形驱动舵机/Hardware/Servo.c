#include "stm32f10x.h"     // Device header
#include "PWM.h"		//头文件包含PWM.h		
//初始化舵机
void Servo_Init(void)
{
	PWM_Init();//调用PWM初始化函数
}
//舵机角度设置函数
void Servo_SetAngle(float Angle)
{
	//调用PWM_SetCompare2函数设置舵机角度，由于0-180度对应CCR值为500-2500
	//故得出以下公式：Angle / 180 * 2000 +500，计算结果值返回给CCR设置函数
	PWM_SetCompare2(Angle / 180 * 2000 +500);
}
