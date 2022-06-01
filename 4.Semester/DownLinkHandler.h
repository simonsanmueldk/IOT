//---HEADER---

#pragma once

// Includes
#include "ATMEGA_FreeRTOS.h"
#include "portmacro.h"

//Methods
void lora_DownLinkHandler_create(UBaseType_t priority);
void lora_DownLinkHandler_task();
void lora_DownLinkHandler_startTask();
