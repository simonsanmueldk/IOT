/*
 * CO2_Sensor.h
 *
 * Created: 05/05/2022 12:35:45
 *  Author: Chen
 */ 
#pragma once

#include "ATMEGA_FreeRTOS.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <hih8120.h>
#include <ATMEGA_FreeRTOS.h>
#include <avr/io.h>
#include "event_groups.h"

typedef enum CO2ReturnCode{
	OK,
	FAILED
}CO2ReturnCode;
typedef struct CO2_Sensor* co2_sensor_t;

void CO2_sensor_create(EventGroupHandle_t event1,EventGroupHandle_t event2);
void myCo2CallBack(uint16_t *ppm);
uint16_t get_CO2_data();
void CO2_taskRun(void);
void CO2_Sensor_Task(void *pvParameters);
