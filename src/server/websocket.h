#ifndef WEBSOCKET_API_H
#define WEBSOCKET_API_H

#include "ESPAsyncWebServer.h"
#include "AsyncTCP.h"
#include <ArduinoJson.h>

void setupWebSocket(AsyncWebServer& server);
void wsCleanup();
void wsSendSensorStatesToClients(
  float temp,
  float pressure,
  float pumpFlow,
  float weightFlow,
  float weight,
  float shotWeight
);

#endif
