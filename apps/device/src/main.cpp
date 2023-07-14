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
  log("\n\n\nSetup");
  pmSetup();
  co2Setup();
  tempHumidSetup();
  wifiConnect();
  log("Setup complete");
}

void loop() {
  if (loopSensors()) {
    // wifiReconnect();
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
    values.uptime = millis() / 1000;
    values.co2 = co2Get();
    pmRead();
    values.pm1p0 = pmGet1p0();
    values.pm2p5 = pmGet2p5();
    values.pm10p0 = pmGet10p0();
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
