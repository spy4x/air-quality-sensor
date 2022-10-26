#include "http.h"
#include "_shared.h"
#include <HTTPClient.h>
// #include <StreamUtils.h>

static const char *HTTPMethodString[] = {"GET", "POST", "PATCH", "DELETE"};

HTTPResponse http(HTTPMethod method, String url, int responseBodySize,
                  String payload) {
  // configure stuff
  const int timeout = 5000;
  HTTPClient client;
  client.setReuse(false);
  client.setConnectTimeout(timeout);
  client.setTimeout(timeout);
  // DynamicJsonDocument body(responseBodySize);
  HTTPResponse response = {status : 0, /*  body : body,  */ isSuccess : false};

  // logging
  String logPrefix = String(HTTPMethodString[method]) + " " + url;
  log(logPrefix + " " + payload);

  // connect
  const bool isConnected = client.begin(url);
  if (!isConnected) {
    const String error = logPrefix + " Connection failed";
    log(error);
    response.error = error;
    response.isSuccess = false;
    return response;
  }

  // make request
  client.addHeader("Content-Type", "application/json");
  response.status = client.sendRequest(HTTPMethodString[method], payload);
  log(logPrefix + " " + response.status);
  response.isSuccess = response.status >= 200 && response.status < 300;

  // parse response body
  // if (responseBodySize) {
  //   ReadLoggingStream loggingStream(client.getStream(), Serial);
  //   DeserializationError error = deserializeJson(response.body,
  //   loggingStream); Serial.println(); if (error) {
  //     response.error = error.c_str();
  //     response.isSuccess = false;
  //     Serial.println(logPrefix + " JSON parse error: " + response.error);
  //   }
  // }

  return response;
}
