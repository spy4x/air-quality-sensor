#include "display.h"
#include <Adafruit_SSD1306.h>
#include <Wire.h>

Adafruit_SSD1306 oled(128, 64, &Wire, -1);

bool displaySetup() {
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    return false;
  }
  oled.clearDisplay();
  return true;
}

void display(String text, const uint8_t textSize = 1) {
  oled.clearDisplay();
  oled.setTextColor(SSD1306_WHITE);
  oled.setTextSize(textSize);
  oled.setCursor(0, 0);
  oled.cp437(true);
  oled.print(text);
  oled.display();
  log("OLED:");
  log(text);
}