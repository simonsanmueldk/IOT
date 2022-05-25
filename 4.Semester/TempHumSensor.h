/*
 * TempHum.h
 *
 * Created: 01/05/2022 21.39.51
 *  Author: Simon
 */ 
#pragma once

#include "event_groups.h"
#include <stdint.h>
#include <stdio.h>



void tempHum_taskCreate(UBaseType_t task_priority);
void tempHum_Task( void *pvParameters );
void tempHum_Run();
typedef struct Temp_Humidity_Sensor* temperature_humdity_sensor_t;

void Temp_Humidty_sensor_create();uint16_t get_temperature_data();uint16_t get_humidity_data();
