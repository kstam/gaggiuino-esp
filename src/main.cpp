#include <Arduino.h>
#include <LittleFS.h>
#include "server/server_setup.h"
#include "wifi/wifi_setup.h"

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
    Serial.println("An error has occurred while mounting LittleFS");
  }
  Serial.println("LittleFS mounted successfully");
}
