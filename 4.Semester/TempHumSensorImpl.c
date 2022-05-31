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
#include "Utility.h"
#include "TempHumSensor.h"
#include "event_groups.h"

/* Event Groups */
extern EventGroupHandle_t _meassureEventGroup ;
extern EventGroupHandle_t _dataReadyEventGroup;
 //Bit for set
 #define TEMPERATURE_HUMIDITY_READY_BIT (1 << 1)


/* Tick type */
TickType_t xLastWakeTime;
TickType_t xFrequency;

float temperature = 0;
uint16_t humidity = 0;
EventBits_t event_measure;

typedef struct Temp_Humidity_Sensor {
	uint16_t temperature_data;
	float humidity_data;
}Temp_Humidity_Sensor;

//-------Constructor-------------------

void Temp_Humidty_sensor_create()
{
	temperature_humdity_sensor_t new_temperature_humidty = pvPortMalloc(sizeof(Temp_Humidity_Sensor));
	if(NULL == new_temperature_humidty)
	{
		return NULL;
	}
	new_temperature_humidty->humidity_data=humidity;
	new_temperature_humidty->temperature_data=temperature;
	
}
//------Get temperature data-----------

uint16_t get_temperature_data()
{
	return (uint16_t)temperature;
}

//-------Get humidity data--------
uint16_t get_humidity_data()
{
	return humidity;
}

void tempHum_taskCreate(UBaseType_t task_priority){
	
	xTaskCreate(
	tempHum_Task
	,  "Temperature_Humidity"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  task_priority // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	
}


/* Task to run for the sensors to work */
void tempHum_Run() {
	event_measure = xEventGroupWaitBits(
	_meassureEventGroup,
	TEMPERATURE_HUMIDITY_BIT,
	pdTRUE,
	pdTRUE,
	portMAX_DELAY);
		
	if ((event_measure & TEMPERATURE_HUMIDITY_BIT) ==TEMPERATURE_HUMIDITY_BIT)
	{
		vTaskDelay( pdMS_TO_TICKS(100UL));
		if (HIH8120_OK != hih8120_wakeup())
		{
			vTaskDelay(pdMS_TO_TICKS(100UL));
			printf("\n ---Temp/humidity sensor couldn't wake up trying again--");
			while(HIH8120_OK == hih8120_wakeup())
			{
				vTaskDelay(pdMS_TO_TICKS(50UL));
			}
		}
		hih8120_measure();
		vTaskDelay(pdMS_TO_TICKS(50UL));
		if (HIH8120_OK == hih8120_measure() )
		{
			vTaskDelay(pdMS_TO_TICKS(100UL));
			humidity =  hih8120_getHumidityPercent_x10();
			temperature = hih8120_getTemperature_x10();
			xEventGroupSetBits(_dataReadyEventGroup, TEMPERATURE_HUMIDITY_READY_BIT);
		}
	}
}

void tempHum_Task( void *pvParameters )
{
	(void)pvParameters;
	for(;;)
	{
		tempHum_Run();
	}
}

