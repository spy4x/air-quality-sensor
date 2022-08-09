#include "pm.h"
#include <PMS.h>

#if defined(ESP32)
HardwareSerial pmSensorSerial = PMS5003_SERIAL;
#else
SoftwareSerial pmSensorSerial(PMS5003_RX, PMS5003_TX);
#endif
PMS pmSensor(pmSensorSerial);
PMS::DATA pmData;

void pmSetup() {
  pmSensorSerial.begin(PMS::BAUD_RATE);
  delay(1000);
  pmSensor.passiveMode();
  pmSensor.wakeUp();
}

void pmSleep() { pmSensor.sleep(); }
void pmWakeUp() { pmSensor.wakeUp(); }

void pmRead() {
#if !defined(ESP32)
  pmSensorSerial.listen();
#endif
  while (pmSensorSerial.available()) {
    // Clear buffer (removes potentially old data) before read. Some data
    // could have been also sent before switching to passive mode.
    pmSensorSerial.read();
  }
  pmSensor.requestRead();

  if (!pmSensor.readUntil(pmData)) {
    log("pmSensor: No data. Check your circuitry.");
  }
}

uint16_t pmGet1p0() { return pmData.PM_SP_UG_1_0; }
uint16_t pmGet2p5() { return pmData.PM_SP_UG_2_5; }
uint16_t pmGet10p0() { return pmData.PM_SP_UG_10_0; }
