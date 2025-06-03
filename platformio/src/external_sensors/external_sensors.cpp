#include "external_sensors.h"
#include <PubSubClient.h>
//#include <nlohmann/json.hpp>

#include <Wire.h>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
/*
#include "json.hpp" // Include the nlohmann/json library

using json = nlohmann::json;

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(EXTSS_Payload, 
	owner,
  device_class, 
  state_class, 
  unit_of_measurement, 
  sensor_value, 
  value_type, 
  min_value, 
  max_value, 
  description, 
  timestamp);


int EXTSS_Stuff::read_json_file(const std::string& filename) {

    std::ifstream file("external_sensors.json"); // Open JSON file
    if (!file) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    json jsonData;
    file >> jsonData; // Parse JSON into jsonData

    std::map<std::string, std::string> myMap;

    // Iterate through JSON object and populate the map
    for (auto& [key, value] : jsonData.items()) {
        myMap[key] = value; // Assuming values are strings, adjust as needed
    }

    // Print the map contents
    for (const auto& [key, value] : myMap) {
        std::cout << key << ": " << value << std::endl;
    }

    return 0;
}
*/
const int MAX_DEVICES = 127; // Maximum possible addresses
byte foundDevices[MAX_DEVICES]; // Array to store device addresses
int deviceCount = 0; // Counter for detected devices

void EXTSS_Stuff::scan_for_i2c_sensors() {
    Serial.println("Scanning I2C bus...");
    for (byte addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0) {
					foundDevices[deviceCount++] = addr; // Store found address
					Serial.print("Device found at address 0x");
					Serial.println(addr, HEX);
        }
    }
		Serial.print("Total devices found: ");
    Serial.println(deviceCount);
    
    // Print the stored addresses
    Serial.println("Stored I2C Addresses:");
    for (int i = 0; i < deviceCount; i++) {
        Serial.print("0x");
        Serial.println(foundDevices[i], HEX);
    }
}

void EXTSS_Stuff::setup()
{
// default settings
    bool status = bme.begin(0x76, &Wire);  
    // You can also pass in a Wire library object like &Wire2
    // status = bme.begin(0x76, &Wire2)
    if (!status) {
        Serial.println(F("Could not find a valid BME280 sensor, check wiring, address, sensor ID!"));
        Serial.print(F("SensorID was: 0x")); Serial.println(bme.sensorID(),16);
        Serial.print(F("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n"));
        Serial.print(F("   ID of 0x56-0x58 represents a BMP 280,\n"));
        Serial.print(F("        ID of 0x60 represents a BME 280.\n"));
        Serial.print(F("        ID of 0x61 represents a BME 680.\n"));
        while (1) delay(10);
    }
		else {
			bme_temperature->printSensorDetails();
  		bme_pressure->printSensorDetails();
		  bme_humidity->printSensorDetails();
		}
    
    delayTime = 5000;

    Serial.println();
}

void EXTSS_Stuff::loop()
{
	pr_BME_Values();
}

void EXTSS_Stuff::pr_BME_Values() {
	// Read the sensor values
  sensors_event_t temp_event, pressure_event, humidity_event;
  bme_temperature->getEvent(&temp_event);
  bme_pressure->getEvent(&pressure_event);
  bme_humidity->getEvent(&humidity_event);
  
  Serial.print(F("Temperature = "));
  Serial.print(temp_event.temperature);
 	Serial.println(F(" °C"));

  Serial.print(F("Humidity = "));
  Serial.print(humidity_event.relative_humidity);
	Serial.println(F(" %rH"));

  Serial.print(F("Pressure = "));
  Serial.print(pressure_event.pressure);
  Serial.println(" hPa");

	Serial.print(F("Approx. Altitude = "));
	Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
	Serial.println(F(" m"));
}

void EXTSS_Stuff::reconnect()
{
	bool is_extss_connecting = true;

  bool status;

	// No need to check for nullptr; just call end() if needed
	bme.end(); // End the previous connection if it exists
	
	Serial.println("Reconnecting to external sensor...");
	// You can also pass in a Wire library object like &Wire2
	status = bme.begin(0x76, &Wire);
	if (!status) {
			is_connected = false;
			Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
			Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
			Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
			Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
			Serial.print("        ID of 0x60 represents a BME 280.\n");
			Serial.print("        ID of 0x61 represents a BME 680.\n");
			is_extss_connecting = true;
			// while (1) delay(10);
	}
	else {
		is_connected = true;
		Serial.println("** external_sensor connected **");
    Serial.print("I2C address is ");
    Serial.println(EXTSS_Stuff::i2c_address().c_str());
    Serial.println();
		is_extss_connecting = false;
	}
}

bool EXTSS_Stuff::is_dirty() const
{
	return this->dirty;
}

void EXTSS_Stuff::set_dirty(bool dirty)
{
	this->dirty = dirty;
}

void EXTSS_Stuff::process()
{
	if (millis() - nextss_reconnect > retry_time && !is_connected)
	{
		nextss_reconnect = millis();

		if (!is_extss_connecting && retry_attemps > 0)
			reconnect();
	}
	else
	{
		loop();
	}
}

std::string EXTSS_Stuff::i2c_address()
{
  if (bme.I2Caddress() != 0) { // Check if the sensor is initialized
    char buf[8];
    snprintf(buf, sizeof(buf), "0x%02X", bme.I2Caddress());
    return std::string(buf); // Use the method to get the I2C address
  } else {
    Serial.println("BME280 sensor not initialized.");
    return "0x00"; // Return a default value if the sensor is not initialized
  }
}

void EXTSS_Stuff::read_sensor_data()
{
		// Read the sensor values
		int nr_events = 3; // Number of sensor events to read
		sensors_event_t temp_event, pressure_event, humidity_event;
		bme_temperature->getEvent(&temp_event);
		bme_pressure->getEvent(&pressure_event);
		bme_humidity->getEvent(&humidity_event);

	 	for (int i = 0; i < nr_events; i++) {
			// Store the values in the payload
			EXTSS_Payload payload;
			if (i == 0) {
				payload.device_class = "temperature";
				payload.sensor_value = String(temp_event.temperature).c_str();
				payload.unit_of_measurement = "°C";
				payload.timestamp = millis() / 1000; // Convert milliseconds to seconds
			} else if (i == 1) {	
				payload.device_class = "humidity";
				payload.sensor_value = String(temp_event.temperature).c_str();
				payload.unit_of_measurement = "%rH";
				payload.timestamp = millis() / 1000; // Convert milliseconds to seconds
			} else if (i == 2) {
				payload.device_class = "pressure";
				payload.sensor_value = String(pressure_event.pressure).c_str();
				payload.unit_of_measurement = "hPa";
				payload.timestamp = millis() / 1000; // Convert milliseconds to seconds
			}
			// Add the payload to the map
			extss_topic_payloads[payload.device_class].push_back(payload);
		}
		// Print the payloads for debugging
		for (const auto& pair : extss_topic_payloads) {
			const std::string& topic = pair.first;
			const std::vector<EXTSS_Payload>& payloads = pair.second;
			Serial.print("Topic: ");
			Serial.println(topic.c_str());
			for (const auto& payload : payloads) {
				Serial.print("  Sensor Value: ");
				Serial.print(payload.sensor_value.c_str());
				Serial.print(" ");
				Serial.println(payload.unit_of_measurement.c_str());
				Serial.print("  Timestamp: ");
				Serial.println(payload.timestamp);
			}
		}
}