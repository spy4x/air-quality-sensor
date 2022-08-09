#ifndef SHARED_H
#define SHARED_H

#include <Arduino.h>

enum SensorsState { SLEEPING, WAKING_UP, READY };
#define UNSET_INTEGER 0
struct SensorsValues {
  uint16_t co2 = UNSET_INTEGER;
  uint16_t pm1p0 = UNSET_INTEGER;
  uint16_t pm2p5 = UNSET_INTEGER;
  uint16_t pm10p0 = UNSET_INTEGER;
  float temperature = UNSET_INTEGER;
  float humidity = UNSET_INTEGER;
  unsigned long uptime = UNSET_INTEGER;
};

void log(const char str[], bool shouldEndWithNewLine = true) {
#if SHOULD_LOG_TO_SERIAL
  if (shouldEndWithNewLine) {
    Serial.println(str);
  } else {
    Serial.print(str);
  }
#endif
}

#endif