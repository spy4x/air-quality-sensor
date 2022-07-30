// -------------------------------- LIBRARIES
#include "co2.h"
#include "display.h"
#include "pm.h"
#include "shared.h"
#include "tempHumid.h"

// -------------------------------- CONFIG
#include "config.h"

// -------------------------------- VARIABLES & CONSTANTS
int16_t co2 = UNSET_INTEGER;
int16_t pm2p5 = UNSET_INTEGER;
int8_t temperature = UNSET_INTEGER;
uint8_t humidity = UNSET_INTEGER;
bool updatesAvailable = false;
// unsigned long buzzerTimestamp = -HIGH_LEVELS_NOTIFICATIONS_INTERVAL;
// bool isBuzzerOn = false;

// -------------------------------- FUNCTION DECLARATIONS
void displayUpdates();
void notifyHighLevels();

void setup() {
  Serial.begin(SENSORS_BAUDRATE);
  if (!displaySetup()) {
    Serial.printf("Version: %s\n", VERSION);
    Serial.println("Display initiation failed");
    while (1) {
    }
  }
  delay(1000);
  display(String("Version: ") + "\n" + VERSION + "\nInit...", 2);
  pmSetup();
  co2Setup();
  tempHumidSetup();
  display(String("Version: ") + "\n" + VERSION + "\nWarming up\nsensors...", 2);
}

void loop() {
  pmLoop();
  co2Loop();
  tempHumidLoop();
  if (co2GetState() == READY) {
    co2 = co2GetValue();
    updatesAvailable = true;
  }
  if (pmGetState() == READY) {
    pm2p5 = pmGetValue();
    updatesAvailable = true;
  }
  if (tempHumidGetState() == READY) {
    temperature = tempGetValue();
    humidity = humidGetValue();
    updatesAvailable = true;
  }
  if (updatesAvailable) {
    displayUpdates();
    updatesAvailable = false;
  }
  notifyHighLevels();
}

void displayUpdates() {
  char co2String[20] = "  CO2: waiting";
  if (co2 != UNSET_INTEGER) {
    sprintf(co2String, "  CO2: %d ppm", co2);
  }

  char pmString[20] = "PM2.5: waiting";
  if (pm2p5 != UNSET_INTEGER) {
    sprintf(pmString, "PM2.5: %d ug/m3", pm2p5);
  }

  char tempString[20] = "    T: waiting";
  if (temperature != UNSET_INTEGER) {
    sprintf(tempString, "    T: %d C", temperature);
  }

  char humidString[20] = "Humid: waiting";
  if (humidity != UNSET_INTEGER) {
    sprintf(humidString, "Humid: %d %%", humidity);
  }
  display(String(co2String) + "\n" + pmString + "\n" + tempString + "\n" +
          humidString);
}

void notifyHighLevels() {
  // if (isBuzzerOn && millis() - buzzerTimestamp > 300) {
  //   noTone(PIN_BUZZER);
  //   isBuzzerOn = false;
  // }
  // if (co2 > 800) {
  //   display(String("CO2 level too high\n") + co2 + "ppm\nOpen windows", 2);
  //   if (millis() - buzzerTimestamp > HIGH_LEVELS_NOTIFICATIONS_INTERVAL) {
  //     buzzerTimestamp = millis();
  //     tone(PIN_BUZZER, 50);
  //     isBuzzerOn = true;
  //   }
  // }
}
