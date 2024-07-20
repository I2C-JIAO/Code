#include "my_usart.h"

static usart_callback_t rx_callback;

//初始化串口GPIO
void usart_pin_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //GPIO引脚复用设置
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
}

//初始化串口硬件
void usart_lowlevel_init(void)
{
    USART_InitTypeDef USART_InitStructure;
    USART_StructInit(&USART_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &USART_InitStructure);
    //使能串口
    USART_Cmd(USART1, ENABLE);
}

//初始化串口中断
void usart_irq_init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    //使能串口中断
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    //使能串口接收中断    
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

//发送一个字节
void my_usart_send(uint8_t data)
{
    USART_SendData(USART1, data);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

void usart_recv_callback_register(usart_callback_t callback)
{
    rx_callback = callback;
}

void usart_rx_handler(uint8_t data)
{
    //处理接收到的数据
    rx_data = data;
}

void my_usart_recv(void)
{
    //接收回调函数注册
    usart_recv_callback_register(usart_rx_handler);
}

//串口接收：中断服务程序
void USART1_IRQHandler(void)
{
    //等待接收到数据
    if ((USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET))
    {
        //接收一个字节
        uint8_t data = USART_ReceiveData(USART1);
        if (rx_callback)
        {
            //调用回调函数
            (rx_callback)(data);
        }
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}

//初始化串口程序
void my_usart_init(void)
{
    //初始化GPIO
    usart_pin_init();
    //初始化串口
    usart_lowlevel_init();
    //初始化中断
    usart_irq_init();
    //初始化接收回调函数
    my_usart_recv();
}
