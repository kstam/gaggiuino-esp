#ifndef SERVER_H
#define SERVER_H
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <Preferences.h>
#include "esp_task_wdt.h"

const char* PARAM_INPUT_SSID = "ssid";
const char* PARAM_INPUT_PASS = "pass";

// WiFi definitions
struct WiFiParams_t {
  int wifiNetworkCount = 0;
  bool refreshWiFiNetworks = false;
  bool attemptReconnect = false;

  String ssid;
  String pass;
  Preferences preferences;
};

// Server definitions
AsyncWebServer server(80);
// AsyncWebSocket ws("/ws");
WiFiParams_t WifiParams;

void setupWiFi();
void setupServer();

//##############################################################################################################################
//############################################________________WIFI______________################################################
//##############################################################################################################################

bool initWiFi(const unsigned long interval);
void setupWiFiAccessPoint();

void setupWiFi() {
  WiFi.mode(WIFI_AP_STA);
  WifiParams.preferences.begin("gagguino_wifi");
  initWiFi(10000);
  setupWiFiAccessPoint();
}

// Initialize WiFi
bool initWiFi(const unsigned long interval) {
  unsigned long wifiStartTimer = millis();

  if (WifiParams.ssid == "" && WifiParams.pass == "") {
    WifiParams.ssid = WifiParams.preferences.getString(PARAM_INPUT_SSID);
    WifiParams.pass = WifiParams.preferences.getString(PARAM_INPUT_PASS);
  }

  Serial.printf("initWifi: status=[%d], ssid=[%s], ip=[%s].\n", WiFi.status(), WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());

  if (WifiParams.ssid == "" && WifiParams.pass == "") {
    Serial.println("No ssid or password provided.");
    return false;
  }

  WiFi.begin(WifiParams.ssid.c_str(), WifiParams.pass.c_str());
  Serial.printf("Connecting to WiFi [%s]\n", WifiParams.ssid.c_str());

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
  WifiParams.attemptReconnect = false;
  return true;
}

void scanWiFiNetworks() {
  WifiParams.wifiNetworkCount = WiFi.scanNetworks();
  if (WifiParams.wifiNetworkCount != WIFI_SCAN_FAILED) {
    WifiParams.refreshWiFiNetworks = false;
  }
}

void setupWiFiAccessPoint() {
  // Connect to Wi-Fi network with SSID and password
  WiFi.softAP("Gagguino AP", NULL);
  Serial.printf("AP (Access Point) IP address: %s\n", WiFi.softAPIP().toString().c_str());
  scanWiFiNetworks();
}

//##############################################################################################################################
//############################################________________SERVER_____________################################################
//##############################################################################################################################

void handleGetWifiStatus(AsyncWebServerRequest* request);
void handleGetNetworks(AsyncWebServerRequest* request);
void handlePostSelectedNetwork(AsyncWebServerRequest* request);
void handleDeleteSelectedNetwork(AsyncWebServerRequest* request);

void setupServer() {
  // WiFi APIs
  server.on("/api/wifi/status", HTTP_GET, handleGetWifiStatus);
  server.on("/api/wifi/networks", HTTP_GET, handleGetNetworks);
  server.on("/api/wifi/selected-network", HTTP_PUT, handlePostSelectedNetwork);
  server.on("/api/wifi/selected-network", HTTP_DELETE, handleDeleteSelectedNetwork);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) { request->send(LittleFS, "/index.html", "text/html"); });
  server.serveStatic("/", LittleFS, "/");

  // Start webserver
  server.begin();
}

void handlePostSelectedNetwork(AsyncWebServerRequest* request) {
  Serial.println("Got request to connect to WiFi");
  int params = request->params();
  for (int i = 0;i < params;i++) {
    AsyncWebParameter* p = request->getParam(i);
    if (p->isPost()) {
      // HTTP POST ssid value
      if (p->name() == PARAM_INPUT_SSID) {
        WifiParams.ssid = p->value();
        Serial.printf("SSID set to: %s\n", WifiParams.ssid.c_str());
        WifiParams.preferences.putString(PARAM_INPUT_SSID, WifiParams.ssid);
      }
      // HTTP POST pass value
      if (p->name() == PARAM_INPUT_PASS) {
        WifiParams.pass = p->value();
        Serial.printf("Password set to: %s\n", "*************");
        WifiParams.preferences.putString(PARAM_INPUT_PASS, WifiParams.pass);
      }
    }
  }

  AsyncResponseStream* response = request->beginResponseStream("application/json");
  DynamicJsonDocument json(256);
  if (initWiFi(9000)) {
    json["result"] = "ok";
    json["message"] = "Done. Connected to WiFi.";
    response->setCode(200);
  } else {
    json["result"] = "failed";
    json["message"] = "Failed to connect to WiFi.";
    response->setCode(422);
  }
  serializeJson(json, *response);
  request->send(response);
}

void handleGetNetworks(AsyncWebServerRequest* request) {
  Serial.println("Got request get available WiFi networks");
  AsyncResponseStream* response = request->beginResponseStream("application/json");
  DynamicJsonDocument json(2048);
  JsonArray networksJson = json.to<JsonArray>();

  for (int i = 0; i < WifiParams.wifiNetworkCount; i++) {
    JsonObject network = networksJson.createNestedObject();
    network["ssid"] = WiFi.SSID(i);
    network["rssi"] = WiFi.RSSI(i);
    network["secured"] = WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? false : true;
  }

  serializeJson(networksJson, *response);
  request->send(response);
  server.begin();
}

void handleDeleteSelectedNetwork(AsyncWebServerRequest* request) {
  Serial.println("Got request to disconnect connect from WiFi");
  AsyncWebServerResponse* response = request->beginResponse(204);
  request->send(response);
  if (WiFi.isConnected()) {
    WifiParams.preferences.clear();
    WiFi.disconnect();
    Serial.println("Disconnected from WiFi and cleared saved WiFi.");
  }
}

void handleGetWifiStatus(AsyncWebServerRequest* request) {
  AsyncResponseStream* response = request->beginResponseStream("application/json");
  DynamicJsonDocument json(256);
  if (WiFi.isConnected()) {
    json["status"] = "connected";
    json["ssid"] = WiFi.SSID();
    json["ip"] = WiFi.localIP().toString();
  } else {
    json["status"] = "disconnected";
    json["ssid"] = "";
    json["ip"] = "";
  }
  serializeJson(json, *response);
  request->send(response);
}
#endif
