/*
 * SensorDataPackageHandler.c
 *
 * Created: 4/28/2022 3:52:25 PM
 *  Author: Dorin Pascal
 */ 
#include "SensorData.h"
static lora_driver_payload_t payload;

//Structure 
typedef struct SensorDataPackage
{
	uint16_t co2Ppm;
	uint16_t temperatureData;
	uint16_t humidityData;
	}SensorDataPackage;

SensorDataPackage_t packet;

//Create method - calling pvPortMalloc()
SensorDataPackage_t SensorDataPackage_create(){
	packet = pvPortMalloc(sizeof(SensorDataPackage));
	if (NULL == packet){
		return NULL;
	}
	return packet;
}

//Set co2Ppm variable inside packet to CO2 value (ppm)
void setCO2Ppm(uint16_t pppm)
{
	packet->co2Ppm=pppm;
}

//Set temperatureData variable inside packet to Temperature value (data)
void setTemperatureData(uint16_t data)
{
packet->temperatureData=data;	
}

//Set humidityData variable inside packet to Humidity value (data)
void setHumidityData(uint16_t data)
{	 
packet->humidityData=data;	
}

//Method to get payload containing all set values
lora_driver_payload_t getLoRaPayload(uint8_t portNo){
	//6 bytes long payload
	payload.len = 6;
	
	//Select a port number
	payload.portNo = portNo;
	
	//Transform the value for each byte
	payload.bytes[0]=(uint8_t)(packet->co2Ppm>>8);
	payload.bytes[1] =(uint8_t) (packet->co2Ppm & 0xFF);
	payload.bytes[2]=(uint8_t)(packet->temperatureData>>8);
	payload.bytes[3] = (uint8_t)(packet->temperatureData &  0xFF);
	payload.bytes[4]=(uint8_t)(packet->humidityData>>8);
	payload.bytes[5] = (uint8_t)(packet->humidityData &  0xFF);
	
	return payload;
	}
