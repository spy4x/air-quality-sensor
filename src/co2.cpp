#include "co2.h"
#include <MHZ19.h>

MHZ19 co2Sensor;
#if defined(ESP32)
HardwareSerial co2SensorSerial = MHZ19E_SERIAL;
#else
SoftwareSerial co2SensorSerial(PIN_MHZ19E_RX, PIN_MHZ19E_TX);
#endif
SensorState co2SensorState = WAKING_UP;
uint16_t co2Value = UNSET_INTEGER;
unsigned long co2StateStartedAt = 0;
void co2ReadValue();

void co2Setup() {
  co2SensorSerial.begin(SENSORS_BAUDRATE);
  delay(1000);
  co2Sensor.begin(co2SensorSerial);
  co2Sensor.autoCalibration();
  co2Sensor.setRange(2000);
}

void co2Loop() {
  const long now = millis();

  switch (co2SensorState) {
  case SLEEPING:
    if (now - co2StateStartedAt <
        (REPORT_PERIOD_MS - SENSORS_WARM_UP_DELAY_MS)) {
      return;
    }
    co2SensorState = WAKING_UP;
    co2StateStartedAt = now;
    Serial.println("co2Sensor: waking up");
    break;

  case WAKING_UP:
    if (now - co2StateStartedAt < SENSORS_WARM_UP_DELAY_MS) {
      return;
    }
    co2ReadValue();
    co2SensorState = READY;
    co2StateStartedAt = now;
    Serial.println("co2Sensor: ready");
    break;

  case READY:
    co2SensorState = SLEEPING;
    co2StateStartedAt = now;
    Serial.println("co2Sensor: going to sleep");
    break;

  default:
    Serial.print("co2Sensor: Unknown state ");
    Serial.println(co2SensorState);
    break;
  }
}

void co2ReadValue() {
#if !defined(ESP32)
  co2SensorSerial.listen();
#endif
  co2Value = co2Sensor.getCO2();
}

uint16_t co2GetValue() { return co2Value; }
SensorState co2GetState() { return co2SensorState; }

// class CO2Sensor {
// public:
//   void setup() {
//     serial->begin(SENSORS_BAUDRATE);
//     sensor.begin(serial);
//     sensor.autoCalibration();
//     sensor.setRange(2000);
//   }
//   void loop() {
//     if (millis() < SENSORS_WARM_UP_DELAY_MS) {
//       return;
//     }
//     if (millis() - lastTimeDataWasRead >= INTERVAL) {

//       serial.listen();
//       char buff[50];
//       co2 = sensor.getCO2();
//       sprintf(buff, "CO2: %dppm", co2);
//       Serial.println(buff);

//       lastTimeDataWasRead = millis();
//     }
//   }

// private:
//   MHZ19 sensor;
//   SoftwareSerial serial(PIN_MHZ19E_RX,
//     PIN_MHZ19E_TX);
//   unsigned long lastTimeDataWasRead = 0;
//   uint16_t co2 = 0;
// };