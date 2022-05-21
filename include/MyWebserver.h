#ifndef __MY_WEBSERVER_H__
#define __MY_WEBSERVER_H__

#include <Arduino.h>
#include <WiFi.h>
#include "Utils.h"

namespace MyWebServer {
  static uint32_t currentTime = 0;
  static uint32_t lastTime = 0;
  static const uint32_t waitInterval = 2000;
  static WiFiServer *server;

  static String requestHeaderPayload = "";
  static String requestPayload = "";

  WiFiServer *initWebServer(uint8_t port = 80);
  bool closeWebServer();
  void runWebServer(Utils::Device *devices, size_t devicesCount);
  WiFiServer *getWebServerInstance();
}

#endif // __MY_WEBSERVER_H__
