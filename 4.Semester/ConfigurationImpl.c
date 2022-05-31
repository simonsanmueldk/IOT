/*
 * Configuration.c
 *
 * Created: 4/28/2022 12:46:56 PM
 *  Author: Dorin Pascal
 */ 

//-----------------------------Includes--------------------------//
#include "Configuration.h"
#include "Utility.h"
//---------------------------------------------------------------
 

//-----------------------------Variables---------------------------//

int16_t min_temperature_data;
int16_t max_temperature_data;
uint16_t min_humidity_data;
uint16_t max_humidity_data;


//-----------------------------Methods--------------------------//


void Configuration_SetMinTemperature(int16_t temperature_data)
{
	if( xSemaphoreTake(mutex, pdMS_TO_TICKS(200))==pdTRUE)
	{
		min_temperature_data=temperature_data;
		xSemaphoreGive(mutex);
	}
}
void Configuration_SetMaxTemperature(int16_t temperature_data)
{
	if( xSemaphoreTake(mutex, portMAX_DELAY)==pdTRUE)
	{
		max_temperature_data=temperature_data;
		xSemaphoreGive(mutex);
	}
}
void Configuration_SetMinHumidity(uint16_t humidity_data)
{
	if( xSemaphoreTake(mutex, portMAX_DELAY)==pdTRUE)
	{
	min_humidity_data=humidity_data;
	xSemaphoreGive(mutex);	
	}
}


void Configuration_SetMaxHumidity(uint16_t humidity_data)
{
	if( xSemaphoreTake(mutex, portMAX_DELAY)==pdTRUE)
	{
	max_humidity_data=humidity_data;
	xSemaphoreGive(mutex);
	}
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
	return min_humidity_data;
}

//---------------------------------------------------------------
