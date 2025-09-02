// -------------------------------- LIBRARIES
#include "_shared.h"
#include "devices/co2/co2.h"
#include "devices/pm/pm.h"
#include "devices/report/report.h"
#include "devices/tempHumid/tempHumid.h"
#include "devices/wifi/wifi.h"
#include <esp_sleep.h>

// -------------------------------- CONFIG
#include "_config.h"

// -------------------------------- VARIABLES & CONSTANTS
SensorsValues values;
TechInfo info;

// -------------------------------- FUNCTION DECLARATIONS
bool loopSensors();

void setup() {
  setCpuFrequencyMhz(80); // Reduce CPU frequency to save power
  loggerSetup();
  log("\n\n\nSetting sensors up...");
  pmSetup();
  co2Setup();
  tempHumidSetup();
  log("Setup complete");

  log("Warming up sensors (" + String(SENSORS_AFTER_SLEEP_WARM_UP_DELAY_MS / 1000) + " seconds)");
  delay(SENSORS_AFTER_SLEEP_WARM_UP_DELAY_MS);
}

void loop() {
  if (loopSensors()) {
    info.wifiIsConnected = wifiIsConnected();
    info.wifiSignal = wifiGetSignal();
    info.wifiSSID = wifiGetSSID();
    info.wifiIP = wifiGetIP();
    report(values, info);

    // Prepare for deep sleep
    log("Preparing for deep sleep");
    pmSleep();
    // Calculate sleep duration (REPORT_INTERVAL_MS)
    esp_sleep_enable_timer_wakeup(REPORT_INTERVAL_MS * 1000ULL); // ms to us
    log("Entering deep sleep for next reading (" + String(REPORT_INTERVAL_MS / 1000) + " seconds)");
    esp_deep_sleep_start();
    // Device will reset after deep sleep, so no code below will run
  }
}

bool loopSensors() {
  // On wake, always read sensors and report
  log("Reading all sensors");
  values.uptime = millis() / 1000;
  values.co2 = co2Get();
  pmRead();
  values.pm1p0 = pmGet1p0();
  values.pm2p5 = pmGet2p5();
  values.pm10p0 = pmGet10p0();
  values.temperature = tempGet();
  values.humidity = humidGet();
  info.lastSensorsReadAt = millis();
  log("Sensors read complete");
  return true;
}
