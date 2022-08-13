#ifndef SHARED_H
#define SHARED_H

#include "_config.h"
#include <Arduino.h>

enum SensorsState { SLEEPING, WAKING_UP, READY };

#define UNSET_INTEGER 0

struct SensorsValues {
  uint16_t co2 = UNSET_INTEGER;
  uint16_t pm2p5 = UNSET_INTEGER;
  float temperature = UNSET_INTEGER;
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