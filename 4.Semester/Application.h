
/*
 * Application.h
 *
 * Created: 5/24/2022 7:14:44 PM
 *  Author: Dorin Pascal
 */ 

//---HEADER---

#pragma once

// Methods
void Application_Task(void* pvParameters);
void application_task_create(UBaseType_t task_priority);
void application_run(TickType_t* xLastWakeTime,TickType_t xFrequency);
