#pragma once

#include "ATMEGA_FreeRTOS.h"
#include "portmacro.h"
#include "message_buffer.h"


void upLink_create(UBaseType_t priority, MessageBufferHandle_t mBuffer);
void lora_handler_task(MessageBufferHandle_t messageBuffer);
void upLinkHandler_StartTask(void* mBuffer);