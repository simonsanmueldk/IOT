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
#include <mh_z19.h>
#include <event_groups.h>


#include <hih8120.h>

 // Needed for LoRaWAN
#include <lora_driver.h>
#include <status_leds.h>
#include "TempHumSensor.h"
#include "message_buffer.h"
#include "UpLinkHandler.h"
#include "DownLinkHandler.h"
#include "CO2_Sensor.h"
#include <rc_servo.h>


// define two Tasks
void Temperature_Humidity_Task(void *pvParameters);
void Application_Task(void *pvParameters );
void CO2_Task(void *pvParameters );

//Bit for set
#define HUMIDITY_TEMPERATURE_BIT (1<<0)
#define CO2_BIT (1<<1)




// define semaphore handle
SemaphoreHandle_t xTemperatureHumiditySemaphore;
SemaphoreHandle_t xUpLinkSemaphore;
SemaphoreHandle_t xCO2Semaphore;

// Prototype for LoRaWAN handler
void lora_handler_initialise(UBaseType_t lora_handler_task_priority);

 MessageBufferHandle_t xMessageBuffer;
 MessageBufferHandle_t downlinkMessageBuffer;
 const size_t xMessageBufferSizeBytes = 100;
 EventBits_t measureEventGroup;
EventBits_t dataReadyEventGroup;
/*-----------------------------------------------------------*/

void create_tasks_and_semaphores(void)
{
	xMessageBuffer = xMessageBufferCreate( xMessageBufferSizeBytes );
	downlinkMessageBuffer = xMessageBufferCreate(sizeof(lora_driver_payload_t)*2);;
	
	
	SensorDataPackage_create();
	measureEventGroup=xEventGroupCreate();
	dataReadyEventGroup=xEventGroupCreate();
	Temp_Humidty_sensor_create(measureEventGroup,dataReadyEventGroup);
	CO2_sensor_create(measureEventGroup,dataReadyEventGroup);
	// Semaphores are useful to stop a Task proceeding, where it should be paused to wait,
	// because it is sharing a resource, such as the Serial port.
	// Semaphores should only be used whilst the scheduler is running, but we can set it up here.
	if ( xUpLinkSemaphore == NULL )  // Check to confirm that the Semaphore has not already been created.
	{
		xUpLinkSemaphore = xSemaphoreCreateMutex(); 
		if ( ( xUpLinkSemaphore ) != NULL )
		{
			xSemaphoreGive( ( xUpLinkSemaphore ) ); 
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
	
xTaskCreate(
	CO2_Task
	,  "CO2 Task"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  1 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	
	
	
}

/*-----------------------------------------------------------*/
void Temperature_Humidity_Task( void *pvParameters )
{
    tempHum_getDataFromTempHumSensorTask(pvParameters);
}


void CO2_Task( void *pvParameters )
{	
	CO2_Sensor_Task(pvParameters);
}

/*-----------------------------------------------------------*/
void Application_Task(void* pvParameters)
{
	lora_driver_payload_t payload;
	EventBits_t dataReadyEventBits;
		TickType_t xLastWakeTime;
		const TickType_t xFrequency = pdMS_TO_TICKS(300000UL); // Upload message every 5 minutes (300000 ms)
		xLastWakeTime = xTaskGetTickCount();
		
	
	for (;;)
	{
		
		xEventGroupSetBits(measureEventGroup,CO2_BIT | HUMIDITY_TEMPERATURE_BIT);
		dataReadyEventBits=xEventGroupWaitBits(dataReadyEventGroup,CO2_BIT | HUMIDITY_TEMPERATURE_BIT,pdTRUE,pdTRUE,portMAX_DELAY);
		if ((dataReadyEventBits &(CO2_BIT | HUMIDITY_TEMPERATURE_BIT)  )== (CO2_BIT | HUMIDITY_TEMPERATURE_BIT))
		{
		
			setTemperatureData(get_temperature_data());
			printf("Temperature data ---> %d ",get_temperature_data());
			setHumidityData(get_humidity_data());
			printf(" Humidity data ----> %d ",get_humidity_data());
			setCO2Ppm(get_CO2_data());
			printf("CO2 data ---> %i ",get_CO2_data());
			
			payload=getLoRaPayload((uint8_t)2);
			vTaskDelay(pdMS_TO_TICKS(50UL));
			xMessageBufferSend(xMessageBuffer,(void*)&payload,sizeof(payload),portMAX_DELAY);
			xTaskDelayUntil( &xLastWakeTime, xFrequency );
		}

		
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
	mh_z19_initialise(ser_USART3);
	 rc_servo_initialise();
	create_tasks_and_semaphores();
       // Driver initialised OK
       // Always check what hih8120_initialise() returns
	

	// vvvvvvvvvvvvvvvvv BELOW IS LoRaWAN initialisation vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	// Status Leds driver
	status_leds_initialise(5); // Priority 5 for internal task
	// Initialise the LoRaWAN driver without down-link buffer
	lora_driver_initialise(1, downlinkMessageBuffer);
	// Create LoRaWAN task and start it up with priority 3
	//lora_handler_initialise(3);
	lora_DownLinkHandler_create(5,downlinkMessageBuffer);
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

