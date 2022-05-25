
/*
 * ConditionerController.h
 *
 * Created: 5/19/2022 11:08:44 AM
 *  Author: Dorin Pascal
 */ 

#include "Configuration.h"
#include "TempHumSensor.h"
#include <rc_servo.h>

void conditioner_controller_create(UBaseType_t priority);
void Conditioner_Task_Run(void* pvParameters);
void Conditioner_task();
