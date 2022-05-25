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

extern EventGroupHandle_t _dataReadyEventGroup ;
extern EventGroupHandle_t _meassureEventGroup ;



TickType_t xLastWakeTime;	
TickType_t xFrequency;

mh_z19_returnCode_t rc;
uint16_t co2_data;

typedef struct CO2_Sensor {
	uint16_t co2_Data;
}CO2_Sensor;



void CO2_sensor_create()
{
	co2_sensor_t new_co2_data = pvPortMalloc(sizeof(CO2_Sensor));
	if(NULL == new_co2_data)
	{
		return NULL;
	}
	new_co2_data->co2_Data=co2_data;

}
void co2_task_create(UBaseType_t task_priority)
{
	xTaskCreate(
	CO2_Sensor_Task
	,  "CO2 Task" 
	,  configMINIMAL_STACK_SIZE  
	,  NULL
	,  task_priority 
	,  NULL );
}

void myCo2CallBack(uint16_t *data){
	co2_data = data;
	xEventGroupSetBits(_dataReadyEventGroup,CO2_READY_BIT);
};


uint16_t get_CO2_data(){
	
	return (uint16_t)co2_data;
}


void CO2_taskRun() {
	
	EventBits_t event_measure;
	event_measure = xEventGroupWaitBits(
	_meassureEventGroup,
	BIT_CO2,
	pdTRUE,
	pdTRUE,
	portMAX_DELAY);
	
	if ((event_measure & BIT_CO2)==BIT_CO2)
	{
		vTaskDelay(pdMS_TO_TICKS(100UL));
		rc = mh_z19_takeMeassuring();
		if (rc != MHZ19_OK)
		{
			printf("task got wrong");
		}
		printf("<<CO2 task set>>");		
	}		
}


void CO2_Sensor_Task(void *pvParameters){
	(void)pvParameters;
	mh_z19_injectCallBack(myCo2CallBack);
	for(;;)
	{
		CO2_taskRun();
	}
	
}