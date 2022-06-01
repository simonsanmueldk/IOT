/*
 * Configuration.c
 *
 * Created: 4/28/2022 12:46:56 PM
 *  Author: Dorin Pascal
 */ 

// Includes
#include "Configuration.h"
#include "Utility.h"

// Variables

int16_t min_temperature_data;
int16_t max_temperature_data;
uint16_t min_humidity_data;
uint16_t max_humidity_data;

// Method to set min temperature
void Configuration_SetMinTemperature(int16_t temperature_data)
{
	// Use the semaphore and mutex
	if( xSemaphoreTake(mutex, pdMS_TO_TICKS(200))==pdTRUE)
	{
		// Rewrite the min_temperature_data with temperature_data
		min_temperature_data=temperature_data;
		// Since its finished xSemaphoreGive()
		xSemaphoreGive(mutex);
	}
}

// Method to set max temperature
void Configuration_SetMaxTemperature(int16_t temperature_data)
{
	// Use the semaphore and mutex
	if( xSemaphoreTake(mutex, portMAX_DELAY)==pdTRUE)
	{
		// Rewrite the max_temperature_data with temperature_data
		max_temperature_data=temperature_data;
		// Since its finished xSemaphoreGive()
		xSemaphoreGive(mutex);
	}
}

// Method to set min humidity
void Configuration_SetMinHumidity(uint16_t humidity_data)
{
	// Use the semaphore and mutex
	if( xSemaphoreTake(mutex, portMAX_DELAY)==pdTRUE)
	{
	// Rewrite the min_humidity_data with humidity_data
	min_humidity_data=humidity_data;
	// Since its finished xSemaphoreGive()
	xSemaphoreGive(mutex);	
	}
}

// Method to set max humidity
void Configuration_SetMaxHumidity(uint16_t humidity_data)
{
	// Use the semaphore and mutex
	if( xSemaphoreTake(mutex, portMAX_DELAY)==pdTRUE)
	{
	// Rewrite the min_humidity_data with humidity_data
	max_humidity_data=humidity_data;
	xSemaphoreGive(mutex);
	}
}

// Method to get max temperature
int16_t Configuration_GetMaxTemperature()
{
	return max_temperature_data;	
}

// Method to get min temperature
int16_t Configuration_GetMinTemperature()
{
	return min_temperature_data;
}

// Method to get max humidity
uint16_t Configuration_GetMaxHumidity()
{
	return max_humidity_data;
}

// Method to get min humidity
uint16_t Configuration_GetMinHumidity()
{	
	return min_humidity_data;
}
