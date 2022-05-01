/*
 * SensorDataPackageHandler.c
 *
 * Created: 4/28/2022 3:52:25 PM
 *  Author: Dorin Pascal
 */ 
#include "SensorData.h"
static lora_driver_payload_t payload;
typedef struct SensorDataPackage
{
	uint16_t co2Ppm;
	uint16_t temperatureData;
	uint16_t humidityData;
	}SensorDataPackage;

void setCO2Ppm(SensorDataPackage_t self,uint16_t pppm)
{
	
	self->co2Ppm=pppm;
}
void setTemperatureData(SensorDataPackage_t self,uint16_t data)
{
self->temperatureData=data;	
}
void setHumidityData(SensorDataPackage_t self,uint16_t data)
{
	
self->humidityData=data;	
}
lora_driver_payload_t getLoRaPayload(SensorDataPackage_t self,uint8_t portNo){

	 payload.len = 20;
		payload.portNo = 2;
	
	payload.bytes[0]=(self->co2Ppm>>8) & 0xFF;
	payload.bytes[1] = (self->co2Ppm >> 0) & 0xFF;
	payload.bytes[3]=(self->temperatureData>>8) & 0xFF;
	payload.bytes[4] = (self->temperatureData >> 0) & 0xFF;
	payload.bytes[5]=(self->humidityData>>8) & 0xFF;
	payload.bytes[6] = (self->humidityData >> 0) & 0xFF;
	
	return payload;
	};
