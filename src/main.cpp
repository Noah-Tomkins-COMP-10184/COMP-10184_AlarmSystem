/**
 * I, Noah Tomkins, 0000790079 certify that this material is my original work.
 * No other person's work has been used without due acknowledgement.
 */

#include <Arduino.h>

void setup() {
  Serial.begin(115200);

  Serial.println("\n\nHello, World!");

  Serial.println("\n\nNoah Tomkins (000790079)");
  
  Serial.println("\nESP8266 Chip ID: " + String(ESP.getChipId()));
  Serial.println("Flash Chip ID: " + String(ESP.getFlashChipId()));

  Serial.println("\n");
}

void loop() {
  Serial.println(String(millis()) + " ms");
  delay(2000);
}