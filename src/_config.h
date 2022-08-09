#ifndef CONFIG_H
#define CONFIG_H

#define VERSION "1.6.0"

// -------------------------------- FEATURE FLAGS

#define SHOULD_LOG_TO_SERIAL true

// -------------------------------- NETWORK
#define WIFI_SSID "TurkTelekom_ZT29JH_2.4GHz"
#define WIFI_PASSWORD "ChHdXu4asPFt"
#define API_URL "http://192.168.1.34:5000/api/aqs"

// -------------------------------- PINS

// I2C on ESP32: SDA on 21, SCL on 22

#define PIN_TOUCH_BUTTON 12
#define PIN_DHT22 14
#define PIN_LED_WIFI_STATUS LED_BUILTIN
#define MHZ19E_SERIAL Serial1  // Serial1 on ESP32 - RX: SD2 pin, TX: SD3 pin
#define PMS5003_SERIAL Serial2 // Serial1 on ESP32 - RX: 16 pin, TX: 17 pin

// -------------------------------- TIMERS
#define REPORT_PERIOD_IN_SLEEP_MODE_MS 2 * 60 * 1000 // 2 minutes
#define REPORT_PERIOD_IN_SLEEPLESS_MODE_MS 5000      // 5 seconds
#define SENSORS_WARM_UP_DELAY_MS 30 * 1000           // 30 seconds
#define KEEP_DISPLAY_ON_MS 60 * 1000                 // 60 seconds
#define DISPLAY_UPDATE_INTERVAL_MS 5 * 1000          // 60 seconds

// -------------------------------- MISC
#define SENSORS_BAUDRATE 9600

#endif