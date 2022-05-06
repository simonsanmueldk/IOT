/*
* main.c
* Author : Dorin Pascal
*
* Example main file including LoRaWAN setup
* 
*/

#include <stdio.h>
#include <avr/io.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "SensorData.h"
#include <stdio_driver.h>
#include <serial.h>

#include <event_groups.h>


#include <hih8120.h>

 // Needed for LoRaWAN
#include <lora_driver.h>
#include <status_leds.h>
#include "TempHumSensor.h"
#include "message_buffer.h"
#include "UpLinkHandler.h"


// define two Tasks
void Temperature_Humidity_Task(void *pvParameters);
void Application_Task(void *pvParameters );

//Bit for set
#define ALL_MEASURE_BITS (1<<0)
//Bit for wait
#define ALL_READY_BITS (1<<1)




// define semaphore handle
SemaphoreHandle_t xTestSemaphore;

// Prototype for LoRaWAN handler
void lora_handler_initialise(UBaseType_t lora_handler_task_priority);

 MessageBufferHandle_t xMessageBuffer;
 const size_t xMessageBufferSizeBytes = 100;
 EventBits_t measureEventGroup;
EventBits_t dataReadyEventGroup;
/*-----------------------------------------------------------*/

void create_tasks_and_semaphores(void)
{
	xMessageBuffer = xMessageBufferCreate( xMessageBufferSizeBytes );
	
	SensorDataPackage_create();
	measureEventGroup=xEventGroupCreate();
	dataReadyEventGroup=xEventGroupCreate();
	Temp_Humidty_sensor_create(measureEventGroup,dataReadyEventGroup);
	// Semaphores are useful to stop a Task proceeding, where it should be paused to wait,
	// because it is sharing a resource, such as the Serial port.
	// Semaphores should only be used whilst the scheduler is running, but we can set it up here.
	if ( xTestSemaphore == NULL )  // Check to confirm that the Semaphore has not already been created.
	{
		xTestSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore.
		if ( ( xTestSemaphore ) != NULL )
		{
			xSemaphoreGive( ( xTestSemaphore ) );  // Make the mutex available for use, by initially "Giving" the Semaphore.
		}
	}

	xTaskCreate(
	Application_Task
	,  "Application"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	
xTaskCreate(
	Temperature_Humidity_Task
	,  "Temperature_Humidity"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  1 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	
	
	
}

/*-----------------------------------------------------------*/
//void task1( void *pvParameters )
//{
	//TickType_t xLastWakeTime;
	//const TickType_t xFrequency = 500/portTICK_PERIOD_MS; // 500 ms
//
	//// Initialise the xLastWakeTime variable with the current time.
	//xLastWakeTime = xTaskGetTickCount();
//
	//for(;;)
	//{
		//xTaskDelayUntil( &xLastWakeTime, xFrequency );
		//puts("Task1"); // stdio functions are not reentrant - Should normally be protected by MUTEX
		//PORTA ^= _BV(PA0);
	//}
//}
//
///*-----------------------------------------------------------*/


void Temperature_Humidity_Task( void *pvParameters )
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 500/portTICK_PERIOD_MS; // 500 ms
	//
	//// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
		
		tempHum_getDataFromTempHumSensorTask(pvParameters);
		
		
	
}
/*-----------------------------------------------------------*/
void Application_Task(void* pvParameters)
{
	lora_driver_payload_t payload;
	EventBits_t dataReadyEventBits;
	
	
	for (;;)
	{
		xEventGroupSetBits(measureEventGroup,ALL_READY_BITS);
		dataReadyEventBits=xEventGroupWaitBits(dataReadyEventGroup,ALL_MEASURE_BITS,pdTRUE,pdFALSE,portMAX_DELAY);
		if ((dataReadyEventBits & ALL_MEASURE_BITS  )==ALL_MEASURE_BITS)
		{
			setCO2Ppm(1050);
			setTemperatureData(get_temperature_data());
			printf("Task data %d",get_temperature_data());
			setHumidityData(get_humidity_data());
			printf(" Task data %d",get_humidity_data());
		}

		payload=getLoRaPayload((uint8_t)2);
		vTaskDelay(pdMS_TO_TICKS(50UL));
		xMessageBufferSend(xMessageBuffer,(void*)&payload,sizeof(payload),portMAX_DELAY);
	}
	
	
	
	
}
void initialiseSystem()
{
	// Set output ports for leds used in the example
	DDRA |= _BV(DDA0) | _BV(DDA7);

	// Make it possible to use stdio on COM port 0 (USB) on Arduino board - Setting 57600,8,N,1
	stdio_initialise(ser_USART0);
	// Let's create some tasks
	
    tempHum_init();
	puts("Task0");
	create_tasks_and_semaphores();
       // Driver initialised OK
       // Always check what hih8120_initialise() returns
	

	// vvvvvvvvvvvvvvvvv BELOW IS LoRaWAN initialisation vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	// Status Leds driver
	status_leds_initialise(5); // Priority 5 for internal task
	// Initialise the LoRaWAN driver without down-link buffer
	lora_driver_initialise(1, NULL);
	// Create LoRaWAN task and start it up with priority 3
	//lora_handler_initialise(3);
upLink_create(4,xMessageBuffer);
}

/*-----------------------------------------------------------*/
int main(void)
{
	initialiseSystem(); // Must be done as the very first thing!!
	//printf("Program Started!!\n");
	vTaskStartScheduler(); // Initialise and run the freeRTOS scheduler. Execution should never return from here.

	/* Replace with your application code */
	
}

