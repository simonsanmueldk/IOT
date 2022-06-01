/*
 * CO2_SensorImpl.c
 *
 * Created: 05/05/2022 12:36:21
 *  Author: Chen
 */ 

#include "CO2_Sensor.h"
#include "mh_z19.h"
#include "event_groups.h"
#include "task.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <serial.h>
#include <ATMEGA_FreeRTOS.h>
#include "Utility.h"

// Create event groups
extern EventGroupHandle_t _dataReadyEventGroup ;
extern EventGroupHandle_t _meassureEventGroup ;
#define CO2_READY_BIT (1<<0)


// Tick types
TickType_t xLastWakeTime;	
TickType_t xFrequency;

// Return code Variable
mh_z19_returnCode_t rc;
// CO2 Value Variable
uint16_t co2_data;

// Structure
typedef struct CO2_Sensor {
	uint16_t co2_Data;
}CO2_Sensor;


// Constructor (create method using pvPortMalloc())
void CO2_sensor_create()
{
	co2_sensor_t new_co2_data = pvPortMalloc(sizeof(CO2_Sensor));
	if(NULL == new_co2_data)
	{
		return NULL;
	}
	// Set the default valuew
	new_co2_data->co2_Data=co2_data;

}

// Method that creates task
void co2_task_create(UBaseType_t task_priority)
{
	xTaskCreate(
	CO2_Sensor_Task // Task method
	,  "CO2 Task" // Name of task
	,  configMINIMAL_STACK_SIZE // Stack size
	,  NULL
	,  task_priority // Priority of task
	,  NULL );
}

// Callback method that rewrite the CO2 Value with the actual one
void myCo2CallBack(uint16_t *data){
	// Rewrite the value
	co2_data = data;
	// Set bits in event group
	xEventGroupSetBits(_dataReadyEventGroup,CO2_READY_BIT);
};

// Method to get current CO2 Data
uint16_t get_CO2_data(){
	return (uint16_t)co2_data;
}


void CO2_taskRun() {
	EventBits_t event_measure;
	event_measure = xEventGroupWaitBits(
	_meassureEventGroup,
	CO2_BIT,
	pdTRUE,
	pdTRUE,
	portMAX_DELAY);
	if ((event_measure & CO2_BIT)==CO2_BIT)
	{
		// Delay
		vTaskDelay(pdMS_TO_TICKS(100UL));
		// Take Measuring
		rc = mh_z19_takeMeassuring();
		
		// In cas ethe measuring failed
		if (rc != MHZ19_OK)
		{
			// Print for troubleshooting
			printf("CO2 task got wrong");
		}
	}		
}


void CO2_Sensor_Task(void *pvParameters){
	(void)pvParameters;
	// Injecting created callback method
	mh_z19_injectCallBack(myCo2CallBack);
	
	// Infinite loop to run the task to measure CO2
	for(;;)
	{
		//Task method to measure CO2
		CO2_taskRun();
	}
}
