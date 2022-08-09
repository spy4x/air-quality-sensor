#include "co2.h"
#include "_config.h"
#include <MHZ19.h>

MHZ19 co2Sensor;
#if defined(ESP32)
HardwareSerial co2SensorSerial = MHZ19E_SERIAL;
#else
SoftwareSerial co2SensorSerial(PIN_MHZ19E_RX, PIN_MHZ19E_TX);
#endif

void co2Setup() {
  co2SensorSerial.begin(SENSORS_BAUDRATE);
  delay(1000);
  co2Sensor.begin(co2SensorSerial);
  co2Sensor.autoCalibration();
  co2Sensor.setRange(2000);
}

uint16_t co2Get() {
#if !defined(ESP32)
  co2SensorSerial.listen();
#endif
  return co2Sensor.getCO2();
}
