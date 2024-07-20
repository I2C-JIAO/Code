#include "main.h"                  // Device header

QueueHandle_t xQueue;

typedef enum {
	SPEED,
	DISTENCE
}ID_t;

typedef struct {
	ID_t id;
	int32_t value;
}Data_t;

static const Data_t xStructsToSend[2] = {
	{SPEED, 120},
	{DISTENCE, 1000}
};

void vTask1 ( void * param)
{
	BaseType_t xStatus;
	const TickType_t xTicksToWait = pdMS_TO_TICKS(100UL);
	while(1)
	{
		xStatus = xQueueSend(xQueue, param, xTicksToWait);
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
	BaseType_t xStatus;
	const TickType_t xTicksToWait = pdMS_TO_TICKS(100UL);
	while(1)
	{
		xStatus = xQueueSend(xQueue, param, xTicksToWait);
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
	Data_t xReceivedStructure;
	BaseType_t xStatus;
	while(1)
	{
		xStatus = xQueueReceive(xQueue, &xReceivedStructure, 0);

		if(xStatus == pdPASS)
		{
			LED_ON(&led2);
			if(xReceivedStructure.id == SPEED)
			{
				printf("Speed = %d\r\n", xReceivedStructure.value);
			}
			else if(xReceivedStructure.id == DISTENCE)
			{
				printf("Distence = %d\r\n", xReceivedStructure.value);
			}
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
	
	xQueue = xQueueCreate(5, sizeof(Data_t));
	if(xQueue != NULL)
	{
		xTaskCreate(vTask1, "Task1", 1000, (void *)&(xStructsToSend[0]), 2, NULL);
		xTaskCreate(vTask2, "Task2", 1000, (void *)&(xStructsToSend[1]), 2, NULL);

		xTaskCreate(vTask3, "Task3", 1000, NULL, 1, NULL);
		
		vTaskStartScheduler();
	}
	else
	{
		printf("Queue creation failed\r\n");
		return 1;
	}
	
	return 0;	
}
