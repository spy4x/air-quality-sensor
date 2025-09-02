#include "pm.h"
#include <PMS.h>

HardwareSerial pmSensorSerial = PMS5003_SERIAL;
PMS pmSensor(pmSensorSerial);
PMS::DATA pmData;

void pmSetup() {
  pmSensorSerial.begin(PMS::BAUD_RATE);
  delay(1000);
  pmSensor.passiveMode();
}

void pmSleep() { pmSensor.sleep(); }
void pmWakeUp() { pmSensor.wakeUp(); }
void pmRead() {
  while (pmSensorSerial.available()) {
    // Clear buffer (removes potentially old data) before read. Some data
    // could have been also sent before switching to passive mode.
    pmSensorSerial.read();
  }
  pmSensor.requestRead();

  uint16_t pm2p5 = 0;

  if (!pmSensor.readUntil(pmData)) {
    log("pmSensor: No data. Check your circuitry.");
  } else {
    pm2p5 = pmData.PM_SP_UG_2_5;
  }
}

uint16_t pmGet1p0() {
  uint16_t pm1p0 = pmData.PM_SP_UG_1_0;
  log("pm1.0: ", false);
  log(String(pm1p0), false);
  log(" ug/m3");
  return pm1p0;
}

uint16_t pmGet2p5() {
  uint16_t pm2p5 = pmData.PM_SP_UG_2_5;
  log("pm2.5: ", false);
  log(String(pm2p5), false);
  log(" ug/m3");
  if (pm2p5 == 0) {
    return UNSET_INTEGER;
  }
  return pm2p5;
}

uint16_t pmGet10p0() {
  uint16_t pm10p0 = pmData.PM_SP_UG_10_0;
  log("pm10.0: ", false);
  log(String(pm10p0), false);
  log(" ug/m3");
  return pm10p0;
}
