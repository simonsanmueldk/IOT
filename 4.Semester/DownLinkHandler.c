#include <ATMEGA_FreeRTOS.h>
#include <stddef.h>
#include <stdio.h>
#include "ConfigurationImpl.h"
#include <stdlib.h>

#include <lora_driver.h>
#include <status_leds.h>
#include <message_buffer.h>

static lora_driver_payload_t _downlink_payload;

configuration ConfigurationImpl;


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
	
	xMessageBufferReceive(messageBuffer,(void*) &_downlink_payload,sizeof(lora_driver_payload_t),portMAX_DELAY);
	
			if (_downlink_payload.len == 6)
			{
				Configuration_SetMinTemperature(configuration,_downlink_payload.bytes[0]);
				Configuration_SetMinHumidity(configuration,_downlink_payload.bytes[1]);
				Configuration_SetMinCO2(configuration,_downlink_payload.bytes[2]);
				Configuration_SetMaxTemperature(configuration,_downlink_payload.bytes[3]);
				Configration_SetMaxHumidity(configuration,_downlink_payload.bytes[4]);
				Configuration_SetMaxCO2(configuration,_downlink_payload.bytes[5]);
			
			}
			else
			{
				// Retry in 2.5 minutes
				vTaskDelay(pdMS_TO_TICKS(150000));
			}
			
		}
	}
}