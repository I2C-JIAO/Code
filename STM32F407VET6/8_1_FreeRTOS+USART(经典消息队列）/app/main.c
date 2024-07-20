#include "main.h"                  // Device header

#define QUEUE_LENGTH 10
#define ITEM_SIZE sizeof(uint32_t)

QueueHandle_t xQueue;

void vTask1 ( void * param)
{
	int32_t lvalueToSend;
	BaseType_t xStatus;
	lvalueToSend = (int32_t)param;
	while(1)
	{
		xStatus = xQueueSend(xQueue, &lvalueToSend, 0);
		if (xStatus == pdPASS)
		{
			LED_ON(&led0);
		}
		else
		{
			printf("Task1 failed to send to queue\r\n");
		}
	}
}

void vTask2 ( void * param)
{
	int32_t lvalueToSend;
	BaseType_t xStatus;
	lvalueToSend = (int32_t)param;
	while(1)
	{
		xStatus = xQueueSend(xQueue, &lvalueToSend, 0);
		if (xStatus == pdPASS)
		{
			LED_ON(&led1);
		}
		else
		{
			printf("Task2 failed to send to queue\r\n");
		}
	}
}

void vTask3 ( void * param)
{
	int32_t lReceivedValue;
	BaseType_t xStatus;
	const TickType_t xTicksToWait = pdMS_TO_TICKS(100UL);
	while(1)
	{
		xStatus = xQueueReceive(xQueue, &lReceivedValue, xTicksToWait);

		if(xStatus == pdPASS)
		{
			LED_ON(&led2);
			printf("Received value = %d\r\n", lReceivedValue);
		}
		else
		{
			printf("Failed to receive from queue\r\n");
		}
	}
}

static void prvSetupHardware( void )
{
	Serial_Init(115200);
	LED_Init(&led0);
	LED_Init(&led1);
	LED_Init(&led2);
	
}

int main( void )
{
	prvSetupHardware();
	
	xQueue = xQueueCreate(QUEUE_LENGTH, ITEM_SIZE);
	if(xQueue != NULL)
	{
		xTaskCreate(vTask1, "Task1", 1000, (void *)100, 1, NULL);
		xTaskCreate(vTask2, "Task2", 1000, (void *)200, 1, NULL);

		xTaskCreate(vTask3, "Task3", 1000, NULL, 2, NULL);
		
		vTaskStartScheduler();
	}
	else
	{
		printf("Queue creation failed\r\n");
		return 1;
	}
	
	return 0;	
}
