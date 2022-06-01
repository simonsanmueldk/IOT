
/*
 * Application.c
 *
 * Created: 5/24/2022 7:14:56 PM
 *  Author: Dorin Pascal
 */ 

//Includes
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ATMEGA_FreeRTOS.h>
#include <avr/io.h>
#include <stdio_driver.h>
#include <task.h>
#include <event_groups.h>
#include "TempHumSensor.h"
#include "message_buffer.h"
#include "CO2_Sensor.h"
#include "Application.h"
#include <lora_driver.h>
#include "Utility.h"
#include "SensorData.h"
#include <serial.h>

// Create payload
lora_driver_payload_t payload;
// Create Event Bits
EventBits_t dataReadyEventBits;
// Create message buffers
extern MessageBufferHandle_t xMessageBuffer;
// Create Event Groups
extern EventGroupHandle_t _meassureEventGroup ;
extern EventGroupHandle_t _dataReadyEventGroup;


void Application_Task(void* pvParameters)
{
	// Type of ticks
	TickType_t xLastWakeTime;
	// Upload message every 5 minutes (300000 ms)
	const TickType_t xFrequency = pdMS_TO_TICKS(300000UL);
	xLastWakeTime = xTaskGetTickCount();
	
	//Infinite loop to run the application
	for (;;)
	{
		application_run(&xLastWakeTime,xFrequency);
	}
}

//Create Application Task
void application_task_create(UBaseType_t task_priority)
{
	xTaskCreate(
	Application_Task // Application Task method
	,  "Application"  // Name of task
	,  configMINIMAL_STACK_SIZE  // Stack size
	,  NULL
	,  task_priority  // Priority of the task
	,  NULL );
	
}

//Method to run the application
void application_run(TickType_t* xLastWakeTime,TickType_t xFrequency)
{
	//Set Bits
	xEventGroupSetBits(_meassureEventGroup, CO2_BIT | TEMPERATURE_HUMIDITY_BIT);
	dataReadyEventBits=xEventGroupWaitBits(_dataReadyEventGroup,CO2_BIT | TEMPERATURE_HUMIDITY_BIT,pdTRUE,pdTRUE,portMAX_DELAY);
	
	// If both temperature/humidity and CO2 bits are ready
	if ((dataReadyEventBits & (CO2_BIT | TEMPERATURE_HUMIDITY_BIT) ) == (CO2_BIT | TEMPERATURE_HUMIDITY_BIT))
	{
		// Set the temperature data
		setTemperatureData(get_temperature_data());
		printf("Temperature data ---> %d ",get_temperature_data()); // Just for troubleshooting purpose
		// Set the humidity data
		setHumidityData(get_humidity_data());
		printf(" Humidity data ---> %d ",get_humidity_data()); // Just for troubleshooting purpose
		// Set the CO2 data
		setCO2Ppm(get_CO2_data());
		printf("CO2 data ---> %i ",get_CO2_data()); // Just for troubleshooting purpose
		
		//Set the payload
		payload=getLoRaPayload((uint8_t)2);
		vTaskDelay(pdMS_TO_TICKS(50UL)); // Delay
		//Send the payload so Uplink can recieve it
		xMessageBufferSend(xMessageBuffer,(void*)&payload,sizeof(payload),portMAX_DELAY);
		xTaskDelayUntil(xLastWakeTime, xFrequency ); // Delay
	}
	
}
