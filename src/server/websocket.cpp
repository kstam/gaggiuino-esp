#include "websocket.h"

AsyncWebSocket ws("/ws");

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
void onEvent(
  AsyncWebSocket       *server,
  AsyncWebSocketClient *client,
  AwsEventType          type,
  void                 *arg,
  uint8_t              *data,
  size_t                len
);

void setupWebSocket(AsyncWebServer& server) {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void wsCleanup() {
  ws.cleanupClients();
}

void onEvent(
  AsyncWebSocket       *server,
  AsyncWebSocketClient *client,
  AwsEventType          type,
  void                 *arg,
  uint8_t              *data,
  size_t                len
) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {

    const uint8_t size = JSON_OBJECT_SIZE(1);
    StaticJsonDocument<size> json;
    DeserializationError err = deserializeJson(json, data);
    if (err) {
      Serial.print(F("deserializeJson() failed with code "));
      Serial.println(err.c_str());
      return;
    }

    const char *action = json["action"];
    const char *actionData = json["data"];
    Serial.printf("Message: %s -> %s\n", action, actionData);
  }
}

//-------------------------------------------------------------//
//--------------------------OUTGOING---------------------------//
//-------------------------------------------------------------//
void wsSendSensorStatesToClients(
  float temp,
  float pressure,
  float pumpFlow,
  float weightFlow,
  float weight,
  float shotWeight
) {
    const uint8_t size = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(8);
    StaticJsonDocument<size> json;

    json["action"] = "sensor_data_update";

    JsonObject data = json.createNestedObject("data");
    data["temp"] = temp;
    data["pressure"] = pressure;
    data["pumpFlow"] = pumpFlow;
    data["weightFlow"] = weightFlow;
    data["weight"] = weight;
    data["shotWeight"] = shotWeight;

    String dataTxt;
    size_t len = serializeJson(json, dataTxt);
    AsyncWebSocketMessageBuffer * buffer = ws.makeBuffer(len); //  creates a buffer (len + 1) for you.
    ws.textAll(buffer);
}
