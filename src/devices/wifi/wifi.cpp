#include "wifi.h"

#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

// void wifiReady(WiFiEvent_t event, WiFiEventInfo_t info) {}

void wifiConnect() {
  if (wifiIsConnected()) {
    return;
  }
  log(String("Connecting to WiFi \"") + WIFI_SSID + "\"");

  // WiFi.onEvent(wifiReady, SYSTEM_EVENT_WIFI_READY);

  pinMode(PIN_LED_WIFI_STATUS, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    digitalWrite(PIN_LED_WIFI_STATUS, !digitalRead(PIN_LED_WIFI_STATUS));
    log(".", false);
  }
  digitalWrite(PIN_LED_WIFI_STATUS, LOW);

  log("\nConnected. IP: ", false);
  log(WiFi.localIP());
  log("Signal: ", false);
  log(WiFi.RSSI());
}

void wifiDisconnect() { WiFi.disconnect(true); }

bool wifiIsConnected() { return WiFi.status() == WL_CONNECTED; }
String wifiGetIP() { return WiFi.localIP().toString(); }
String wifiGetSSID() { return WIFI_SSID; }
int8_t wifiGetSignal() { return WiFi.RSSI(); }