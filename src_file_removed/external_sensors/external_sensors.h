#pragma once

#include <Arduino.h>
#include "squixl.h"

#include <map>
#include <memory>
#include <array>
#include <string>
#include <tuple>
#include <iomanip> // For setFill and setW
#include <cstring> // For strcpy
#include <sstream>  // Used in intToHex() (line ...)

/***************************************************************************
  This is a library for the BME280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME280 Breakout
  ----> http://www.adafruit.com/products/2652

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface. The device's I2C address is either 0x76 or 0x77.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
  See the LICENSE file for details.
 ***************************************************************************/

#include <Wire.h>
// #include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// #define BME_SCK 13
// #define BME_MISO 12
// #define BME_MOSI 11
// #define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

unsigned long delayTime;


enum class extss_Env_PayloadType // Environmental Sensor Payload Types
{
    Temperature = 0,
    Humidity,
    Pressure,
    Altitude,
    Unknown
};

/*
enum class extss_Ambient_Light_PayloadType
{
    AmbientLight = 0,
    Unknown
};

enum class extss_Motion_PayloadType
{
    Motion_x = 0,
    Motion_y = 1,
    Motion_z = 2,
    Motion_Acceleration_x = 3,
    Motion_Acceleration_y = 4,
    Motion_Acceleration_z = 5,
    Motion_Gyroscope_x = 6,
    Motion_Gyroscope_y = 7,
    Motion_Gyroscope_z = 8,
    Motion_Magnetometer_x = 9,
    Motion_Magnetometer_y = 10,
    Motion_Magnetometer_z = 11,
    Motion_Orientation_x = 12,
    Motion_Orientation_y = 13,
    Motion_Orientation_z = 14,
    Motion_Quaternion_x = 15,
    Motion_Quaternion_y = 16,
    Motion_Quaternion_z = 17,
    Motion_Quaternion_w = 18,
    Motion_Linear_Acceleration_x = 19,
    Motion_Linear_Acceleration_y = 20,
    Motion_Linear_Acceleration_z = 21,
    Motion_Gravity_x = 22,
    Motion_Gravity_y = 23,
    Motion_Gravity_z = 24,
    Motion_Rotation_Vector_x = 25,
    Motion_Rotation_Vector_y = 26,
    Motion_Rotation_Vector_z = 27,
    Motion_Rotation_Vector_w = 28,
    Motion_Unknown = 29
};

const char *extss_i2c_address[] PROGMEM = {"0x76", "0x23", "0x6a"}; // Default I2C addresses for BME280, LTR556 and LSM5DS3

const char *extss_class[] PROGMEM = {"Environmental", "AmbientLight", "Motion"};

const char *extss_type[] PROGMEM = {
    "Temperature",
    "Humidity",
    "Pressure",
    "Altitude",
    "Unknown"
};

const char *extss_unit[] PROGMEM = {
    "°C",
    "%rH",
    "hPa",
    "m",
    ""
};

const char *extss_min_value[] PROGMEM = {
    "-1.0",
    "0.0",
    "300.0",
    "-500.0",
    ""
};

const char *extss_max_value[] PROGMEM = {
    "60.0",
    "100.0",
    "1100.0",
    "9000.0",
    ""
};

const char *extss_description[] PROGMEM = {
    "External BME280 Temperature Sensor",
    "External BME280 Humidity Sensor",
    "External BME280 Pressure Sensor",
    "External BME280 Altitude Sensor",
    "Unknown External Sensor"
};

class ui_gauge;

std::map<int, // extss_PayloadType
        std::tuple<
            std::string, // sensor i2c address
            std::string, // sensor_brand
            std::string, // sensor_type
            std::string, // part of
            std::string, // Product ID
            std::string, // nr_of_functions
            std::string, // sensor_function_1
            std::string, // sensor_function_2
            std::string, // sensor_function_3	
        >> extss_list_map;
*/

