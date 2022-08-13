// -------------------------------- LIBRARIES
#include "_shared.h"
#include "devices/co2/co2.h"
#include "devices/pm/pm.h"
#include "devices/report/report.h"
#include "devices/tempHumid/tempHumid.h"
#include "devices/wifi/wifi.h"

// -------------------------------- CONFIG
#include "_config.h"

// -------------------------------- VARIABLES & CONSTANTS
SensorsValues values;
TechInfo info;

// -------------------------------- FUNCTION DECLARATIONS
bool loopSensors();

void setup() {
  loggerSetup();
  log("Setup");

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(300);
  digitalWrite(LED_BUILTIN, LOW);

  pmSetup();
  co2Setup();
  tempHumidSetup();
  log("Setup complete");
}

void loop() {
  if (loopSensors()) {
    wifiConnect(true);
    info.wifiIsConnected = wifiIsConnected();
    info.wifiSignal = wifiGetSignal();
    info.wifiSSID = wifiGetSSID();
    info.wifiIP = wifiGetIP();
    report(values, info);
  }
}

bool loopSensors() {
  const unsigned long now = millis();
  const bool didInitialWarmUpTimePassed =
      now > SENSORS_INITIAIL_WARM_UP_DELAY_MS;
  const bool didTimeForWarmUpPassed =
      now - info.lastSensorsReadAt >
      REPORT_INTERVAL_MS - SENSORS_AFTER_SLEEP_WARM_UP_DELAY_MS;
  const bool isTimeToWakeUp =
      !didInitialWarmUpTimePassed || didTimeForWarmUpPassed;
  const bool didTimeForReportPassed =
      now - info.lastSensorsReadAt > REPORT_INTERVAL_MS;
  const bool isTimeToReport =
      didInitialWarmUpTimePassed && didTimeForReportPassed;

  if (SLEEPING == info.sensorsState && isTimeToWakeUp) {
    log("\nWaking up sensors");
    pmWakeUp();
    info.sensorsState = WAKING_UP;
  }

  if (WAKING_UP == info.sensorsState && isTimeToReport) {
    log("Sensors have woken up.");
    info.sensorsState = READY;
  }

  if (READY == info.sensorsState) {
    log("Reading all sensors");
    values.co2 = co2Get();
    values.pm2p5 = pmGet();
    values.temperature = tempGet();
    values.humidity = humidGet();
    info.lastSensorsReadAt = now;
    log("Sending sensors to sleep");
    pmSleep();
    info.sensorsState = SLEEPING;
    return true;
  }

  return false;
}
