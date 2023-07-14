#ifndef CONFIG_H
#define CONFIG_H

#define VERSION "1.9.1"
#define DEVICE_NAME "spy4x-aqs"

// -------------------------------- FEATURE FLAGS

#define SHOULD_LOG_TO_SERIAL true

// -------------------------------- NETWORK

// #define WIFI_SSID "Eirene-iPhone"
// #define WIFI_PASSWORD "Danger1234"

// #define WIFI_SSID "CozyHome 2.4G"
// #define WIFI_PASSWORD "WYC95555"

// #define WIFI_SSID "spy4x-mobile"
// #define WIFI_PASSWORD "Danger1234"

#define WIFI_SSID "TP-Link_B0E4"
#define WIFI_PASSWORD "pepperpots"

#define API_URL "https://home.antonshubin.com/api/aqs"

// -------------------------------- PINS - I2C on ESP32: SDA on 21, SCL on 22

#define PIN_DHT22 15
#define PIN_LED_WIFI_STATUS LED_BUILTIN
#define PMS5003_SERIAL Serial1 // Serial1 on ESP32 - RX: SD2 pin, TX: SD3 pin
#define MHZ19E_SERIAL Serial2  // Serial2 on ESP32 - RX: 16 pin, TX: 17 pin

// -------------------------------- TIMERS
#define SENSORS_INITIAIL_WARM_UP_DELAY_MS 5 * 60 * 1000 // 5 minutes
#define SENSORS_AFTER_SLEEP_WARM_UP_DELAY_MS 30 * 1000  // 30 seconds
#define REPORT_INTERVAL_MS 2 * 60 * 1000                // 2 minutes

// -------------------------------- MISC
#define LOG_BAUDRATE 115200
#define SENSORS_BAUDRATE 9600

#endif