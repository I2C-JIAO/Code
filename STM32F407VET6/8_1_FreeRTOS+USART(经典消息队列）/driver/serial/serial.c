#include "serial.h"

/* 重定义fputc函数 */
int fputc( int ch, FILE *f )
{
	USART_TypeDef* USARTx = USART1;
	while ((USARTx->SR & (1<<7)) == 0);
	USARTx->DR = ch;
	return ch;
}

//发送一个字节
void Serial_Send(uint8_t data)
{
    USART_SendData(USART1, data);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

//串口接收：中断服务程序
void USART1_IRQHandler(void)
{
	uint8_t Serial_RxData;
    //等待接收到数据
    if ((USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET))
    {
        //接收一个字节
		Serial_RxData = USART_ReceiveData(USART1);
		Serial_Send(Serial_RxData);
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}

//串口初始化
void Serial_Init(uint32_t bound_rate)
{
	/* 配置中断优先级分组 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	/* 开启时钟 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //使能GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//使能USART1时钟
    /* 串口1对应引脚复用映射 */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);  //PA9复用为USART1_TX
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); //PA10复用为USART1_RX

    /* GPIO端口配置 */
	GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_9 | GPIO_Pin_10;  //GPIOA9与GPIOA10
    GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_AF;              //复用功能
    GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;          //速度50MHz
    GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;             //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_UP;              //上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);                      //初始化PA9，PA10

    /* USART1 初始化设置 */
	USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate              = bound_rate;                       //波特率设置
    USART_InitStructure.USART_WordLength            = USART_WordLength_8b;              //8位数据位长
    USART_InitStructure.USART_StopBits              = USART_StopBits_1;                 //一个停止位
    USART_InitStructure.USART_Parity                = USART_Parity_No;                  //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl   = USART_HardwareFlowControl_None;   //无硬件数据流控制
    USART_InitStructure.USART_Mode                  = USART_Mode_Rx | USART_Mode_Tx;    //收发模式
    USART_Init(USART1, &USART_InitStructure);                                           //初始化串口
    
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	//中断通道为USART1的中断
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1, ENABLE); 

}
