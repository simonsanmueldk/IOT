/*
 * Configuration.h
 *
 * Created: 4/28/2022 12:47:27 PM
 *  Author: Dorin Pascal
 */

//---HEADER---

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#pragma once

//Includes
#include <ATMEGA_FreeRTOS.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> 
#include <semphr.h>

// Enum for return code
typedef enum { GOOD,BAD} ConfigurationReturnCode;

//Methods
ConfigurationReturnCode Configuration_destroy();
void Configuration_SetMinTemperature(int16_t temperature_data);
void Configuration_SetMinHumidity(uint16_t humidity_data);
void Configuration_SetMaxTemperature(int16_t temperature_data);
void Configuration_SetMaxHumidity(uint16_t humidity_data);

int16_t Configuration_GetMaxTemperature();
uint16_t Configuration_GetMaxHumidity();
int16_t Configuration_GetMinTemperature();
uint16_t Configuration_GetMinHumidity();
#endif /* CONFIGURATION_H_*/