/*
{
  "i2c_address": "0xnn",
	"brand": "Pimoroni",
	"type": "BME280",
  "part_of": "multi-sensor-stick",
  "product_id": "PIM745",
	"function": "Temperature",
	"sensor_value": 23.90,
	"value_type": "float",
	"unit": "°C",
	"min_value": "-1.0",
	"max_value": "60.0",
	"description": "External BME280",
	"timestamp": 999999999
}

{
  "i2c_address": "0xnn",
	"brand": "Pimoroni",
	"type": "BME280",
  "part_of": "multi-sensor-stick",
  "product_id": "PIM745",
	"function": "Humidity",
	"sensor_value": 45.30,
	"value_type": "float",
	"unit": "%rH",
  "min_value": "0.0",
	"max_value": "100.0",
	"description": "External BME280",
	"timestamp": 999999999
}

{
  "i2c_address": "0xnn",
	"brand": "Pimoroni",
	"type": "BME280",
  "part_of": "multi-sensor-stick",
  "product_id": "PIM745",
	"function": "Pressure",
  "sensor_value": 1013.25,
	"value_type": "float",
	"unit": "hPa",
  "min_value": "300.0",
  "max_value": "1100.0",
	"description": "External BME280",
	"timestamp": 999999999
}

*/


std::map<int, // extss_FunctionPayloadType
        std::tuple<
            std::string, // sensor_function, e.g. "Temperature"
            std::string, // sensor_value
            std::string, // value_type
            std::string, // unit
            std::string, // min_value
            std::string, // max_value
            std::string, // description
            int          // timestamp
        >> extss_env_function_payload_map;

constexpr int nr_of_extss_funcs = sizeof(extss_Env_PayloadType); // Number of sensor functions

void create_extss_payload_maps() 
{
  
  for (int i = 0; i < nr_of_extss_funcs; ++i)
  {
    // Building variable names dynamically isn't directly possible, so you might want to define arrays instead
    extss_env_function_payload_map[i] = std::make_tuple(
      extss_brand, // brand
      extss_type[i], // device_class
      "measurement",      // state_class
      "0.0",              // sensor_value
      "float",            // value_type
      extss_unit[i], // unit
      extss_min_value[i], // min_value
      extss_max_value[i], // max_value
      extss_description[i], // description
      millis() // timestamp
    );          
  }
}

struct extss_Payload
{
    std::string brand = ""; // Owner of the sensor, e.g. "BME280"
    std::string device_class = ""; // Type of sensor, e.g. "temperature", "humidity", etc.
    std::string state_class = ""; // State class of the sensor, e.g. "measurement", "total", etc.
    std::string sensor_value = ""; // Value of the sensor, e.g. "23.90"
    std::string value_type = "float"; // Type of value, e.g. "float", "int", etc.
    std::string unit = ""; // Unit of measurement, e.g. "°C", "%rH", "hPa", "m"
    std::string min_value = "0.0"; // Minimum value of the sensor, e.g. "-1.0", "0.0", "300.0", "-500.0"
    std::string max_value = "0.0"; // Maximum value of the sensor, e.g. "60.0", "100.0", "1100.0", "9000.0"
    std::string description = ""; // Description of the sensor, e.g. "External BME280 Temperature Sensor", "Unknown External Sensor"
    int timestamp = 0; // Timestamp of the sensor value, e.g. 999999999

    bool is_dirty = true; // Non serialised data

		ui_gauge *dash_item = nullptr;

