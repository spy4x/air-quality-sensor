#include "co2.h"
#include "_config.h"
#include <MHZ19.h>

MHZ19 co2Sensor;
HardwareSerial co2SensorSerial = MHZ19E_SERIAL;

void co2Setup() {
  co2SensorSerial.begin(SENSORS_BAUDRATE);
  delay(1000);
  co2Sensor.begin(co2SensorSerial);
  co2Sensor.autoCalibration();
  co2Sensor.setRange(2000);
}

uint16_t co2Get() {
  const uint16_t co2 = co2Sensor.getCO2();
  log("co2Sensor: ", false);
  log(String(co2), false);
  log(" ppm");
  return co2;
}
