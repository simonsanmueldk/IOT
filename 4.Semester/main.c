/*
* main.c
* Author : Dorin Pascal
*
* Example main file including LoRaWAN setup
* 
*/

#include <stdio.h>
#include <avr/io.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "SensorData.h"
#include <stdio_driver.h>
#include <serial.h>
#include <mh_z19.h>
#include <event_groups.h>


#include <hih8120.h>

 // Needed for LoRaWAN
#include <lora_driver.h>
#include <status_leds.h>
#include "TempHumSensor.h"
#include "message_buffer.h"
#include "UpLinkHandler.h"
#include "DownLinkHandler.h"
#include "CO2_Sensor.h"
#include <rc_servo.h>
#include "ConditionerController.h"
#include "Utility.h"

extern MessageBufferHandle_t downlinkMessageBuffer;

void create_tasks()
{
	co2_task_create(1);
	
	tempHum_taskCreate(1);
	
	application_task_create(3);
	
	//ToDo maybe to move inside the application task
	SensorDataPackage_create();
	
	// Create UpLinkTask and start it up with priority 4 and setup the LoRaWan
	upLink_create(4);
	
	// Create DownlinkTask and start it up with priority 5
	lora_DownLinkHandler_create(5);
	
	//conditioner_controller_create(2);
	
}

void drivers_initialisation()
{
	  hih8120_initialise();
	  mh_z19_initialise(ser_USART3);
	  rc_servo_initialise();
}

void createUtility()
{
	create_event_groups();
	create_message_buffer();
}

void initialiseSystem()
{
	// Set output ports for leds used in the example
	DDRA |= _BV(DDA0) | _BV(DDA7);
	// Make it possible to use stdio on COM port 0 (USB) on Arduino board - Setting 57600,8,N,1
	stdio_initialise(ser_USART0);
	
	drivers_initialisation();
	createUtility();
	create_tasks();

	status_leds_initialise(5); // Priority 5 for internal task
	// Initialise the LoRaWAN driver without down-link buffer
	lora_driver_initialise(ser_USART1, downlinkMessageBuffer);
		
}

/*-----------------------------------------------------------*/
int main(void)
{
	initialiseSystem(); // Must be done as the very first thing!!
	//printf("Program Started!!\n");
	printf("Program started \n");
	vTaskStartScheduler(); // Initialise and run the freeRTOS scheduler. Execution should never return from here.
	/* Replace with your application code */
	
}

