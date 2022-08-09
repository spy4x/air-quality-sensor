// -------------------------------- LIBRARIES
#include "_shared.h"
#include "devices/co2/co2.h"
#include "devices/display/display.h"
#include "devices/pm/pm.h"
#include "devices/report/report.h"
#include "devices/tempHumid/tempHumid.h"
#include "devices/wifi/wifi.h"

// -------------------------------- CONFIG
#include "_config.h"

// -------------------------------- VARIABLES & CONSTANTS
SensorsValues values;
unsigned long lastReportAt = 0;
unsigned long displayIsOnSince = 0;
unsigned long lastDisplayUpdateAt = 0;
SensorsState state = SLEEPING;
bool isSleepModeEnabled = false;
bool prevButtonState = false;
enum DisplayMode { OFF, DATA, TECH_INFO } displayMode = OFF;

// -------------------------------- FUNCTION DECLARATIONS
void displayUpdates();
bool readTouchButton();
void readAllSensors();
void toggleSleepMode();
void toggleDisplayMode();
void sendAllSensorsToSleep();
void wakeUpAllSensors();
void turnOffDisplay();
bool loopSensors();
String getUptimeString();

void setup() {
  Serial.begin(115200);
  if (!displaySetup()) {
    log("Display initiation failed");
    log("Version: ", false);
    log(VERSION);
  }
  pmSetup();
  co2Setup();
  tempHumidSetup();
  wifiConnect();
}

void loop() {
  values.uptime = millis() / 1000;
  bool doesNeedUpdateDisplay = readTouchButton();
  turnOffDisplay();
  if (loopSensors()) {
    doesNeedUpdateDisplay = true;
    reportLoop(values, isSleepModeEnabled ? REPORT_PERIOD_IN_SLEEP_MODE_MS
                                          : REPORT_PERIOD_IN_SLEEPLESS_MODE_MS);
  }
  if (doesNeedUpdateDisplay ||
      (millis() - lastDisplayUpdateAt) > DISPLAY_UPDATE_INTERVAL_MS) {
    displayUpdates();
  }
}

bool loopSensors() {
  if (isSleepModeEnabled) {

    // TODO: initial track right after WARM UP

    if (SLEEPING == state &&
        millis() - lastReportAt >
            (REPORT_PERIOD_IN_SLEEP_MODE_MS - SENSORS_WARM_UP_DELAY_MS)) {
      wakeUpAllSensors();
    }
    if (WAKING_UP == state &&
        millis() - lastReportAt > REPORT_PERIOD_IN_SLEEP_MODE_MS) {
      log("Sensors have woken up.");
      state = READY;
    }
    if (READY == state) {
      log("Reading values");
      readAllSensors();
      sendAllSensorsToSleep();
      lastReportAt = millis();
      return true;
    }
    return false;
  } else {
    if (WAKING_UP == state &&
        millis() - lastReportAt > SENSORS_WARM_UP_DELAY_MS) {
      log("Sensors have woken up.");
      state = READY;
    }
    if (READY == state &&
        millis() - lastReportAt > REPORT_PERIOD_IN_SLEEPLESS_MODE_MS) {
      log("Reading values");
      readAllSensors();
      lastReportAt = millis();
      return true;
    }
    return false;
  }
}

void turnOffDisplay() {
  if (OFF != displayMode && millis() - displayIsOnSince > KEEP_DISPLAY_ON_MS) {
    displayMode = OFF;
    log("Turning display OFF");
  }
}

void readAllSensors() {
  pmRead();
  values.pm1p0 = pmGet1p0();
  values.pm2p5 = pmGet2p5();
  values.pm10p0 = pmGet10p0();
  tempHumidRead();
  values.temperature = tempGet();
  values.humidity = humidGet();
  values.co2 = co2Get();
}

bool readTouchButton() {
  bool isButtonPressed = digitalRead(PIN_TOUCH_BUTTON);
  if (isButtonPressed != prevButtonState) {
    prevButtonState = isButtonPressed;
    if (true == isButtonPressed) {
      // toggleSleepMode();
      toggleDisplayMode();
      return true;
    }
  }
  return false;
}

