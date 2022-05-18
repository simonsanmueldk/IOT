#include <ATMEGA_FreeRTOS.h>
#include <stddef.h>
#include <stdio.h>
#include "Configuration.h"
#include <stdlib.h>

#include <lora_driver.h>
#include <status_leds.h>
#include <message_buffer.h>

static lora_driver_payload_t _downlink_payload;
Configuration_t ConfigurationImpl;


void lora_DownLinkHandler_startTask(void* messageBuffer){
	for(;;)
	{
		lora_DownLinkHandler_task((MessageBufferHandle_t)messageBuffer);
	}
}

void lora_DownLinkHandler_create(UBaseType_t priority, MessageBufferHandle_t messageBuffer)
{
	xTaskCreate(
	lora_DownLinkHandler_startTask,
	"LoraWanDownlik",
	configMINIMAL_STACK_SIZE+200,
	(void*)messageBuffer,
	priority,
	NULL );
}

void lora_DownLinkHandler_task(MessageBufferHandle_t messageBuffer)
{
<<<<<<< HEAD
=======

>>>>>>> dc7eee9fd033450f69e862f7c94c9c61883246b2
	puts("START");
	xMessageBufferReceive(
	messageBuffer,
	(void*) &_downlink_payload,
	sizeof(lora_driver_payload_t),
	portMAX_DELAY);
			puts("BeforeIF");
			if (_downlink_payload.len == 8)
			{				
				uint16_t minHumidity = (_downlink_payload.bytes[0] << 8) || (_downlink_payload.bytes[1]);
				printf("MINHumidity %d", minHumidity);
				
				uint16_t minTemperature = (_downlink_payload.bytes[2] << 8) || (_downlink_payload.bytes[3]);
				printf("MINTemperature %d", minTemperature);
				
				uint16_t maxHumidity = (_downlink_payload.bytes[4] << 8) || (_downlink_payload.bytes[5]);
				printf("MAXHumidity %d", maxHumidity);
				
				uint16_t maxTemperature = (_downlink_payload.bytes[6] << 8) || (_downlink_payload.bytes[7]);
				printf("MAXTemperature %d", maxTemperature);
				
				Configuration_SetMinTemperature(minTemperature);
				Configuration_SetMinHumidity(minHumidity);
				Configuration_SetMaxTemperature(maxTemperature);
				Configuration_SetMaxHumidity(maxHumidity);
			}
			else
			{
				// Retry in 2.5 minutes
				vTaskDelay(pdMS_TO_TICKS(150000));
			}
			
		

}