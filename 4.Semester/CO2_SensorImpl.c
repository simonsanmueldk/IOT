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


EventGroupHandle_t dataReadyEventGroup = xEventGroupCreate;
EventGroupHandle_t meassureEventGroup = xEventGroupCreate;

#define CO2_READY_BIT (1<<0)
#define CO2_BIT (1<<1)

TickType_t xLastWakeTime;	
TickType_t xFrequency;

mh_z19_returnCode_t rc;
uint16_t *ppm;
uint16_t *co2_data;

void myCo2CallBack(uint16_t *ppm){
	co2_data = ppm;
	//printf(" CO2 data %d",co2_data);
};


uint16_t get_CO2_data(){
	
	printf(" CO2 data %d",co2_data);
	return co2_data;
}


void CO2_taskRun() {
	EventBits_t event;
	event = xEventGroupWaitBits(
	meassureEventGroup,
	CO2_BIT,
	pdTRUE,
	pdFALSE,
	portMAX_DELAY);
	
	xTaskDelayUntil( &xLastWakeTime, xFrequency );
	
	rc = mh_z19_takeMeassuring();
	
	if (rc != MHZ19_OK)
	{
		printf("task got wrong");
	}
	
	vTaskDelay(pdMS_TO_TICKS(500UL));
	
	puts("Hello");
	
	
	mh_z19_returnCode_t tc = mh_z19_getCo2Ppm(ppm);
	
	printf("Value: %d",ppm);
	
	if (tc != MHZ19_OK)
	{
		printf("cant get co2 data");
	}
	
	
	//myCo2CallBack(ppm);
	mh_z19_injectCallBack(myCo2CallBack);
	puts("CALLLBACKFINISHED");
	
	get_CO2_data();

	
}


void CO2_Sensor_Task(void *pvParameters){
	puts("CO2");
	xEventGroupSetBits(dataReadyEventGroup, CO2_READY_BIT);
	
	xFrequency = 3000/portTICK_PERIOD_MS;
	
	xLastWakeTime = xTaskGetTickCount();
	puts("CO2 Second");

	for(;;)
	{
		
		CO2_taskRun();
	}
	
	
}