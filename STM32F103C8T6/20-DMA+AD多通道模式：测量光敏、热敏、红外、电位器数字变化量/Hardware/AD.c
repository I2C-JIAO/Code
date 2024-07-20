#include "stm32f10x.h"                  // Device header
/*ADC（Analog-Digital Converter）模拟-数字转换器
ADC可以将引脚上连续变化的模拟电压转换为内存中存储的数字变量，建立模拟电路到数字电路的桥梁
12位逐次逼近型ADC，1us转换时间 AD转换的步骤：采样->保持->量化->编码。量化->编码过程为ADC逐次比较的过程
输入电压范围：0~3.3V，转换结果范围：0~4095
18个输入通道，可测量16个外部和2个内部信号源
规则组和注入组两个转换单元
模拟看门狗自动监测输入电压范围
STM32F103C8T6 ADC资源：ADC1、ADC2，10个外部输入通道*/

/*DMA（Direct Memory Access）直接存储器存取
DMA可以提供外设和存储器或者存储器和存储器之间的高速数据传输，无须CPU干预，节省了CPU的资源
12个独立可配置的通道： DMA1（7个通道）， DMA2（5个通道）
每个通道都支持软件触发和特定的硬件触发

STM32F103C8T6 DMA资源：DMA1（7个通道）
*/

//本函数使用ADC扫描模式+DMA配合实现完全自动化硬件传输模数转换的数据

uint16_t AD_Value[4];//定义DMA存储器数组，存放传输过来的数据
//AD模数转换初始化
void AD_Init(void)
{
	//开启APB2片上外设的ADC1和GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//DMA1外设在AHB片上外设上，开启时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	//RCC时钟分频ADC,因ADC频率最大为14MHz，所以选择6分频，避免出现数据不精确的情况72MHz/6=12MHz
	//目前ADC时钟频率为12MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	//GPIO初始化，GPIO模式设置为模拟输入模式：GPIO_Mode_AIN
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1 |GPIO_Pin_2 |GPIO_Pin_3;//这里同时选用4个GPIO口
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//ADC规则组通道配置：这里需要扫描四个通道
	//分别选用ADC1的Channel_0，1，2，3，规则组排序分别为1，2，3，4设置采样时间值为555
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);
	//初始化ADC
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//连续模式设置为：连续转换
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数据对齐设置右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//外部触发设置为空，这里我们用软件触发
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//ADC模式为独立模式，如果使用多个ADC同时工作则选择其他模式
	ADC_InitStructure.ADC_NbrOfChannel = 4;//指定要转换的ADC通道的数量为4
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//扫描模式设置为：扫描模式
	ADC_Init(ADC1, &ADC_InitStructure);
	//初始化DMA
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;//DMA外设地址为：ADC1_DR数据寄存器
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//DMA外设传输数据大小为半字
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//DMA外设地址设置不自增
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Value;//DMA存储器地址为：变量AD_Value的地址
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//DMA外设接收数据大小为半字
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable ;//DMA外设地址设置自增：填充完一个数据后，自动跳到下一个地址
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//传输模式为循环模式
	DMA_InitStructure.DMA_BufferSize = 4;//传输计数器值为4
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;////外设作为传输源
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;////触发源设置为硬件触发
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//优先级为中等优先级
	//ADC1特定只能用DMA1的通道1
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	//使能DMA
	DMA_Cmd(DMA1_Channel1, ENABLE);
	//使能ADC到DMA的输出
	ADC_DMACmd(ADC1, ENABLE);
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
	
	//软件触发控制开启ADC1
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
//这里不再需要定义AD转换值获取函数，因为ADC已经通过DMA实现了硬件自动转换传输
/*//定义AD转换值获取函数
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
*/