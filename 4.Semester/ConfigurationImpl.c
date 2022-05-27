/*
 * Configuration.c
 *
 * Created: 4/28/2022 12:46:56 PM
 *  Author: Dorin Pascal
 */ 

//-----------------------------Includes--------------------------//
#include "Configuration.h"

//---------------------------------------------------------------
 

//-----------------------------Structs---------------------------//
typedef struct Configuration {
	int16_t min_temperature_data;
	int16_t max_temperature_data;
	uint16_t min_humidity_data;
	uint16_t max_humidity_data;
} Configuration;
//---------------------------------------------------------------
Configuration_t conf;

//-----------------------------Methods--------------------------//
Configuration_t Configuration_create()
{
	 mutex = xSemaphoreCreateMutex();
	 conf=pvPortMalloc(sizeof(Configuration));
	if (NULL==conf)
	{
		return NULL;
	}
	conf->max_humidity_data=0;
	conf->max_temperature_data=0;
	conf->min_humidity_data=0;
	conf->min_temperature_data=0;
	return conf;

}
ConfigurationReturnCode Configuration_destroy(Configuration_t self)
{
	vPortFree(self);
	return GOOD;
}


void Configuration_SetMinTemperature(int16_t temperature_data)
{
	if( xSemaphoreTake(mutex, portMAX_DELAY)==pdTRUE)
	{
		conf->min_temperature_data=temperature_data;
		printf("CONFIGURATION MIN TEMP-->%i",conf->min_temperature_data);
		xSemaphoreGive(mutex);
	}
	
	puts("HAHA");
}
void Configuration_SetMaxTemperature(int16_t temperature_data)
{
	if( xSemaphoreTake(mutex, portMAX_DELAY)==pdTRUE)
	{
		conf->max_temperature_data=temperature_data;
		//printf("CONFIGURATION MAX TEMP-->%i",conf->max_temperature_data);
		xSemaphoreGive(mutex);
	}
	
	puts("HAHA1");

}
void Configuration_SetMinHumidity(uint16_t humidity_data)
{
	if( xSemaphoreTake(mutex, portMAX_DELAY)==pdTRUE)
	{
	conf->min_humidity_data=humidity_data;
	//printf("CONFIGURATION MIN HUM-->%i",conf->min_humidity_data);
		xSemaphoreGive(mutex);
	}

puts("HAHA2");

}


void Configuration_SetMaxHumidity(uint16_t humidity_data)
{
	if( xSemaphoreTake(mutex, portMAX_DELAY)==pdTRUE)
	{

	conf->max_humidity_data=humidity_data;
	//printf("CONFIGURATION MAX HUM-->%i",conf->max_humidity_data);
	xSemaphoreGive(mutex);
	}

puts("HAHA3");
}

int16_t Configuration_GetMaxTemperature()
{
	xSemaphoreTake(mutex, portMAX_DELAY);
	return conf->max_temperature_data;
	xSemaphoreGive(mutex);
}
int16_t Configuration_GetMinTemperature()
{

	xSemaphoreTake(mutex, portMAX_DELAY);
	return conf->min_temperature_data;
	xSemaphoreGive(mutex);
}
uint16_t Configuration_GetMaxHumidity()
{
	xSemaphoreTake(mutex, portMAX_DELAY);
	return conf->max_humidity_data;
	xSemaphoreGive(mutex);
}

uint16_t Configuration_GetMinHumidity()
{
	printf("GETTING MIN HUMID");
	xSemaphoreTake(mutex, portMAX_DELAY);
	return conf->min_humidity_data;
	xSemaphoreGive(mutex);
}

//---------------------------------------------------------------
