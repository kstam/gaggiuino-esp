#include <Arduino.h>
#include <LittleFS.h>
#include "server/server_setup.h"
#include "wifi/wifi_setup.h"
#include "server/websocket.h"

// File system

void initFS();

void setup() {
  Serial.begin(115200);
  initFS();
  setupWiFi();
  setupServer();
}

static long nextTimer;
void loop() {
  wsCleanup();

  if (millis() > nextTimer) {
    wsSendSensorStatesToClients(90.f, 0.f, 0.f, 0.f, 0.f, 0.f);
    nextTimer = millis() + 1000;
  }
}

// Initialize SPIFFS
void initFS() {
  if (!LittleFS.begin(true)) {
    Serial.println("An error has occurred while mounting LittleFS");
  }
  Serial.println("LittleFS mounted successfully");
}
