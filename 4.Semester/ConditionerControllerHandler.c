/*
 * RadiatorController.c
 *
 * Created: 5/18/2022 12:01:10 PM
 *  Author: Dorin Pascal
 */ 


#include "ConditionerController.h"

// Variables (Current values)
uint16_t current_temperature;
uint16_t current_humiditiy;

// Variables (Configuration values)
uint16_t min_temperature;
uint16_t min_humidity;
uint16_t max_temperature;
uint16_t max_humidity;


// Method to create a task to controll the actuator
void conditioner_controller_create(UBaseType_t priority)
{
	xTaskCreate(
	Conditioner_Task_Run, // Task method
	"ConditionerController", // Name of task
	configMINIMAL_STACK_SIZE+200, // Stack size
	NULL,
	priority, // Task priority
	NULL );
	
}

// Infinite loop that calls method to controlls the actuator
void Conditioner_Task_Run(void* pvParameters)
{	
	(void)pvParameters;
	for (;;)
	{
		// Method to controlls the actuator
		Conditioner_task();
	}
}

void Conditioner_task()
{
	// Wait a few second
	vTaskDelay(6000);
	
	// Set the variables representing curent values with a real data
	current_humiditiy=get_humidity_data();
	current_temperature=get_temperature_data();
	
	// Set the variables representing max/min values with a real data
	min_humidity=Configuration_GetMinHumidity();
	max_humidity=Configuration_GetMaxHumidity();
	min_temperature=Configuration_GetMinTemperature();
	max_temperature=Configuration_GetMaxTemperature();
	
	// Check if the configuration values are set
	uint16_t temp=min_humidity+max_humidity+min_temperature+max_temperature;
	
	// If the configuration is not empty  AND the current humidity is bellow min value OR current temperature is bellow minimum value
	if (temp!=0 && ((current_humiditiy<min_humidity ) || (current_temperature<min_temperature )))
	{
		printf("MOVING the Servo");// Print just for troubleshooting purpose
		// Method to move the actuator
		rc_servo_setPosition((uint8_t)0,100);
	}
	// If the configuration is not empty  AND the current humidity is above maximum value OR current temperature is above maximum value
	else if (temp!=0 && ((current_humiditiy>max_humidity) || (current_temperature>max_temperature)))
	{
		printf("MOVING the Servo"); // Print just for troubleshooting purpose
		// Method to move the actuator to the other side
		rc_servo_setPosition((uint8_t)0,-100);
	}
	else
	{
		// Method to move the actuator to the middle possition
		rc_servo_setPosition((uint8_t)0,0);
	}
}
