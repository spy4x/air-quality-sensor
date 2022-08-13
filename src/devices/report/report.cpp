#include "report.h"
#include "devices/http/http.h"
#include "devices/wifi/wifi.h"

void report(SensorsValues values, TechInfo info) {
  log("Report: Start");

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
          "\"co2\": %d, "
          "\"pm2p5\": %d, "
          "\"temperature\": %.1f, "
          "\"humidity\": %.1f "
          " }",
          millis() / 1000, values.co2, values.pm2p5, values.temperature,
          values.humidity);

  const HTTPResponse postResponse = http(POST, API_URL, 0, payload);

  if (!postResponse.isSuccess) {
    log(String("Report: Failed") + postResponse.status + " " +
        postResponse.error);
  } else {
    log("Report: Success");
  }
}