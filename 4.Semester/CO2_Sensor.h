/*
 * CO2_Sensor.h
 *
 * Created: 05/05/2022 12:35:45
 *  Author: Chen
 */ 

#include <stdbool.h>
#include <stdint.h>
#include <ATMEGA_FreeRTOS.h>



typedef enum CO2ReturnCode{
	OK,
	FAILED
}CO2ReturnCode;

void myCo2CallBack(uint16_t *ppm);
uint16_t get_CO2_data();
void CO2_taskRun(void);
void CO2_Sensor_Task();
