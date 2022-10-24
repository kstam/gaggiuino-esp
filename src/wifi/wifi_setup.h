#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H
#include <WiFi.h>
#include <Preferences.h>
#include <Arduino.h>

// WiFi definitions
struct WiFiParams_t {
  int wifiNetworkCount = 0;
  bool refreshWiFiNetworks = false;
  bool attemptReconnect = false;

  String ssid;
  String pass;
  Preferences preferences;
};

void setupWiFi();
bool initWiFi(const unsigned long interval);
void setupWiFiAccessPoint();

extern const char* PARAM_INPUT_SSID;
extern const char* PARAM_INPUT_PASS;
extern WiFiParams_t wifiParams;
#endif
