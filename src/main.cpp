#include <Arduino.h>
#include <SPIFFS.h>
#include "Server.h"

// File system

void initSPIFFS();

void setup() {
  Serial.begin(115200);
  initSPIFFS();
  setupWiFi();
  setupServer();
}

void loop() {
}

// Initialize SPIFFS
void initSPIFFS() {
  if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}
