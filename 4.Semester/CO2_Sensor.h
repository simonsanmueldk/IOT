/*
 * CO2_Sensor.h
 *
 * Created: 05/05/2022 12:35:45
 *  Author: Chen
 */ 

//---HEADER---
#pragma once

// Includes
#include "ATMEGA_FreeRTOS.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <hih8120.h>
#include <ATMEGA_FreeRTOS.h>
#include <avr/io.h>
#include "event_groups.h"

// Structure
typedef struct CO2_Sensor* co2_sensor_t;

// Methods
void CO2_sensor_create();
void myCo2CallBack(uint16_t *ppm);
void co2_task_create(UBaseType_t task_priority);
uint16_t get_CO2_data();
void CO2_taskRun(void);
void CO2_Sensor_Task(void *pvParameters);
