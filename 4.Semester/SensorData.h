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

SensorDataPackage_t SensorDataPackage_create();
void setCO2Ppm(uint16_t pppm);
void setTemperatureData(uint16_t data);
void setHumidityData(uint16_t data);
lora_driver_payload_t getLoRaPayload(uint8_t portNo);