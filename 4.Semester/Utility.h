
/*
 * Utility.h
 *
 * Created: 5/24/2022 7:45:09 PM
 *  Author: Dorin Pascal
 */ 
#include "message_buffer.h"
#include "event_groups.h"
#include <task.h>
#include "semphr.h"

MessageBufferHandle_t xMessageBuffer;
MessageBufferHandle_t downlinkMessageBuffer;
SemaphoreHandle_t mutex;

#define CO2_BIT (1<<1)
#define TEMPERATURE_HUMIDITY_BIT (1 << 0)


EventGroupHandle_t _meassureEventGroup ;
EventGroupHandle_t _dataReadyEventGroup;

void create_message_buffer();
void create_event_groups();
void create_mutex();