#include "wifi.h"
#include <WiFi.h>

void wifiWaitForConnection();
void wifiScanForNetworks();

void wifiConnect() {
  if (wifiIsConnected()) {
    log("WiFi is connected.");
    return;
  }
  wifiScanForNetworks();

  WiFi.disconnect();
  WiFi.setHostname(DEVICE_NAME);

  log("Connecting to WiFi: ", false);
  log(WIFI_SSID);

  pinMode(PIN_LED_WIFI_STATUS, OUTPUT);

  WiFi.begin(WIFI_SSID);

  wifiWaitForConnection();
}

void wifiReconnect() {
  if (!wifiIsConnected()) {
    log("WiFi connection lost. Reconnecting");
    WiFi.reconnect();
    wifiWaitForConnection();
  }
}

void wifiScanForNetworks() {
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  if (n == 0) {
    log("No Wi-Fi networks found");
  } else {
    log(String(n) + " Wi-Fi networks found:\n--------------\n");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
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