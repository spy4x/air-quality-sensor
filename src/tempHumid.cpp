#include "config.h"
#include "shared.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTTYPE DHT22

DHT_Unified dht(PIN_DHT22, DHTTYPE);
uint32_t delayMS;

SensorState tempHumidSensorState = WAKING_UP;
int8_t tempValue = UNSET_INTEGER;
uint8_t humidValue = UNSET_INTEGER;
unsigned long tempHumidStateStartedAt = 0;

void tempHumidReadValue();

void tempHumidSetup() {
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print(F("Sensor Type: "));
  Serial.println(sensor.name);
  Serial.print(F("Driver Ver:  "));
  Serial.println(sensor.version);
  Serial.print(F("Unique ID:   "));
  Serial.println(sensor.sensor_id);
  Serial.print(F("Max Value:   "));
  Serial.print(sensor.max_value);
  Serial.println(F("°C"));
  Serial.print(F("Min Value:   "));
  Serial.print(sensor.min_value);
  Serial.println(F("°C"));
  Serial.print(F("Resolution:  "));
  Serial.print(sensor.resolution);
  Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print(F("Sensor Type: "));
  Serial.println(sensor.name);
  Serial.print(F("Driver Ver:  "));
  Serial.println(sensor.version);
  Serial.print(F("Unique ID:   "));
  Serial.println(sensor.sensor_id);
  Serial.print(F("Max Value:   "));
  Serial.print(sensor.max_value);
  Serial.println(F("%"));
  Serial.print(F("Min Value:   "));
  Serial.print(sensor.min_value);
  Serial.println(F("%"));
  Serial.print(F("Resolution:  "));
  Serial.print(sensor.resolution);
  Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
}

void tempHumidLoop() {

  const long now = millis();

  switch (tempHumidSensorState) {
  case SLEEPING:
    if (now - tempHumidStateStartedAt <
        (REPORT_PERIOD_MS - SENSORS_WARM_UP_DELAY_MS)) {
      return;
    }
    tempHumidSensorState = WAKING_UP;
    tempHumidStateStartedAt = now;
    Serial.println("tempHumidSensor: waking up");
    break;

  case WAKING_UP:
    if (now - tempHumidStateStartedAt < SENSORS_WARM_UP_DELAY_MS) {
      return;
    }
    tempHumidReadValue();
    tempHumidSensorState = READY;
    tempHumidStateStartedAt = now;
    Serial.println("tempHumidSensor: ready");
    break;

  case READY:
    tempHumidSensorState = SLEEPING;
    tempHumidStateStartedAt = now;
    Serial.println("tempHumidSensor: going to sleep");
    break;

  default:
    Serial.print("tempHumidSensor: Unknown state ");
    Serial.println(tempHumidSensorState);
    break;
  }
}

void tempHumidReadValue() {
  sensors_event_t event;
  
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
    tempValue = UNSET_INTEGER;
  } else {
    tempValue = event.temperature;
  }

  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
    humidValue = UNSET_INTEGER;
  } else {
    humidValue = event.relative_humidity;
  }
}

SensorState tempHumidGetState() { return tempHumidSensorState; }

int8_t tempGetValue() { return tempValue; }

uint8_t humidGetValue() { return humidValue; }