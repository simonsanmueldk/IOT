/*
 * Configuration.h
 *
 * Created: 4/28/2022 12:47:27 PM
 *  Author: Dorin Pascal
 */ 
//-----------------------------Define----------------------------//
#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

//---------------------------------------------------------------
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef enum { OK, FAILED }ConfigurationReturnCode;

typedef struct Configuration* Configuration_t;

Configuration_t Configuration_create();
ConfigurationReturnCode Configuration_destroy();
ConfigurationReturnCode Configuration_SetMinTemperature(Configuration_t conf,uint16_t temperature_data);
ConfigurationReturnCode Configuration_SetMinHumidity(Configuration_t conf,uint16_t humidity_data);
ConfigurationReturnCode Configuration_SetMinCO2(Configuration_t conf,uint16_t co2_data);
ConfigurationReturnCode Configuration_SetMaxTemperature(Configuration_t conf,uint16_t temperature_data);
ConfigurationReturnCode Configuration_SetMaxHumidity(Configuration_t conf,uint16_t humidity_data);
ConfigurationReturnCode Configuration_SetMaxCO2(Configuration_t conf,uint16_t co2_data);
uint16_t Configuration_GetMaxTemperature(Configuration_t conf);
uint16_t Configuration_GetMaxHumidity(Configuration_t conf);
uint16_t Configuration_GetMinTemperature(Configuration_t conf);
uint16_t Configuration_GetMinHumidity(Configuration_t conf);
#endif /* CONFIGURATION_H_*/