void toggleSleepMode() {
  isSleepModeEnabled = !isSleepModeEnabled;
  if (isSleepModeEnabled) {
    log("\nSleep mode was enabled\n");
  } else {
    log("\nSleep mode was disabled\n");
    wakeUpAllSensors();
  }
}

void toggleDisplayMode() {
  switch (displayMode) {
  case OFF: {
    log("toggleDisplayMode() from OFF to DATA");
    displayMode = DATA;
    displayIsOnSince = millis();
    return;
  }
  case DATA: {
    log("toggleDisplayMode() from DATA to TECH_INFO");
    displayMode = TECH_INFO;
    displayIsOnSince = millis();
    return;
  }
  case TECH_INFO: {
    log("toggleDisplayMode() from TECH_INFO to OFF");
    displayMode = OFF;
    return;
  }
  default: {
    log("Unknown DisplayMode: ", false);
    log(displayMode);
    displayMode = OFF;
    return;
  }
  }
}

void sendAllSensorsToSleep() {
  log("Sending sensors to sleep");
  pmSleep();
  state = SLEEPING;
}

void wakeUpAllSensors() {
  log("\nWaking up sensors");
  pmWakeUp();

  state = WAKING_UP;
}

void displayUpdates() {
  lastDisplayUpdateAt = millis();
  switch (displayMode) {
  case OFF: {
    display("");
    return;
  }
  case DATA: {
    char co2String[20] = "   CO2: waiting";
    if (values.co2 != UNSET_INTEGER) {
      sprintf(co2String, "   CO2: %d ppm", values.co2);
    }

    char pm1p0String[20] = "PM 1.0: waiting";
    if (values.pm1p0 != UNSET_INTEGER) {
      sprintf(pm1p0String, "PM 1.0: %d ug/m3", values.pm1p0);
    }

    char pm2p5String[20] = "PM 2.5: waiting";
    if (values.pm2p5 != UNSET_INTEGER) {
      sprintf(pm2p5String, "PM 2.5: %d ug/m3", values.pm2p5);
    }

    char pm10p0String[20] = " PM 10: waiting";
    if (values.pm10p0 != UNSET_INTEGER) {
      sprintf(pm10p0String, " PM 10: %d ug/m3", values.pm10p0);
    }

    char tempString[20] = "  Temp: waiting";
    if (values.temperature != UNSET_INTEGER) {
      sprintf(tempString, "  Temp: %.1f C", values.temperature);
    }

    char humidString[20] = " Humid: waiting";
    if (values.humidity != UNSET_INTEGER) {
      sprintf(humidString, " Humid: %.1f %%", values.humidity);
    }

    display(String(co2String) + "\n" + pm1p0String + "\n" + pm2p5String + "\n" +
            pm10p0String + "\n" + tempString + "\n" + humidString);
    return;
  }
  case TECH_INFO: {
    char buff[100];
    sprintf(buff,
            "  Ver: %s\n"
            " WiFi: %s %d\n"
            " SSID: %s\n"
            "   IP: %s\n"
            "   Up: %s\n"
            "Sleep: %s\n",
            VERSION, wifiIsConnected() ? "OK" : "FAIL", wifiGetSignal(),
            wifiGetSSID(), wifiGetIP(), getUptimeString(),
            (isSleepModeEnabled ? "ON" : "OFF"));
    display(buff);
    return;
  }
  default: {
    display(String("Unknown DisplayMode: ") + displayMode);
    return;
  }
  }
}

String getUptimeString() {
  const unsigned long now = millis();
  const unsigned long seconds = now / 1000;
  if (seconds <= 60) {
    return String(seconds) + " sec";
  }

  const unsigned long minutes = seconds / 60;
  if (minutes <= 60) {
    const uint8_t secondsLeft = seconds % 60;
    return String(minutes) + "min " + secondsLeft + "sec";
  }

  const u_int16_t hours = minutes / 60;
  if (hours <= 24) {
    const uint8_t minutesLeft = minutes % 60;
    return String(hours) + "hour " + minutesLeft + "min";
  }

  const u_int8_t days = hours / 24;
  if (days <= 30) {
    const uint8_t hoursLeft = hours % 60;
    return String(days) + "day " + hoursLeft + "hour";
  }
  const u_int8_t months = days / 30;
  const uint8_t daysLeft = days % 30;
  return String(months) + "month " + daysLeft + "day";
}
