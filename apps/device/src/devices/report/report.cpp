#include "report.h"
#include "devices/http/http.h"
#include "devices/wifi/wifi.h"
#include <vector>

uint16_t reportIndex = 0;

void report(SensorsValues values, TechInfo info) {
  static std::vector<SensorsValues> data = {};
  log(String("Free heap: ") + ESP.getFreeHeap());
  log(String("Start Report #") + reportIndex++);
  data.push_back(values);

  String payload = "";
  payload += "{\"u\":";
  payload += (millis() / 1000);
  payload += +", \"d\":[";
  for (uint16_t i = 0; i < data.size(); i++) {
    if (i > 0) {
      payload += ',';
    }
    SensorsValues item = data.at(i);
    char itemString[100];
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

  const HTTPResponse postResponse = http(POST, API_URL, 0, payload);

  if (!postResponse.isSuccess) {
    log(String("Report: Failed") + " " + postResponse.status + " " +
        postResponse.error);
  } else {
    log("Report: Success");
    data.clear();
  }
}