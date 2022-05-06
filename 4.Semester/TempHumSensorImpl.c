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
 EventGroupHandle_t _meassureEventGroup ;
 EventGroupHandle_t _dataReadyEventGroup;

float temperature = 0.0;
float humidity = 0.0;

typedef struct Temp_Humidity_Sensor {
	float temperature_data;
	float humidity_data;
}Temp_Humidity_Sensor;

//-------Constructor-------------------

void Temp_Humidty_sensor_create(EventGroupHandle_t event1,EventGroupHandle_t event2)
{
	temperature_humdity_sensor_t new_temperature_humidty = pvPortMalloc(sizeof(Temp_Humidity_Sensor));
	if(NULL == new_temperature_humidty)
	{
		return NULL;
	}
	_meassureEventGroup=event1;
	_dataReadyEventGroup=event2;
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
		return (uint16_t)humidity;	
}


//Bit for set
#define TEMPERATURE_HUMIDITY_READY_BIT (1 << 0)
//Bit for wait
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

/* Get Data from the sensors */
void tempHum_getDataFromTempHumSensorTask( void *pvParameters )
{
	(void)pvParameters;
	
	EventBits_t event_measure;
	
	for(;;)
	{
		
		//Wait for Event bits to be set in Group
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
			humidity =  hih8120_getHumidity();
			temperature = hih8120_getTemperature();
			xEventGroupSetBits(_dataReadyEventGroup, TEMPERATURE_HUMIDITY_READY_BIT);
			
		}
	}	
		else{
			puts("hahah");
		}
		
		}
	}

