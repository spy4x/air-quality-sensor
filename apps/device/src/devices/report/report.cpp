#include "report.h"
#include "devices/http/http.h"
#include "devices/wifi/wifi.h"
#include <memory>
#include <vector>

typedef std::vector<SensorsValues> SensorsValuesVector;

void report(SensorsValues values, TechInfo info) {

  static uint32_t reportIndex = 0;
  log(String("Start Report #") + reportIndex++);

  // Connect WiFi for reporting with timeout
  wifiConnect();
  unsigned long wifiStart = millis();
  const unsigned long wifiTimeout = 10000; // 10 seconds
  while (!wifiIsConnected()) {
    if (millis() - wifiStart > wifiTimeout) {
      log("WiFi connection timeout. Aborting report and powering off WiFi...");
      wifiPowerOff();
      return;
    }
    delay(100);
  }

  // Prepare data for reporting
  SensorsValuesVector data;
  data.push_back(values);


  // Build JSON
  String payload = "";
  payload += "{\"u\":";
  payload += (millis() / 1000);
  payload += ", \"d\":[";
  for (size_t i = 0; i < data.size(); i++) {
    if (i > 0) {
      payload += ',';
    }
    SensorsValues item = data[i];
    char itemString[256];
    sprintf(itemString,
            "{"
            "\"u\":%d,"
            "\"c\":%d,"
            "\"pm1p0\":%d,"
            "\"pm2p5\":%d,"
            "\"pm10p0\":%d,"
            "\"t\":%.1f,"
            "\"h\":%.1f"
            "}",
            item.uptime, item.co2, item.pm1p0, item.pm2p5, item.pm10p0,
            item.temperature, item.humidity);
    payload += itemString;
  }
  payload += "]}";

  // Send data to server
  const HTTPResponse postResponse = http(POST, API_URL, 0, payload);

  if (postResponse.isSuccess) {
    log("Report: Success");
  } else {
    log(String("Report: Failed") + " " + postResponse.status + " " + postResponse.error);
    wifiPowerOff();
    return;
  }
  // Power off WiFi after reporting
  wifiPowerOff();
}