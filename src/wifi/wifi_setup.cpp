#include "wifi_setup.h"
#include "esp_task_wdt.h"

const char* PARAM_INPUT_SSID = "ssid";
const char* PARAM_INPUT_PASS = "pass";
WiFiParams_t wifiParams;

void setupWiFiAccessPoint();

WiFiParams_t& getWifiParams() {
  return wifiParams;
}

void setupWiFi() {
  WiFi.mode(WIFI_AP_STA);
  wifiParams.preferences.begin("gagguino_wifi");
  initWiFi(10000);
  setupWiFiAccessPoint();
}

// Initialize WiFi
bool initWiFi(const unsigned long interval) {
  unsigned long wifiStartTimer = millis();

  if (wifiParams.ssid == "" && wifiParams.pass == "") {
    wifiParams.ssid = wifiParams.preferences.getString(PARAM_INPUT_SSID);
    wifiParams.pass = wifiParams.preferences.getString(PARAM_INPUT_PASS);
  }

  Serial.printf("initWifi: status=[%d], ssid=[%s], ip=[%s].\n", WiFi.status(), WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());

  if (wifiParams.ssid == "" && wifiParams.pass == "") {
    Serial.println("No ssid or password provided.");
    return false;
  }

  WiFi.begin(wifiParams.ssid.c_str(), wifiParams.pass.c_str());
  Serial.printf("Connecting to WiFi [%s]\n", wifiParams.ssid.c_str());

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(WiFi.status());
    Serial.print(".");
    if (WiFi.status() == WL_CONNECT_FAILED) {
      Serial.println("\nFailed to connect. Check password.");
      return false;
    }
    if (millis() - wifiStartTimer >= interval) {
      Serial.printf("\nFailed to connect after %ld seconds.\n", interval / 1000);
      return false;
    }
    esp_task_wdt_reset();
    delay(200);
  }

  Serial.printf("\nConnected to WiFi [%s] with IP:[%s]\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
  wifiParams.attemptReconnect = false;
  return true;
}

void scanWiFiNetworks() {
  wifiParams.wifiNetworkCount = WiFi.scanNetworks();
  if (wifiParams.wifiNetworkCount != WIFI_SCAN_FAILED) {
    wifiParams.refreshWiFiNetworks = false;
  }
}

void setupWiFiAccessPoint() {
  // Connect to Wi-Fi network with SSID and password
  WiFi.softAP("Gagguino AP", NULL);
  Serial.printf("AP (Access Point) IP address: %s\n", WiFi.softAPIP().toString().c_str());
  scanWiFiNetworks();
}
