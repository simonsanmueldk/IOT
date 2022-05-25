
/*
 * Application.c
 *
 * Created: 5/24/2022 7:14:56 PM
 *  Author: Dorin Pascal
 */ 

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <event_groups.h>
#include "TempHumSensor.h"
#include "message_buffer.h"
#include "CO2_Sensor.h"
#include "Application.h"
#include <lora_driver.h>
#include "Utility.h"
#include "SensorData.h"


lora_driver_payload_t payload;
EventBits_t dataReadyEventBits;

extern MessageBufferHandle_t xMessageBuffer;
extern EventGroupHandle_t _meassureEventGroup ;
extern EventGroupHandle_t _dataReadyEventGroup;

void Application_Task(void* pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = pdMS_TO_TICKS(300000UL); // Upload message every 5 minutes (300000 ms)
	xLastWakeTime = xTaskGetTickCount();
	
	for (;;)
	{
		application_run(xLastWakeTime,xFrequency);
	}
}
void application_task_create(UBaseType_t task_priority)
{
	xTaskCreate(
	Application_Task
	,  "Application"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack High water
	,  NULL
	,  task_priority  // Priority.
	,  NULL );
	
}
void application_run(TickType_t xLastWakeTime,TickType_t xFrequency)
{
	
	xEventGroupSetBits(_meassureEventGroup,CO2_BIT | HUMIDITY_TEMPERATURE_BIT);
	dataReadyEventBits=xEventGroupWaitBits(_dataReadyEventGroup,CO2_BIT | HUMIDITY_TEMPERATURE_BIT,pdTRUE,pdTRUE,portMAX_DELAY);
	if ((dataReadyEventBits &(CO2_BIT | HUMIDITY_TEMPERATURE_BIT)  )== (CO2_BIT | HUMIDITY_TEMPERATURE_BIT))
	{
		
		setTemperatureData(get_temperature_data());
		printf("Temperature data ---> %d ",get_temperature_data());
		setHumidityData(get_humidity_data());
		printf(" Humidity data ---> %d ",get_humidity_data());
		setCO2Ppm(get_CO2_data());
		printf("CO2 data ---> %i ",get_CO2_data());
		
		payload=getLoRaPayload((uint8_t)2);
		vTaskDelay(pdMS_TO_TICKS(50UL));
		xMessageBufferSend(xMessageBuffer,(void*)&payload,sizeof(payload),portMAX_DELAY);
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
	}
	
}