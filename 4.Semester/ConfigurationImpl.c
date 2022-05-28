/*
 * Configuration.c
 *
 * Created: 4/28/2022 12:46:56 PM
 *  Author: Dorin Pascal
 */ 

//-----------------------------Includes--------------------------//
#include "Configuration.h"

//---------------------------------------------------------------
 

//-----------------------------Variables---------------------------//

int16_t min_temperature_data;
int16_t max_temperature_data;
uint16_t min_humidity_data;
uint16_t max_humidity_data;


//-----------------------------Methods--------------------------//
void Configuration_create()
{
	mutex = xSemaphoreCreateBinary();
	max_humidity_data=1;
	max_temperature_data=1;
	min_humidity_data=1;
	min_temperature_data=1;

}


void Configuration_SetMinTemperature(int16_t temperature_data)
{
	if( xSemaphoreTake(mutex, portMAX_DELAY)==pdTRUE)
	{
		min_temperature_data=temperature_data;
		printf("CONFIGURATION MIN TEMP-->%i",min_temperature_data);
		xSemaphoreGive(mutex);
	}
	
	
	puts("HAHA");
}
void Configuration_SetMaxTemperature(int16_t temperature_data)
{
	if( xSemaphoreTake(mutex, portMAX_DELAY)==pdTRUE)
	{
		max_temperature_data=temperature_data;
		//printf("CONFIGURATION MAX TEMP-->%i",conf->max_temperature_data);
		xSemaphoreGive(mutex);
	}

	
	puts("HAHA1");

}
void Configuration_SetMinHumidity(uint16_t humidity_data)
{
	if( xSemaphoreTake(mutex, portMAX_DELAY)==pdTRUE)
	{
	min_humidity_data=humidity_data;
	//printf("CONFIGURATION MIN HUM-->%i",conf->min_humidity_data);
	xSemaphoreGive(mutex);
		
	}
	

puts("HAHA2");

}


void Configuration_SetMaxHumidity(uint16_t humidity_data)
{
	if( xSemaphoreTake(mutex, portMAX_DELAY)==pdTRUE)
	{

	max_humidity_data=humidity_data;
	//printf("CONFIGURATION MAX HUM-->%i",conf->max_humidity_data);
	xSemaphoreGive(mutex);
	}


puts("HAHA3");
}

int16_t Configuration_GetMaxTemperature()
{
	
	return max_temperature_data;
	
}
int16_t Configuration_GetMinTemperature()
{

	
	return min_temperature_data;
	
}
uint16_t Configuration_GetMaxHumidity()
{
	
	return max_humidity_data;
	
}

uint16_t Configuration_GetMinHumidity()
{
	printf("GETTING MIN HUMID");
	
	return min_humidity_data;
	
}

//---------------------------------------------------------------
