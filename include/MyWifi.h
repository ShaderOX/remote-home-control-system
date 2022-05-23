#ifndef __MY_WIFI_H__
#define __MY_WIFI_H__

#include <Arduino.h>
#include <WiFi.h>
#include "Utils.h"

namespace MyWifi {
  static uint32_t currentTime = 0;
  static uint32_t lastTime = 0;
  static const uint32_t waitInterval = 5000;

  void setupWifi(const char *ssid, const char *password, bool isReconnecting = false, wifi_mode_t wifiMode = WIFI_STA);
}

#endif // __MY_WIFI_H__
