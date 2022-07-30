#include "pm.h"
#include <PMS.h>

#if defined(ESP32)
HardwareSerial pmSensorSerial = PMS5003_SERIAL;
#else
SoftwareSerial pmSensorSerial(PMS5003_RX, PMS5003_TX);
#endif
PMS pmSensor(pmSensorSerial);
SensorState pmSensorState = WAKING_UP;
unsigned long pmStateStartedAt = 0;
uint16_t pm2p5Value = UNSET_INTEGER;

void pmReadValue();

void pmSetup() {
  pmSensorSerial.begin(PMS::BAUD_RATE);
  delay(1000);
  pmSensor.passiveMode();
}

void pmLoop() {
  const long now = millis();

  switch (pmSensorState) {
  case SLEEPING:
    if (now - pmStateStartedAt <
        (REPORT_PERIOD_MS - SENSORS_WARM_UP_DELAY_MS)) {
      return;
    }
    pmSensor.wakeUp();
    pmSensorState = WAKING_UP;
    pmStateStartedAt = now;
    Serial.println("pmSensor: waking up");
    break;

  case WAKING_UP:
    if (now - pmStateStartedAt < SENSORS_WARM_UP_DELAY_MS) {
      return;
    }
    pmReadValue();
    pmSensorState = READY;
    pmStateStartedAt = now;
    Serial.println("pmSensor: ready");
    break;

  case READY:
    pmSensor.sleep();
    pmSensorState = SLEEPING;
    pmStateStartedAt = now;
    Serial.println("pmSensor: going to sleep");
    break;

  default:
    Serial.print("pmSensor: Unknown state ");
    Serial.println(pmSensorState);
    break;
  }
}

void pmReadValue() {
#if !defined(ESP32)
  pmSensorSerial.listen();
#endif
  while (pmSensorSerial.available()) {
    // Clear buffer (removes potentially old data) before read. Some data
    // could have been also sent before switching to passive mode.
    pmSensorSerial.read();
  }
  pmSensor.requestRead();

  PMS::DATA data;
  if (pmSensor.readUntil(data)) {
    pm2p5Value = data.PM_AE_UG_2_5;
  } else {
    Serial.println("pmSensor: No data. Check your circuitry.");
  }
}

uint16_t pmGetValue() { return pm2p5Value; }
SensorState pmGetState() { return pmSensorState; }
