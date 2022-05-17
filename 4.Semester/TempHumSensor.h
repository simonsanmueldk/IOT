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



void tempHum_taskRun(void);
void tempHum_getDataFromTempHumSensorTask( void *pvParameters );
void tempHum_init();
typedef struct Temp_Humidity_Sensor* temperature_humdity_sensor_t;

void Temp_Humidty_sensor_create(EventGroupHandle_t event1,EventGroupHandle_t event2);uint16_t get_temperature_data();uint16_t get_humidity_data();
