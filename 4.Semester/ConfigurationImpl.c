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
	uint16_t min_co2_data;
	uint16_t max_co2_data;
	uint16_t min_temperature_data;
	uint16_t max_temperature_data;
	uint16_t min_humidity_data;
	uint16_t max_humidity_data;
} Configuration;
//---------------------------------------------------------------


//-----------------------------Methods--------------------------//
Configuration_t Configuration_create()
{
	Configuration_t config=pvPortMalloc(sizeof(Configuration));
	if (NULL==config)
	{
		return NULL;
	}
	return config;

}
ConfigurationReturnCode Configuration_destroy(Configuration_t self)
{
	vPortFree(self);
	return OK;
	
}
ConfigurationReturnCode Configuration_SetMinTemperature(Configuration_t conf,uint16_t temperature_data)
{
	conf->min_temperature_data=temperature_data;
	return OK;

}
ConfigurationReturnCode Configuration_SetMinHumidity(Configuration_t conf,uint16_t humidity_data)
{
	conf->min_humidity_data=humidity_data;
	return OK;

}
ConfigurationReturnCode Configuration_SetMinCO2(Configuration_t conf,uint16_t co2_data)
{
	conf->min_co2_data=co2_data;
	return OK;
}
ConfigurationReturnCode Configuration_SetMaxTemperature(Configuration_t conf,uint16_t temperature_data)
{
	conf->max_temperature_data=temperature_data;
	return OK;
}
ConfigurationReturnCode Configuration_SetMaxHumidity(Configuration_t conf,uint16_t humidity_data)
{
	
conf->max_humidity_data=humidity_data;
return OK;
}
ConfigurationReturnCode Configuration_SetMaxCO2(Configuration_t conf,uint16_t co2_data)
{
	conf->max_co2_data=co2_data;
	return OK;
}
uint16_t Configuration_GetMaxTemperature(Configuration_t conf)
{
	return conf->max_temperature_data;
}
uint16_t Configuration_GetMaxHumidity(Configuration_t conf)
{
	return conf->max_humidity_data;
}
uint16_t Configuration_GetMinTemperature(Configuration_t conf)
{
	return conf->min_temperature_data;
}
uint16_t Configuration_GetMinHumidity(Configuration_t conf)
{
	
	return conf->min_humidity_data;
}

//---------------------------------------------------------------
