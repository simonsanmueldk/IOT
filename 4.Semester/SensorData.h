/*
 * SensorData.h
 *
 * Created: 4/28/2022 3:49:00 PM
 *  Author: Dorin Pascal
 */ 


#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <lora_driver.h>

typedef struct SensorDataPackage* SensorDataPackage_t;

void setCO2Ppm(SensorDataPackage_t self,uint16_t pppm);
void setTemperatureData(SensorDataPackage_t self,uint16_t data);
void setHumidityData(SensorDataPackage_t self,uint16_t data);
lora_driver_payload_t getLoRaPayload(SensorDataPackage_t self,uint8_t portNo);