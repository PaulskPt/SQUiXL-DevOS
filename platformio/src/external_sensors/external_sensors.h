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
#include "ui/dashboard/ui_gauge.h"

// #define BME_SCK 13
// #define BME_MISO 12
// #define BME_MOSI 11
// #define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI
Adafruit_BME280 bme; // use I2C interface
Adafruit_Sensor *bme_temperature = bme.getTemperatureSensor();
Adafruit_Sensor *bme_pressure = bme.getPressureSensor();
Adafruit_Sensor *bme_humidity = bme.getHumiditySensor();

unsigned long delayTime;

class ui_gauge;

enum class extss_Env_PayloadType // Environmental Sensor Payload Types
{
    Temperature = 0,
    Humidity,
    Pressure,
    Altitude,
    Unknown
};

/*
{
	"owner": "xxxxxxx",
	"device_class": "temperature",
	"state_class": "measurement",
	"sensor_value": 23.90,
	"value_type": "float",
	"unit_of_measurement": "°C",
	"min_value": "-1.0",
	"max_value": "60.0",
	"description": "Office BME280",
	"timestamp": 999999999
}

*/

struct EXTSS_Payload
{
    std::string owner = "home"; // The owner of the sensor, e.g., "home", "office", etc.
    std::string device_class = ""; // The type of sensor, e.g., "temperature", "humidity", etc.
    std::string state_class = "measurement"; // The state class of the sensor, e.g., "measurement", "total", etc.
    std::string unit_of_measurement = "";
    std::string sensor_value = "";
    std::string value_type = "float";
    std::string min_value = "";
    std::string max_value = "";
    std::string description = "";
    int timestamp;

    /*
    extss_Env_PayloadType payload_temperature_type = extss_Env_PayloadType::Temperature;
    extss_Env_PayloadType payload_humidity_type = extss_Env_PayloadType::Humidity;
    extss_Env_PayloadType payload_pressure_type = extss_Env_PayloadType::Pressure;
    extss_Env_PayloadType payload_altitude_type = extss_Env_PayloadType::Altitude;
    extss_Env_PayloadType payload_unknown_type = extss_Env_PayloadType::Unknown;
    */
  
    // Non serialised data
		bool is_dirty = true;
		ui_gauge *dash_item = nullptr;

		void upate_from(EXTSS_Payload new_data)
		{
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
				Serial.printf("%s %s already has a dash item!!!\n", owner, device_class);
				return;
			}
			dash_item = new_dash_item;
		}

		std::string get_sensor_value()
		{
			return (sensor_value + unit_of_measurement);
		}

};


class EXTSS_Stuff
{
  public:
  	std::map<std::string, std::vector<EXTSS_Payload>> extss_topic_payloads;

    EXTSS_Stuff() = default;
    //EXTSS_Stuff(const std::string& i2c_address, const std::string& sensor_brand, const std::string& sensor_type,
    //            const std::string& part_of, const std::string& product_id,
    //            const std::string& sensor_function_1, const std::string& sensor_function_2,
    //            const std::string& sensor_function_3);
    ~EXTSS_Stuff() = default;
    EXTSS_Stuff(const EXTSS_Stuff&) = delete; // Disable copy constructor
    //EXTSS_Stuff& operator=(const EXTSS_Stuff&) = delete; // Disable copy assignment
    //EXTSS_Stuff(EXTSS_Stuff&&) = default; // Enable move constructor
    //EXTSS_Stuff& operator=(EXTSS_Stuff&&) = default; // Enable move assignment
    std::string get_i2c_address() const;

    void begin();
    void end(); // Added by @PaulskPt
    bool is_dirty() const;
    void set_dirty(bool dirty);
    void reconnect();
    void process();

  private:
    void setup();
    void loop();
    void pr_BME_Values();
    void scan_for_i2c_sensors();
    std::string i2c_address();
    int read_json_file(const std::string& filename);
    void read_sensor_data();

    bool is_extss_connecting = false;
    bool is_connected = false;
    bool dirty = false;
    unsigned long nextss_reconnect = 0;
    unsigned long retry_time = 5000; // 5 seconds
    int retry_attemps = 3; // Number of attempts to reconnect
};

EXTSS_Stuff extss_stuff;
