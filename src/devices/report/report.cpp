#include "report.h"
#include "devices/http/http.h"
#include "devices/wifi/wifi.h"

uint32_t reportedAt = -999999;

void reportLoop(SensorsValues values, uint32_t reportInterval) {
  if (millis() - reportedAt < reportInterval) {
    return;
  }
  reportedAt = millis();
  log("Reporting");

  // DynamicJsonDocument payload(150);
  // payload["co2"] = values.co2;
  // payload["humidity"] = values.humidity;
  // payload["pm10p0"] = values.pm10p0;
  // payload["pm1p0"] = values.pm1p0;
  // payload["pm2p5"] = values.pm2p5;
  // payload["temperature"] = values.temperature;
  // payload["uptime"] = values.uptime;
  // String payloadString;
  // serializeJson(payload, payloadString);
  char payload[150];
  sprintf(payload,
          "{ "
          "\"uptime\": %d,"
          "\"temperature\": %.1f, "
          "\"humidity\": %.1f, "
          "\"co2\": %d, "
          "\"pm1p0\": %d, "
          "\"pm2p5\": %d, "
          "\"pm10p0\": %d "
          " }",
          values.uptime, values.temperature, values.humidity, values.co2,
          values.pm1p0, values.pm2p5, values.pm10p0);

  const HTTPResponse postResponse = http(POST, API_URL, 0, payload);

  if (!postResponse.isSuccess) {
    log(String("Report: Failed") + postResponse.status + " " +
        postResponse.error);
  } else {
    log("Report: Success");
  }
  wifiDisconnect();
}