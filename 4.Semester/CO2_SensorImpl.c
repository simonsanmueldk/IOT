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


EventGroupHandle_t dataReadyEventGroup ;
EventGroupHandle_t meassureEventGroup ;

#define CO2_READY_BIT (1<<0)
#define CO2_BIT (1<<1)

TickType_t xLastWakeTime;	
TickType_t xFrequency;

mh_z19_returnCode_t rc;
uint16_t co2_data;

typedef struct CO2_Sensor {
	uint16_t co2_Data;
}CO2_Sensor;



void CO2_sensor_create(EventGroupHandle_t event1,EventGroupHandle_t event2)
{
	
	co2_sensor_t new_co2_data = pvPortMalloc(sizeof(CO2_Sensor));
	if(NULL == new_co2_data)
	{
		return NULL;
	}
	new_co2_data->co2_Data=co2_data;
	meassureEventGroup=event1;
	dataReadyEventGroup=event2;

}

void myCo2CallBack(uint16_t *data){
	co2_data = data;
	xEventGroupSetBits(dataReadyEventGroup,CO2_READY_BIT);
};


uint16_t get_CO2_data(){
	
	return (uint16_t)co2_data;
}


void CO2_taskRun() {
	
	EventBits_t event_measure;
	event_measure = xEventGroupWaitBits(
	meassureEventGroup,
	CO2_BIT,
	pdTRUE,
	pdTRUE,
	portMAX_DELAY);
	
	if ((event_measure & CO2_BIT)==CO2_BIT)
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
	puts("<<<CO2  1>>>>");
	xFrequency = 3000/portTICK_PERIOD_MS;
	xLastWakeTime = xTaskGetTickCount();
	mh_z19_injectCallBack(myCo2CallBack);
	for(;;)
	{
		
		CO2_taskRun();
	}
	
	
}