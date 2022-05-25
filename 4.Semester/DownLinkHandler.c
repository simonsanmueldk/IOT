
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

static lora_driver_payload_t _downlink_payload;
extern  MessageBufferHandle_t downlinkMessageBuffer;
uint16_t minHumidity;
int16_t minTemperature;
uint16_t maxHumidity;
int16_t maxTemperature;



void lora_DownLinkHandler_startTask(void* msbuffer){
	for(;;)
	{
		lora_DownLinkHandler_task();
	}
}

void lora_DownLinkHandler_create(UBaseType_t priority)
{
	xTaskCreate(
	lora_DownLinkHandler_startTask,
	"LoraWanDownlink",
	configMINIMAL_STACK_SIZE+200,
	(void*)downlinkMessageBuffer,
	priority,
	NULL );
}

void lora_DownLinkHandler_task()
{     
	size_t xReceivedBytes;
	xReceivedBytes=xMessageBufferReceive(
	downlinkMessageBuffer,
	&_downlink_payload,
	sizeof(lora_driver_payload_t),
	portMAX_DELAY);

	if (xReceivedBytes>0)
	{	
		printf("DOWN LINK: from port: %d with %d bytes received!\n", _downlink_payload.portNo, _downlink_payload.len);
		
			if (8==_downlink_payload.len )
			{
				
				 minHumidity = (_downlink_payload.bytes[0] << 8) + _downlink_payload.bytes[1];
				 minTemperature = (_downlink_payload.bytes[2] << 8) + _downlink_payload.bytes[3];
				 maxHumidity = (_downlink_payload.bytes[4] << 8) + _downlink_payload.bytes[5];
				 maxTemperature = (_downlink_payload.bytes[6] << 8) + _downlink_payload.bytes[7];
				Configuration_SetMinTemperature(minTemperature);
				Configuration_SetMinHumidity(minHumidity);
				Configuration_SetMaxTemperature(maxTemperature);
				Configuration_SetMaxHumidity(maxHumidity);

			}
			else
			{
				// Retry in 2.5 minutes
				vTaskDelay(pdMS_TO_TICKS(150000)); 			}
			
			puts("finished");
	}
	
			
	
		

}