
#include <ATMEGA_FreeRTOS.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "Configuration.h"


#include <lora_driver.h>
#include <status_leds.h>

#include "Utility.h"

//Create the payload
static lora_driver_payload_t _downlink_payload;
//Create the message buffer
extern  MessageBufferHandle_t downlinkMessageBuffer;

// Variables for configuration
uint16_t minHumidity;
int16_t minTemperature;
uint16_t maxHumidity;
int16_t maxTemperature;

// Start task method
void lora_DownLinkHandler_startTask(){
	// Infinite loop that call the task method to recieve data
	for(;;)
	{
		// Task method to recieve data
		lora_DownLinkHandler_task();
	}
}

// Create the task method
void lora_DownLinkHandler_create(UBaseType_t priority)
{
	xTaskCreate(
	lora_DownLinkHandler_startTask, // Task method
	"LoraWanDownlink", // Name of the task
	configMINIMAL_STACK_SIZE+200, // Stack size
	(void*)downlinkMessageBuffer, // Message buffer
	priority, // Task priority
	NULL );
}

// Task method to recieve data
void lora_DownLinkHandler_task()
{     
	// Size of recieved bytes
	size_t xReceivedBytes;
	
	// Set xReceivedBytes that are recieved from message buffer
	xReceivedBytes=xMessageBufferReceive(
	downlinkMessageBuffer, // Message buffer
	(void*)&_downlink_payload, // Payload
	sizeof(lora_driver_payload_t), // Size of payload
	portMAX_DELAY);
	
	// If recieved bytes are more than zero (meaning we recieved actual data)
	if (xReceivedBytes>0)
	{	
		//Print at which port how many bytes we have revied. For troubleshooting purpose
		printf("DOWN LINK: from port: %d with %d bytes received!\n", _downlink_payload.portNo, _downlink_payload.len);
		
			// Continue only in case we recieved every needed data
			if (8==_downlink_payload.len )
			{
				// Transform recieved bytes into the unsigned integer of 2 bytes representing Maximum Temperature value
				uint16_t maxTemperature = (_downlink_payload.bytes[0] << 8) + _downlink_payload.bytes[1];
				printf("MAXTemperature %d", maxTemperature);
				
				// Transform recieved bytes into the unsigned integer of 2 bytes representing Minimum Temperature value
				uint16_t minTemperature = (_downlink_payload.bytes[2] << 8) + _downlink_payload.bytes[3];
				printf("MINTemperature %d", minTemperature);
				
				// Transform recieved bytes into the unsigned integer of 2 bytes representing Maximum Humidity value
				uint16_t maxHumidity = (_downlink_payload.bytes[4] << 8) + _downlink_payload.bytes[5];
				printf("MAXHumidity %d", maxHumidity);
				
				// Transform recieved bytes into the unsigned integer of 2 bytes representing Minimum Humidity value
				uint16_t minHumidity = (_downlink_payload.bytes[6] << 8) + _downlink_payload.bytes[7];
				
				// Set the recieved data in the Configuration
				Configuration_SetMinTemperature(minTemperature);
				Configuration_SetMinHumidity(minHumidity);
				Configuration_SetMaxTemperature(maxTemperature);
				Configuration_SetMaxHumidity(maxHumidity);
			}
			else
			{
				// Retry in 2.5 minutes
				vTaskDelay(pdMS_TO_TICKS(150000)); 			}
	}
}
