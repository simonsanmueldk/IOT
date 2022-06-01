#pragma once

#include "ATMEGA_FreeRTOS.h"
#include "portmacro.h"

void lora_DownLinkHandler_create(UBaseType_t priority);
void lora_DownLinkHandler_task();
void lora_DownLinkHandler_startTask();
