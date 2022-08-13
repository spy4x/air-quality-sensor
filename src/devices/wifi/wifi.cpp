#include "wifi.h"

#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

// void wifiReady(WiFiEvent_t event, WiFiEventInfo_t info) {}

void wifiConnect(bool shouldWaitForConnect) {
  if (wifiIsConnected()) {
    log("WiFi is connected.");
    return;
  }
  log("Connecting to WiFi: ", false);
  log(WIFI_SSID);
  // WiFi.onEvent(wifiReady, SYSTEM_EVENT_WIFI_READY);

  pinMode(PIN_LED_WIFI_STATUS, OUTPUT);

  digitalWrite(PIN_LED_WIFI_STATUS, HIGH);
  delay(200);
  digitalWrite(PIN_LED_WIFI_STATUS, LOW);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  if (shouldWaitForConnect) {
    while (WiFi.status() != WL_CONNECTED) {
      delay(200);
      log(".", false);
    }
  }

  if (wifiIsConnected()) {
    log("\nConnected. IP: ", false);
    log(WiFi.localIP().toString());
    log("Signal: ", false);
    log(String(WiFi.RSSI()));
  }
}

void wifiDisconnect() { WiFi.disconnect(true); }

bool wifiIsConnected() { return WiFi.status() == WL_CONNECTED; }
String wifiGetIP() { return WiFi.localIP().toString(); }
String wifiGetSSID() { return WiFi.SSID(); }
int8_t wifiGetSignal() { return WiFi.RSSI(); }