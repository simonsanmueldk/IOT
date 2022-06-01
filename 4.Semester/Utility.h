
/*
 * Utility.h
 *
 * Created: 5/24/2022 7:45:09 PM
 *  Author: Dorin Pascal
 */ 

//---HEADER---

// Includes
#include "message_buffer.h"
#include "event_groups.h"
#include <task.h>
#include "semphr.h"

MessageBufferHandle_t xMessageBuffer; // Message Buffer
MessageBufferHandle_t downlinkMessageBuffer; // Downlink Mesage Buffer
SemaphoreHandle_t mutex; // Mutex

// Defines
#define CO2_BIT (1<<1)
#define TEMPERATURE_HUMIDITY_BIT (1 << 0)

// Event groups
EventGroupHandle_t _meassureEventGroup ;
EventGroupHandle_t _dataReadyEventGroup;


//Methods
void create_message_buffer();
void create_event_groups();
void create_mutex();
