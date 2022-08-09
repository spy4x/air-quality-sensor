// #include <ArduinoJson.h>
#include "_shared.h"

enum HTTPMethod { GET, POST, PATCH, DELETE };

struct HTTPResponse {
  int status;
  // DynamicJsonDocument body;
  bool isSuccess;
  String error;
};

HTTPResponse http(HTTPMethod method, String url, int responseBodySize,
                  String payload);