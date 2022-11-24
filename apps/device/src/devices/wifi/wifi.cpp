#include "wifi.h"
#include <WiFi.h>

void wifiWaitForConnection();
void wifiScanForNetworks();

void IRAM_ATTR WiFiEvent(WiFiEvent_t event) {
  switch (event) {
  case SYSTEM_EVENT_STA_CONNECTED:
    log("Connected to WiFi");
    break;
  case SYSTEM_EVENT_STA_DISCONNECTED:
    log("Disconnected from WiFi");
    break;
  default:
    break;
  }
}

void wifiConnect() {
  wifiScanForNetworks();

  WiFi.disconnect();
  WiFi.setHostname(DEVICE_NAME);
  WiFi.onEvent(WiFiEvent);

  log("Connecting to WiFi: ", false);
  log(WIFI_SSID);

  pinMode(PIN_LED_WIFI_STATUS, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // wifiWaitForConnection();
}

void wifiReconnect() {
  if (!wifiIsConnected()) {
    log("WiFi connection lost. Reconnecting");
    WiFi.reconnect();
    wifiWaitForConnection();
  }
}

void wifiScanForNetworks() {
  int n = WiFi.scanNetworks();
  if (n == 0) {
    log("No Wi-Fi networks found");
  } else {
    log("Wi-Fi networks found:\n-----------------------");
    for (int i = 0; i < n; ++i) {
      log(String(i + 1) + ": " + WiFi.SSID(i) + " (" + WiFi.RSSI(i) + ")");
      delay(10);
    }
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