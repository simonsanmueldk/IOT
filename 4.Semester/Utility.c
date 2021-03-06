
/*
 * Utility.c
 *
 * Created: 5/24/2022 7:45:26 PM
 *  Author: Dorin Pascal
 */ 
 
 #include "ATMEGA_FreeRTOS.h"
 #include "Utility.h"
 #include <lora_driver.h>
 const size_t xMessageBufferSizeBytes = 100;
 
// Method to create a message buffer
void create_message_buffer()
{
	xMessageBuffer = xMessageBufferCreate( xMessageBufferSizeBytes );
	downlinkMessageBuffer = xMessageBufferCreate(sizeof(lora_driver_payload_t)*2);;
}
 
// Method to create mutex
void create_mutex()
{
	mutex = xSemaphoreCreateMutex();
}
 
// Method to create event groups
void create_event_groups()
{
	_meassureEventGroup=xEventGroupCreate();
	_dataReadyEventGroup=xEventGroupCreate();
}
