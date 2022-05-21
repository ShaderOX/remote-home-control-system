#ifndef __MY_WIFI_H__
#define __MY_WIFI_H__

#include <Arduino.h>
#include <WiFi.h>

namespace MyWifi {
  void setupWifi(const char *ssid, const char *password, wifi_mode_t wifiMode = WIFI_STA);
}

#endif // __MY_WIFI_H__