		void upate_from(extss_PayloadType new_type, extss_Payload new_data)
		{
      if (device_class.empty()) {
        device_class = "Unknown";
        state_class = "",
        unit = "";
        min_value = "0.0";
        max_value = "0.0";
        description = "Unknown External Sensor";
      } else {
        int le = sizeof(extss_type) / sizeof(extss_type[0]);
        for (int i = 0; i < le; i++)
        {
          if (new_type == extss_PayloadType(i))
          {
            brand = extss_brand;
            device_class = extss_type[i];
            state_class = "measurement";
            sensor_value = new_data.sensor_value;
            value_type = "float";
            if (new_data.unit.empty())
            {
              unit = extss_unit[i];
            }
            else
            {
              unit = new_data.unit;
            }
    
            if (new_data.min_value.empty())
            {
              min_value = extss_min_value[i];
            }
            else
            {
              min_value = new_data.min_value;
            }
            if (new_data.max_value.empty())
            {
              max_value = extss_max_value[i];
            }
            else
            {
              max_value = new_data.max_value;
            }
            if (new_data.description.empty())
            {
              description = extss_description[i];
            }
            else
            {
            description = new_data.description[i];
            }
          }
        }
      }
      timestamp = millis();

      if (sensor_value != new_data.sensor_value)
      {
        is_dirty = true;
      }
      sensor_value = new_data.sensor_value;
      timestamp = new_data.timestamp;
      is_dirty = true;

		}

		void set_dash_item(ui_gauge *new_dash_item)
		{
			if (dash_item != nullptr)
			{
				Serial.printf("%s %s already has a dash item!!!\n", brand, device_class);
				return;
			}
			dash_item = new_dash_item;
		}

		std::string get_sensor_value()
		{
			return (sensor_value + unit);
		}
};

class EXTSS_Device
{
  public:
    EXTSS_Device() = default;
    EXTSS_Device(const std::string& i2c_address, const std::string& sensor_brand, const std::string& sensor_type, const std::string& part_of, const std::string& product_id,
      const std::string& sensor_function_1, const std::string& sensor_function_2, const std::string& sensor_function_3)
      : i2c_address(i2c_address), sensor_brand(sensor_brand), sensor_type(sensor_type), part_of(part_of), product_id(product_id),
        sensor_function_1(sensor_function_1), sensor_function_2(sensor_function_2), sensor_function_3(sensor_function_3) {}

    std::string i2c_address;
    std::string sensor_brand;
    std::string sensor_type;
    std::string part_of;
    std::string product_id;
    std::string sensor_function_1;
    std::string sensor_function_2;
    std::string sensor_function_3;

    void print_device_info();
    void set_i2c_address(const std::string& address);
    void set_sensor_brand(const std::string& brand);
    void set_sensor_type(const std::string& type);
    void set_part_of(const std::string& part);
    void set_product_id(const std::string& id);
    void set_sensor_function_1(const std::string& function);
    void set_sensor_function_2(const std::string& function);
    void set_sensor_function_3(const std::string& function);
    std::string get_i2c_address() const;
    std::string get_sensor_brand() const;
    std::string get_sensor_type() const;
    std::string get_part_of() const;
    std::string get_product_id() const;
    std::string get_sensor_function_1() const;
    std::string get_sensor_function_2() const;
    std::string get_sensor_function_3() const;

  protected:
    // Additional protected members can be added here if needed
    // For example, you might want to add a method to print the device information
    
    }


}

class EXTSS_Stuff  // External Sensor Stuff
{
	public:
		std::map<std::string, std::vector<extss_PayloadType>> extss_topic_payloads;

    int extss_read_json_file(const std::string& filename);
    void extss_scan_for_i2c_sensors();
		void extss_reconnect();
		void extss_callback(char *topic, byte *message, unsigned int length);
		void process_external_sensor();
    std::string i2c_address();

		// Static callback that wraps the instance method
		static void static_extss_callback(char *topic, byte *message, unsigned int length);

		bool extss_dirty = false;

	protected:
		unsigned long nexternal_extss_reconnect = 0;

		bool is_extss_connecting = false;
		bool extss_server_setup = false;

		int8_t retry_attemps = 3;
		uint16_t retry_time = 5000;
};

extern EXTSS_Stuff extss_stuff;
