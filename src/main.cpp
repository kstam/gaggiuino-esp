#include <Arduino.h>
#include <LittleFS.h>
#include "Server.h"

// File system

void initFS();

void setup() {
  Serial.begin(115200);
  initFS();
  setupWiFi();
  setupServer();
}

void loop() {
}

// Initialize SPIFFS
void initFS() {
  if (!LittleFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}
