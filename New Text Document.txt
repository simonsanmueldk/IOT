/*
 * RadiatorController.c
 *
 * Created: 5/18/2022 12:01:10 PM
 *  Author: Dorin Pascal
 */ 


#include "Configuration.h"
#include "TempHumSensor.h"
#include <rc_servo.h>

uint16_t current_temperature;
uint16_t current_humiditiy;
uint16_t min_temperature;
uint16_t min_humidity;
uint16_t max_temperature;
uint16_t max_humidity;
uint16_t actuator;


void radiator_controller_create(UBaseType_t priority)
{
	actuator=0;
	xTaskCreate(
	Radiator_Task_Run(),
	"RadiatorController",
	configMINIMAL_STACK_SIZE+200,
	NULL,
	priority,
	NULL );
}

void Radiator_Task_Run(void* pvParameters)
{
	(void)pvParameters;
	
	for (;;)
	{
		Radiator_task();
	}
}

void Radiator_task()
{

	current_humiditiy=get_humidity_data();
	current_temperature=get_temperature_data();
	
	min_humidity=Configuration_GetMinHumidity();
	max_humidity=Configuration_GetMaxHumidity();
	min_temperature=Configuration_GetMinTemperature();
	max_temperature=Configuration_GetMaxTemperature();
	
	if ((current_humiditiy<min_humidity ) || (current_temperature<min_temperature ))
	{
		actuator+=100;
		rc_servo_setPosition((uint8_t)0,actuator);
		actuator=0;
	}
	else if ((current_humiditiy>max_humidity) || (current_temperature>max_temperature))
	{
		actuator-=100;
		rc_servo_setPosition((uint8_t)0,actuator);
		actuator=0;
	}
	
}

