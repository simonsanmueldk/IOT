/*
 * TempHum.h
 *
 * Created: 01/05/2022 21.39.51
 *  Author: Simon
 */ 
#pragma once

#include "ATMEGA_FreeRTOS.h"
#include <stdint.h>

void tempHum_taskRun(void);
void tempHum_getDataFromTempHumSensorTask( void *pvParameters );
void tempHum_init();
