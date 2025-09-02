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
    log(F("tempGet: UNSET_TEMPERATURE (error)"));
    return UNSET_TEMPERATURE;
  } else {
    log(F("t: "), false);
    log(String(temperatureEvent.temperature), false);
    log(F(" C"));
    return temperatureEvent.temperature;
  }
}

float humidGet() {
  dht.humidity().getEvent(&humidityEvent);
  if (isnan(humidityEvent.relative_humidity)) {
    log(F("humidGet: UNSET_INTEGER (error)"));
  return UNSET_INTEGER;
  } else {
    log(F("h: "), false);
    log(String(humidityEvent.relative_humidity), false);
    log(F(" %"));
    return humidityEvent.relative_humidity;
  }
}