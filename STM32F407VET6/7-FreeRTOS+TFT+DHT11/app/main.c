#include "main.h"                  // Device header

//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		128  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

//任务优先级
#define LED0_TASK_PRIO		2
//任务堆栈大小	
#define LED0_STK_SIZE 		50  
//任务句柄
TaskHandle_t LED0Task_Handler;
//任务函数
void led0_task(void *pvParameters);

//任务优先级
#define LED1_TASK_PRIO		3
//任务堆栈大小	
#define LED1_STK_SIZE 		50  
//任务句柄
TaskHandle_t LED1Task_Handler;
//任务函数
void led1_task(void *pvParameters);

//任务优先级
#define FLOAT_TASK_PRIO		4
//任务堆栈大小	
#define FLOAT_STK_SIZE 		128
//任务句柄
TaskHandle_t FLOATTask_Handler;
//任务函数
void float_task(void *pvParameters);

int main(void)
{ 
	board_init();                  // Initialize board
	my_usart_init(); 
	LCD_Init();                  // Initialize LCD
    LCD_Clear(BLUE);
    Delay_ms(1000);
    LCD_Clear(WHITE);
    Delay_ms(1000);
	LED_Init(&led0);
	LED_Init(&led1);

	char hello[100];
	sprintf(hello, "Welcome to stm32!");
	LCD_ShowString(18, 6, 240, 30, 24, (u8*)hello);
	
	//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
}
 
//开始任务任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区
    //创建LED0任务
    xTaskCreate((TaskFunction_t )led0_task,     	
                (const char*    )"led0_task",   	
                (uint16_t       )LED0_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )LED0_TASK_PRIO,	
                (TaskHandle_t*  )&LED0Task_Handler);   
    //创建LED1任务
    xTaskCreate((TaskFunction_t )led1_task,     
                (const char*    )"led1_task",   
                (uint16_t       )LED1_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LED1_TASK_PRIO,
                (TaskHandle_t*  )&LED1Task_Handler);        
    //浮点测试任务
    xTaskCreate((TaskFunction_t )float_task,     
                (const char*    )"float_task",   
                (uint16_t       )FLOAT_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )FLOAT_TASK_PRIO,
                (TaskHandle_t*  )&FLOATTask_Handler);  
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}

//LED0任务函数 
void led0_task(void *pvParameters)
{
    while(1)
    {
        LED_ON(&led0);
        vTaskDelay(500);
		LED_OFF(&led0);
		vTaskDelay(500);
    }
}   

//LED1任务函数
void led1_task(void *pvParameters)
{
    while(1)
    {
        LED_ON(&led1);
        vTaskDelay(50);
        LED_OFF(&led1);
        vTaskDelay(50);
    }
}

//浮点测试任务
void float_task(void *pvParameters)
{
//	static float float_num=0.00;
	while(1)
	{
		usart_rx_flush();
//		float_num+=0.01f;
//		shellPrint(shellGetCurrent(), "float_num的值为: %.4f\r\n",float_num);
        vTaskDelay(10);
	}
}


//int main(void)
//{
//	mem_init();                 // Initialize memory
//	board_init();                  // Initialize board
//	my_usart_init(); 
//	LCD_Init();                  // Initialize LCD
//    LCD_Clear(BLUE);
//    Delay_ms(1000);
//    LCD_Clear(WHITE);
//    Delay_ms(1000);

//	char hello[100];
//	sprintf(hello, "Welcome to stm32!");
//	LCD_ShowString(18, 6, 240, 30, 24, (u8*)hello);
//	while(1)
//	{	
//		// Check if there is any data in the RX buffer
//		usart_rx_flush();

//	}
//}
