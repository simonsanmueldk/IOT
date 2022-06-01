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

// Event Groups
extern EventGroupHandle_t _meassureEventGroup ;
extern EventGroupHandle_t _dataReadyEventGroup;

//Bit to set event groups
#define TEMPERATURE_HUMIDITY_READY_BIT (1 << 1)

//Tick types
TickType_t xLastWakeTime;
TickType_t xFrequency;

//Default temperature and humidity values
float temperature = 0;
uint16_t humidity = 0;

EventBits_t event_measure;

//Structure
typedef struct Temp_Humidity_Sensor {
	uint16_t temperature_data;
	float humidity_data;
}Temp_Humidity_Sensor;

//Constructor using pvPortMalloc()
void Temp_Humidty_sensor_create()
{
	temperature_humdity_sensor_t new_temperature_humidty = pvPortMalloc(sizeof(Temp_Humidity_Sensor));
	if(NULL == new_temperature_humidty)
	{
		return NULL;
	}
	//Set a default values for a created structure
	new_temperature_humidty->humidity_data=humidity;
	new_temperature_humidty->temperature_data=temperature;
	
}
// Get temperature data
uint16_t get_temperature_data()
{
	return (uint16_t)temperature;
}

// Get humidity data
uint16_t get_humidity_data()
{
	return humidity;
}


//Method to create a task to measure the temperature and humidity
void tempHum_taskCreate(UBaseType_t task_priority){
	xTaskCreate(
	tempHum_Task //Task
	,  "Temperature_Humidity"  // A name of task
	,  configMINIMAL_STACK_SIZE  // This stack size
	,  NULL
	,  task_priority // Priority od the task
	,  NULL );
}


// Task to run for the temperature/humididty sensors to work
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
			//In case the sensor is not ready yet then delay method
			vTaskDelay(pdMS_TO_TICKS(100UL));
			//Print error message for a troubleshhoting purpose
			printf("\n ---Temp/humidity sensor couldn't wake up trying again--");
			while(HIH8120_OK == hih8120_wakeup())
			{
				vTaskDelay(pdMS_TO_TICKS(50UL));
			}
		}
		//Perform measurement of temperature and humidity 
		hih8120_measure();
		//Delay method
		vTaskDelay(pdMS_TO_TICKS(50UL));
		if (HIH8120_OK == hih8120_measure() )
		{
			//Delay method
			vTaskDelay(pdMS_TO_TICKS(100UL));
			//Get the measured temperature and humidity
			humidity =  hih8120_getHumidityPercent_x10();
			temperature = hih8120_getTemperature_x10();
			//Set the bits of the event group
			xEventGroupSetBits(_dataReadyEventGroup, TEMPERATURE_HUMIDITY_READY_BIT);
		}
	}
}

void tempHum_Task( void *pvParameters )
{
	(void)pvParameters;
	//Infinite loop that runs the task
	for(;;)
	{
		//Calling method to performs measurements
		tempHum_Run();
	}
}

