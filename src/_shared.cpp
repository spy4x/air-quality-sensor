#include "_shared.h"

void loggerSetup() {
#if SHOULD_LOG_TO_SERIAL
  Serial.begin(LOG_BAUDRATE);
  delay(1000);
#endif
}

void log(const char str[], bool shouldEndWithNewLine) {
#if SHOULD_LOG_TO_SERIAL
  if (shouldEndWithNewLine) {
    Serial.println(str);
  } else {
    Serial.print(str);
  }
#endif
}

void log(String str, bool shouldEndWithNewLine) {
#if SHOULD_LOG_TO_SERIAL
  if (shouldEndWithNewLine) {
    Serial.println(str);
  } else {
    Serial.print(str);
  }
#endif
}