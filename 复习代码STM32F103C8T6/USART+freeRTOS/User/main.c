#include "main.h"

static uint8_t rx_buff[128];
static ringbuffer8_t rxrb;
static Shell shell;
static char shell_buffer[256];

//系统板初始化函数
 void borad_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	usart_init();
	mem_init();
	LED_Init();
	LED_ON();
}

static void LED_Task(void* parameter)
{
	while(1)
	{
		LED_ON();
		vTaskDelay(500);
		shellPrint(shellGetCurrent(), "LED_Task Running,LED1_ON\r\n");
		LED_OFF();
		vTaskDelay(500);
		shellPrint(shellGetCurrent(), "LED_Task Running,LED1_OFF\r\n");
	}
}

signed short _shell_write(char *data , unsigned short len)
{
	for (unsigned short i = 0; i < len; i++)
	{
		usart_send_byte((uint8_t)data[i]);
	}
	return len;
}

static void usart_rx_handler(uint8_t data)
{
	if(!rb8_full(rxrb))
	{
		rb8_put(rxrb, data);
	}
}

int main(void)
{	
	borad_init();
	usart_callback_register(usart_rx_handler);
	rxrb = rb8_new(rx_buff, sizeof(rx_buff));
	shell.write = _shell_write;
	shellInit(&shell, shell_buffer, sizeof(shell_buffer));
	uint8_t rxdata;
	while(1)
	{	
		if(!rb8_empty(rxrb))
		{
			rb8_get(rxrb, &rxdata);
			shellHandler(&shell, rxdata);
		}
	}
}
