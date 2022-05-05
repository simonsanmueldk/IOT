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
#include "dataShare.h"

/* Event Groups */
EventGroupHandle_t _meassureEventGroup = NULL;
EventGroupHandle_t _dataReadyEventGroup = NULL;

// Left shift 0 times
#define TEMPERATURE_HUMIDITY_READY_BIT (1 << 0)

// Left shift 1 time
#define TEMPERATURE_HUMIDITY_BIT (1 << 1)

/* Create Event Groups*/


/* Tick type */
TickType_t xLastWakeTime;
TickType_t xFrequency;

/* Task to run for the sensors to work */
void tempHum_init() {
	if ( HIH8120_OK == hih8120_initialise() )
	{
		_meassureEventGroup = xEventGroupCreate();
		_dataReadyEventGroup =xEventGroupCreate();
		// Driver initialized OK
		// Always check what hih8120_initialise() returns
	}	else printf("Driver doesn't start");
}

void tempHum_taskRun() {
	
	EventBits_t event;
	
	/*Wait for Event bits to be set in Group*/
	event = xEventGroupWaitBits(
	_meassureEventGroup,
	TEMPERATURE_HUMIDITY_BIT,
	pdTRUE,
	pdFALSE,
	portMAX_DELAY);	
	
	xTaskDelayUntil( &xLastWakeTime, xFrequency );
	
	if (HIH8120_OK != hih8120_wakeup())
	{
		vTaskDelay(pdMS_TO_TICKS(100));
		printf("\n ---Temp/humidity sensor couldn't wake up trying again--");
		while(HIH8120_OK != hih8120_wakeup())
		{
			vTaskDelay(pdMS_TO_TICKS(100));
		}
	}
	vTaskDelay(pdMS_TO_TICKS(50));
	
	if (HIH8120_OK !=  hih8120_measure() )
	{
		vTaskDelay(pdMS_TO_TICKS(100));
		while(HIH8120_OK !=  hih8120_measure())
		{
			vTaskDelay(pdMS_TO_TICKS(100));
		}
	}
	
	vTaskDelay(pdMS_TO_TICKS(500));
	humidity =  hih8120_getHumidity();
	temperature = hih8120_getTemperature();
	
}

	/* Get Data from the sensors */
	void tempHum_getDataFromTempHumSensorTask( void *pvParameters )
	{
		/*Set event bits in group */
		xEventGroupSetBits(_dataReadyEventGroup, TEMPERATURE_HUMIDITY_READY_BIT);
		
		xFrequency = 5000/portTICK_PERIOD_MS; // 1000 ms

		//Initialize the xLastWakeTime variable with the current time.
		xLastWakeTime = xTaskGetTickCount();

		for(;;)
		{
			taskENTER_CRITICAL();
			tempHum_taskRun();
			taskEXIT_CRITICAL();
		}
		
	}

	/* Initialize the driver */
