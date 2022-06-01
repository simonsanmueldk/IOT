#pragma once

//---HEADER---

// Includes
#include "ATMEGA_FreeRTOS.h"
#include "portmacro.h"
#include "message_buffer.h"

// Methods
void upLink_create(UBaseType_t priority);
void lora_handler_task();
void upLinkHandler_StartTask();
