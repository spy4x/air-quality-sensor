#ifndef CONFIG_H
#define CONFIG_H

#define VERSION "1.3.0"

// -------------------------------- PINS
#define PIN_BUZZER 18
#define PIN_TOUCH_BUTTON 12
#define PIN_DHT22 14

#if defined(ESP32)
#define MHZ19E_SERIAL Serial1 // Serial1 on ESP32 - RX: SD2 pin, TX: SD3 pin
#else
#define PIN_MHZ19E_RX 10
#define PIN_MHZ19E_TX 11
#endif

#if defined(ESP32)
#define PMS5003_SERIAL Serial2 // Serial1 on ESP32 - RX: 16 pin, TX: 17 pin
#else
#define PMS5003_TX 2
#define PMS5003_RX 3
#endif

// I2C on ESP32: SDA on 21, SCL on 22

// -------------------------------- TIMERS
#define REPORT_PERIOD_MS 60 * 1000
#define SENSORS_WARM_UP_DELAY_MS 30 * 1000
#define HIGH_LEVELS_NOTIFICATIONS_INTERVAL 60 * 1000

// -------------------------------- MISC
#define SENSORS_BAUDRATE 9600
#define IS_SLEEP_MODE_ENABLED true

#endif