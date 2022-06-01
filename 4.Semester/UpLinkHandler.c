#include "ATMEGA_FreeRTOS.h"
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "message_buffer.h"

#include "lora_driver.h"
#include "UpLinkHandler.h"

#include "SensorData.h"

#define LORA_appEUI "49B360EEE16A8D4C"
#define LORA_appKEY "E0597BF885F1F18CF896B91F8E211814"
#include <status_leds.h>
#include "Utility.h"

//Create the payload
static lora_driver_payload_t _uplink_payload;
//Create the message buffer
extern MessageBufferHandle_t xMessageBuffer;


// Start method of the UplinkHandler task
void upLinkHandler_StartTask(){
	for(;;)
	{
		//Infinite loop to send the measured data
		lora_Handler_task();
	}
}

//Method to create an UplinkHandler Task
void upLink_create(UBaseType_t priority)
{
	xTaskCreate(
	upLinkHandler_StartTask, // Task method
	"LoraUpLink", // Name of the task
	configMINIMAL_STACK_SIZE+200, // Stack size
	(void*)xMessageBuffer, // Message buffer
	priority, //Task priority
	NULL );
}

// Calling required LoRa Serup methods
static void uplink_lora_setup(void)
{
	char _out_buf[20];
	lora_driver_returnCode_t rc;
	status_leds_slowBlink(led_ST2); // OPTIONAL: Led the green led blink slowly while we are setting up LoRa

	// Factory reset the transceiver
	printf("FactoryReset >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_rn2483FactoryReset()));
	
	// Configure to EU868 LoRaWAN standards
	printf("Configure to EU868 >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_configureToEu868()));
	puts("Try to join1");
	// Get the transceivers HW EUI
	rc = lora_driver_getRn2483Hweui(_out_buf);
	printf("Get HWEUI >%s<: %s\n",lora_driver_mapReturnCodeToText(rc), _out_buf);

	// Set the HWEUI as DevEUI in the LoRaWAN software stack in the transceiver
	printf("Set DevEUI: %s >%s<\n", _out_buf, lora_driver_mapReturnCodeToText(lora_driver_setDeviceIdentifier(_out_buf)));
	puts("Try to join2");
	// Set Over The Air Activation parameters to be ready to join the LoRaWAN
	printf("Set OTAA Identity appEUI:%s appKEY:%s devEUI:%s >%s<\n", LORA_appEUI, LORA_appKEY, _out_buf, lora_driver_mapReturnCodeToText(lora_driver_setOtaaIdentity(LORA_appEUI,LORA_appKEY,_out_buf)));

	// Save all the MAC settings in the transceiver
	printf("Save mac >%s<\n",lora_driver_mapReturnCodeToText(lora_driver_saveMac()));

	// Enable Adaptive Data Rate
	printf("Set Adaptive Data Rate: ON >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_setAdaptiveDataRate(LORA_ON)));
	puts("Try to join3");
	// Set receiver window1 delay to 500 ms - this is needed if down-link messages will be used
	printf("Set Receiver Delay: %d ms >%s<\n", 500, lora_driver_mapReturnCodeToText(lora_driver_setReceiveDelay(500)));

	// Join the LoRaWAN
	uint8_t maxJoinTriesLeft = 10;
	
	
	
	do {
		rc = lora_driver_join(LORA_OTAA);
		printf("Join Network TriesLeft:%d >%s<\n", maxJoinTriesLeft, lora_driver_mapReturnCodeToText(rc));

		if ( rc != LORA_ACCEPTED)
		{
			// Make the red led pulse to tell something went wrong
			status_leds_longPuls(led_ST1); // OPTIONAL
			// Wait 5 sec and lets try again
			vTaskDelay(pdMS_TO_TICKS(5000UL));
		}
		else
		{
			break;
		}
	} while (--maxJoinTriesLeft);

	if (rc == LORA_ACCEPTED)
	{
		// Connected to LoRaWAN :-)
		// Make the green led steady
		status_leds_ledOn(led_ST2); // OPTIONAL
	}
	else
	{
		puts("went wrong");
		// Something went wrong
		// Turn off the green led
		status_leds_ledOff(led_ST2); // OPTIONAL
		// Make the red led blink fast to tell something went wrong
		status_leds_fastBlink(led_ST1); // OPTIONAL

		// Lets stay here
		while (1)
		{
			taskYIELD();
		}
	}
}


void lora_Handler_task()
{
	// Hardware reset of LoRaWAN
	lora_driver_resetRn2483(1);
	vTaskDelay(2UL);
	lora_driver_resetRn2483(0);
	
	// Delay so the device has a time to wake up
	vTaskDelay(150UL);

	
	lora_driver_flushBuffers(); // Getting rid of first version after reset!
	uplink_lora_setup(); // Setting up the LoraWan
	size_t xBytesSent;
	
	for(;;)
	{
		// Method to send the bytes through LoRaWan
		send(xBytesSent);
	}
}

void send(size_t xBytesSent){
	
	//Set recieved message buffer to xBytesSent so they can be send
	xBytesSent = xMessageBufferReceive(
	xMessageBuffer, // Message buffer
	(void*) &_uplink_payload, // Object to be send
	sizeof(_uplink_payload), // Size of object
	portMAX_DELAY);
	
	//Only send if xBytesSent is not empty
	if (xBytesSent>0)
	{
		status_leds_shortPuls(led_ST4);  // Pulse the led for troubleshooting purpose
		
		//Print the return code for troubleshooting purpose and send the data by LoRa Driver
		printf("Upload Message >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_sendUploadMessage(false, &_uplink_payload)));
	}
}




