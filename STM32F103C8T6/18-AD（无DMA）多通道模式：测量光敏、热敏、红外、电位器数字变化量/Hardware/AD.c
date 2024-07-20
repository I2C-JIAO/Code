#include "stm32f10x.h"                  // Device header
/*ADC（Analog-Digital Converter）模拟-数字转换器
ADC可以将引脚上连续变化的模拟电压转换为内存中存储的数字变量，建立模拟电路到数字电路的桥梁
12位逐次逼近型ADC，1us转换时间
输入电压范围：0~3.3V，转换结果范围：0~4095
18个输入通道，可测量16个外部和2个内部信号源
规则组和注入组两个转换单元
模拟看门狗自动监测输入电压范围
STM32F103C8T6 ADC资源：ADC1、ADC2，10个外部输入通道*/
//模数转换初始化
void AD_Init(void)
{
	//开启APB2片上外设的ADC1和GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//RCC时钟分频ADC,因ADC频率最大为14MHz，所以选择6分频，避免出现数据不精确的情况72MHz/6=12MHz
	//目前ADC时钟频率为12MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	//GPIO初始化，GPIO模式设置为模拟输入模式：GPIO_Mode_AIN
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//初始化ADC
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//连续模式设置为：单次转换不连续
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数据对齐设置右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//外部触发设置为空，这里我们用软件触发
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//ADC模式为独立模式，如果使用多个ADC同时工作则选择其他模式
	ADC_InitStructure.ADC_NbrOfChannel = 1;//指定要转换的ADC通道的数量为1
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//扫描模式设置为：非扫描模式
	ADC_Init(ADC1, &ADC_InitStructure);
	//ADC运行控制：使能ADC1
	ADC_Cmd(ADC1, ENABLE);
	/*ADC有一个内置自校准模式。校准可大幅减小因内部电容器组的变化而造成的准精度误差。
	校准期间，在每个电容器上都会计算出一个误差修正码(数字值)，这个码用于消除在随后的转换中每个电容器上产生的误差
	
	建议在每次上电后执行一次校准
	
	启动校准前，ADC必须处于关电状态超过至少两个ADC时钟周期*/
	//复位校准
	ADC_ResetCalibration(ADC1);//开始复位校准
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);//等待复位校准完成
	ADC_StartCalibration(ADC1);//开始校准
	while (ADC_GetCalibrationStatus(ADC1) == SET);//等待校准完成
}
//定义AD转换值获取函数
uint16_t AD_GetValue(uint8_t ADC_Channel)
{
	//本程序为无DMA的多通道模式，这里ADC规则组通道放在调用函数里，每次调用都会使用ADC_Channel变量传参的方法实现多通道
	//ADC规则组通道配置：选用ADC1,选用通道0，设置采样时间值为555
	ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_55Cycles5);
	//软件触发控制开启ADC1
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	//转换时间：AD转换的步骤：采样->保持->量化->编码。量化->编码过程为ADC逐次比较的过程
	//STM32ADC的总转换时间为：Tccnv = 采样时间 + 12.5个ADC周期
	//例：这里采样周期为55.5，则总采样时间为12.5+55.5=68 68/12MHz = 5.6us
	//while循环会保持5.6us的时间，标准位为1（SET）时，跳出循环
	//检测ADC规则组是否转换完成:转换标志位是否置1（SET）
	while (ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);
	//调用ADC获取转换值函数获取转换值
	return ADC_GetConversionValue(ADC1);
}
