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

uint16_t pmGet() {
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
  log("pmSensor: ", false);
  log(String(pm2p5), false);
  log(" ug/m3");
  return pm2p5;
}
