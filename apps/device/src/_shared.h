#ifndef SHARED_H
#define SHARED_H

#include "_config.h"
#include <Arduino.h>

enum SensorsState { SLEEPING, WAKING_UP, READY };

#define UNSET_INTEGER -1
#define UNSET_TEMPERATURE -273

struct SensorsValues {
  int32_t uptime = UNSET_INTEGER;
  int16_t co2 = UNSET_INTEGER;
  int16_t pm1p0 = UNSET_INTEGER;
  int16_t pm2p5 = UNSET_INTEGER;
  int16_t pm10p0 = UNSET_INTEGER;
  float temperature = UNSET_TEMPERATURE;
  float humidity = UNSET_INTEGER;
};

struct TechInfo {
  String version = VERSION;
  bool wifiIsConnected = false;
  int8_t wifiSignal = UNSET_INTEGER;
  String wifiSSID = "undefined";
  String wifiIP = "undefined";
  SensorsState sensorsState = SLEEPING;
  unsigned long lastSensorsReadAt = 0;
};

void loggerSetup();
void log(const char str[], bool shouldEndWithNewLine = true);
void log(String str, bool shouldEndWithNewLine = true);

#endif