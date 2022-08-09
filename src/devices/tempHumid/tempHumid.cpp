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
void tempHumidRead() {
  dht.temperature().getEvent(&temperatureEvent);
  dht.humidity().getEvent(&humidityEvent);
}

float tempGet() {
  if (isnan(temperatureEvent.temperature)) {
    log(F("Error reading temperature!"));
    return UNSET_INTEGER;
  } else {
    return temperatureEvent.temperature;
  }
}

float humidGet() {
  if (isnan(humidityEvent.relative_humidity)) {
    log(F("Error reading humidity!"));
    return 0;
  } else {
    return humidityEvent.relative_humidity;
  }
}