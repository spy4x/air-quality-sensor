#include "tempHumid.h"
#include "_config.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTTYPE DHT22
DHT_Unified dht(PIN_DHT22, DHTTYPE);
SensorsState tempHumidSensorState = WAKING_UP;
sensors_event_t temperatureEvent;
sensors_event_t humidityEvent;

void tempHumidSetup() { dht.begin(); }

float tempGet() {
  dht.temperature().getEvent(&temperatureEvent);
  if (isnan(temperatureEvent.temperature)) {
    log(F("tempHumidSensor: Error reading temperature!"));
    return UNSET_INTEGER;
  } else {
    log(F("tempHumidSensor: Temperature: "), false);
    log(String(temperatureEvent.temperature), false);
    log(F(" C"));
    return temperatureEvent.temperature;
  }
}

float humidGet() {
  dht.humidity().getEvent(&humidityEvent);
  if (isnan(humidityEvent.relative_humidity)) {
    log(F("tempHumidSensor: Error reading humidity!"));
    return 0;
  } else {
    log(F("tempHumidSensor: Humidity: "), false);
    log(String(humidityEvent.relative_humidity), false);
    log(F(" %"));
    return humidityEvent.relative_humidity;
  }
}