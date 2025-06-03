#include "external_sensors.h"
#include <PubSubClient.h>
#include <nlohmann/json.hpp>

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(EXTSS_Payload, brand, device_class, state_class, unit, sensor_value, value_type, min_value, max_value, description, timestamp);
#include <Wire.h>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
//#include "json.hpp" // Include the nlohmann/json library

using json = nlohmann::json;
//PubSubClient extss_client(espClientEXTSS);
EXTSS_Device extss_device( "0x76", "Bosch", "BME280", "Weather Station", "BME280-12345",
		"Temperature", "Humidity", "Pressure");


		void EXTSS_Device::print_device_info() const
    {
      Serial.printf("I2C Address: %s, Brand: %s, Type: %s, Part of: %s, Product ID: %s, Functions: [%s, %s, %s]\n",
        i2c_address.c_str(), sensor_brand.c_str(), sensor_type.c_str(), part_of.c_str(), product_id.c_str(),
        sensor_function_1.c_str(), sensor_function_2.c_str(), sensor_function_3.c_str());
    }

    void EXTSS_Device::set_i2c_address(const std::string& address)
    {
      i2c_address = address;
    }

    void EXTSS_Device::set_sensor_brand(const std::string& brand)
    {
      sensor_brand = brand;
    }

    void EXTSS_Device::set_sensor_type(const std::string& type)
    {
      sensor_type = type;
    }

    void EXTSS_Device::set_part_of(const std::string& part)
    {
      part_of = part;
    }

    void EXTSS_Device::set_product_id(const std::string& id)
    {
      product_id = id;
    }

    void EXTSS_Device::set_sensor_function_1(const std::string& function)
    {
      sensor_function_1 = function;
    }

    void EXTSS_Device::set_sensor_function_2(const std::string& function)
    {
      sensor_function_2 = function;
    }

    void EXTSS_Device::set_sensor_function_3(const std::string& function)
    {
      sensor_function_3 = function;
    }

    // You can also add methods to get the device information in a formatted way
    std::string EXTSS_Device::get_i2c_address() const
    {
      return i2c_address;
    }

    std::string EXTSS_Device::get_sensor_brand() const
    {
      return sensor_brand;
    }

    std::string EXTSS_Device::get_sensor_type() const
    {
      return sensor_type;
    }

    std::string EXTSS_Device::get_part_of() const
    {
      return part_of;
    }

    std::string EXTSS_Device::get_product_id() const
    {
      return product_id;
    }

    std::string EXTSS_Device::get_sensor_function_1() const
    {
      return sensor_function_1;
    }

    std::string EXTSS_Device::get_sensor_function_2() const
    {
      return sensor_function_2;
    }

    std::string EXTSS_Device::get_sensor_function_3() const
    {
      return sensor_function_3;
		}







int EXTSS_Stuff::extss_read_json_file(const std::string& filename) {

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

const int MAX_DEVICES = 127; // Maximum possible addresses
byte foundDevices[MAX_DEVICES]; // Array to store device addresses
int deviceCount = 0; // Counter for detected devices

void extss_scan_for_i2c_sensors() {
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

void loop() {
		;
}


// Static callback that wraps the instance method
void EXTSS_Stuff::static_extss_callback(char *topic, byte *message, unsigned int length)
{
	extss_stuff.extss_callback(topic, message, length);
}

void EXTSS_Stuff::extss_callback(char *topic, byte *message, unsigned int length)
{
	// Serial.print("Message arrived on topic: ");
	// Serial.print(topic);
	// Serial.print(". Message: ");
	String messageTemp;

	for (int i = 0; i < length; i++)
	{
		// Serial.print((char)message[i]);
		messageTemp += (char)message[i];
	}
	// Serial.println();

	// Serial.println(String(topic).substring(0, 16));

	if (String(topic).substring(0, 16) == "um_space/sensors" || true)
	{
		// Parse the string into a Json structure
		json json_data = json::parse(messageTemp);
		// Convert json to struct
		EXTSS_Payload payload = json_data.get<EXTSS_Payload>();


		bool extss_dirty = false;

		if (extss_topic_payloads.find(payload.brand) == extss_topic_payloads.end())
		{
			extss_topic_payloads[payload.brand].push_back(payload);
			Serial.printf("\nEXTERNAL_SENSOR: New Sensor brand: %s\nAdded new %s sensor at %s\n", 
				payload.brand.c_str(), 
				payload.device_class.c_str(), 
				payload.description.c_str()
			);
			extss_dirty = true;
		}
		else
		{
			for (int s = 0; s < extss_topic_payloads[payload.brand].size(); s++)
			{
				if (extss_topic_payloads[payload.brand][s].device_class == payload.device_class && extss_topic_payloads[payload.brand][s].timestamp < payload.timestamp)
				{
					extss_topic_payloads[payload.brand][s].upate_from(payload);
					Serial.printf("\nEXTERNAL_SENSOR: Updated sensor %s for %s\n", 
						payload.device_class.c_str(), 
						payload.brand.c_str()
					);
					extss_dirty = true;
					break;
				}
			}
		}

		if (!extss_dirty)
		{
			extss_topic_payloads[payload.brand].push_back(payload);
			Serial.printf("\nEXTERNAL_SENSOR: Added new sensor %s for %s at %s\nNow has %d sensors\n", 
				payload.device_class.c_str(), 
				payload.brand.c_str(), 
				payload.description.c_str(), 
				extss_topic_payloads[payload.brand].size()
			);

			extss_dirty = true;
		}
	}
}

void EXTSS_Stuff::extss_reconnect()
{
	bool is_extss_connecting = true;

  bool status;
	
	// default settings
	// status = bme.begin();  
	// You can also pass in a Wire library object like &Wire2
	status = bme.begin(0x76, &Wire);
	if (!status) {
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
		Serial.println("** external_sensor connected **");
		Serial.print("I2C address is ");
		Serial.println(EXTSS_Stuff::i2c_address().c_str());
		Serial.println();
		is_extss_connecting = false;
	}
}

void EXTSS_Stuff::process_external_sensor()
{
	if (millis() - nexternal_extss_reconnect > retry_time && !extss_device.connected())
	{
		nexternal_extss_reconnect = millis();

		if (!is_extss_connecting && retry_attemps > 0)
			extss_reconnect();
	}
	else
	{
		extss_device.loop();
	}
}

std::string EXTSS_Stuff::i2c_address()
{
	return std::to_string(extss_device.i2c_address());
}

EXTSS_Stuff extss_stuff;