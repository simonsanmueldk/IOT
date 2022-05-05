/*
* TempHum.c
*
* Created: 01/05/2022 21.40.29
*  Author: Simon
*/

#include <stdio.h>
#include <stdlib.h>
#include <hih8120.h>
#include <ATMEGA_FreeRTOS.h>
#include <avr/io.h>

#include "event_groups.h"
#include "TempHumSensor.h"

/* Event Groups */
EventGroupHandle_t _meassureEventGroup = NULL;
extern EventGroupHandle_t _dataReadyEventGroup = NULL;

float temperature = 0.0;
float humidity = 0.0;

typedef struct Temp_Humidity_Sensor {
	uint16_t temperature_data;
	uint16_t humidity_data;
}Temp_Humidity_Sensor;

//-------Constructor-------------------

temperature_humdity_sensor_t temperature_humidity_create(uint16_t temperature_data, uint16_t humidity_data)
{
	temperature_humdity_sensor_t new_temperature_humidty = pvPortMalloc(sizeof(Temp_Humidity_Sensor));
	if(NULL == new_temperature_humidty)
	{
		return NULL;
	}
	return new_temperature_humidty;
}
//------Get temperature data-----------

uint16_t get_temperature_data()
{
	puts("Task1");
		return (uint16_t)temperature;
}

//-------Get humidity data--------
uint16_t get_humidity_data()
{
		return (uint16_t)humidity;	
}


// Left shift 0 times
#define TEMPERATURE_HUMIDITY_READY_BIT (1 << 0)
// Left shift 1 time
#define TEMPERATURE_HUMIDITY_BIT (1 << 1)

/* Tick type */
TickType_t xLastWakeTime;
TickType_t xFrequency;

/* Task to run for the sensors to work */
void tempHum_init() {
	if ( HIH8120_OK == hih8120_initialise() )
	{
		// Driver initialized OK
		// Always check what hih8120_initialise() returns
	}	else printf("Driver doesn't start");
}

void tempHum_taskRun() {
	
	EventBits_t event;
	/*Wait for Event bits to be set in Group
	event = xEventGroupWaitBits(
	_meassureEventGroup,
	TEMPERATURE_HUMIDITY_BIT,
	pdTRUE,
	pdFALSE,
	portMAX_DELAY);	'
	*/
	
	//xTaskDelayUntil( &xLastWakeTime, xFrequency );
	
	if (HIH8120_OK != hih8120_wakeup())
	{
		vTaskDelay(pdMS_TO_TICKS(100));
		printf("\n ---Temp/humidity sensor couldn't wake up trying again--");
		while(HIH8120_OK == hih8120_wakeup())
		{
			vTaskDelay(pdMS_TO_TICKS(50UL));
		}
	}
	puts("Task2");
	
	hih8120_measure();
	vTaskDelay(pdMS_TO_TICKS(5UL));
	
	if (HIH8120_OK != hih8120_measure() )
	{
		vTaskDelay(pdMS_TO_TICKS(100UL));
		while(HIH8120_OK ==  hih8120_measure())
		{
			vTaskDelay(pdMS_TO_TICKS(50UL));
		}
		humidity =  hih8120_getHumidity();
		temperature = hih8120_getTemperature();
		
		
	}
}

/* Get Data from the sensors */
void tempHum_getDataFromTempHumSensorTask( void *pvParameters )
{
	puts("Task3");
	/*Set event bits in group
	xEventGroupSetBits(_dataReadyEventGroup, TEMPERATURE_HUMIDITY_READY_BIT);
	*/
	xFrequency = 5000/portTICK_PERIOD_MS; // 1000 ms
	//Initialize the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
	_meassureEventGroup = xEventGroupCreate();
	_dataReadyEventGroup =xEventGroupCreate();
	
	for(;;)
	{
		tempHum_taskRun();
	}
}

