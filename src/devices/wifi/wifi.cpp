#include "wifi.h"
#include <WiFi.h>

void wifiWaitForConnection();

void wifiConnect() {
  if (wifiIsConnected()) {
    log("WiFi is connected.");
    return;
  }
  WiFi.disconnect();
  WiFi.setHostname(DEVICE_NAME);

  log("Connecting to WiFi: ", false);
  log(WIFI_SSID);

  pinMode(PIN_LED_WIFI_STATUS, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  wifiWaitForConnection();
}

void wifiReconnect() {
  if (!wifiIsConnected()) {
    log("WiFi connection lost. Reconnecting");
    WiFi.reconnect();
    wifiWaitForConnection();
  }
}

void wifiWaitForConnection() {
  while (!wifiIsConnected()) {
    digitalWrite(PIN_LED_WIFI_STATUS, !digitalRead(PIN_LED_WIFI_STATUS));
    delay(1000);
    log(".", false);
  }
  digitalWrite(PIN_LED_WIFI_STATUS, false);

  log("\nConnected. IP: ", false);
  log(WiFi.localIP().toString());
  log("Signal: ", false);
  log(String(WiFi.RSSI()));
}

void wifiDisconnect() { WiFi.disconnect(true); }

bool wifiIsConnected() { return WiFi.status() == WL_CONNECTED; }
String wifiGetIP() { return WiFi.localIP().toString(); }
String wifiGetSSID() { return WiFi.SSID(); }
int8_t wifiGetSignal() { return WiFi.RSSI(); }