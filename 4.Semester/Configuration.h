/*
 * Configuration.h
 *
 * Created: 4/28/2022 12:47:27 PM
 *  Author: Dorin Pascal
 */


#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_



#pragma once
#include <ATMEGA_FreeRTOS.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> 
#include <semphr.h>
typedef enum { OK,FAILED} ConfigurationReturnCode;


SemaphoreHandle_t mutex;


typedef struct Configuration* Configuration_t;

Configuration_t Configuration_create();
ConfigurationReturnCode Configuration_destroy();
ConfigurationReturnCode Configuration_SetMinTemperature(uint16_t temperature_data);
ConfigurationReturnCode Configuration_SetMinHumidity(uint16_t humidity_data);
ConfigurationReturnCode Configuration_SetMinCO2(uint16_t co2_data);
ConfigurationReturnCode Configuration_SetMaxTemperature(uint16_t temperature_data);
ConfigurationReturnCode Configuration_SetMaxHumidity(uint16_t humidity_data);
ConfigurationReturnCode Configuration_SetMaxCO2(uint16_t co2_data);
uint16_t Configuration_GetMaxTemperature();
uint16_t Configuration_GetMaxHumidity();
uint16_t Configuration_GetMinTemperature();
uint16_t Configuration_GetMinHumidity();
#endif /* CONFIGURATION_H_*/




