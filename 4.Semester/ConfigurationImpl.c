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
	uint16_t min_temperature_data;
	uint16_t max_temperature_data;
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
	return OK;
	
}




ConfigurationReturnCode Configuration_SetMinTemperature(uint16_t temperature_data)
{

	 xSemaphoreTake(mutex, portMAX_DELAY);
	conf->min_temperature_data=temperature_data;
	xSemaphoreGive(mutex);
	return OK;

}
ConfigurationReturnCode Configuration_SetMinHumidity(uint16_t humidity_data)
{
	xSemaphoreTake(mutex, portMAX_DELAY);
	conf->min_humidity_data=humidity_data;
	xSemaphoreGive(mutex);
	return OK;

}

ConfigurationReturnCode Configuration_SetMaxTemperature(uint16_t temperature_data)
{
	xSemaphoreTake(mutex, portMAX_DELAY);
	conf->max_temperature_data=temperature_data;
	xSemaphoreGive(mutex);
	return OK;
}
ConfigurationReturnCode Configuration_SetMaxHumidity(uint16_t humidity_data)
{
	xSemaphoreTake(mutex, portMAX_DELAY);
conf->max_humidity_data=humidity_data;
xSemaphoreGive(mutex);
return OK;
}

uint16_t Configuration_GetMaxTemperature()
{
	xSemaphoreTake(mutex, portMAX_DELAY);
	return conf->max_temperature_data;
	xSemaphoreGive(mutex);
}
uint16_t Configuration_GetMaxHumidity()
{
	xSemaphoreTake(mutex, portMAX_DELAY);
	return conf->max_humidity_data;
	xSemaphoreGive(mutex);
}
uint16_t Configuration_GetMinTemperature()
{
	xSemaphoreTake(mutex, portMAX_DELAY);
	return conf->min_temperature_data;
	xSemaphoreGive(mutex); 
}
uint16_t Configuration_GetMinHumidity()
{
	xSemaphoreTake(mutex, portMAX_DELAY);
	return conf->min_humidity_data;
	xSemaphoreGive(mutex);
}

//---------------------------------------------------------------
