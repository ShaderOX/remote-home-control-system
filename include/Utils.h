#ifndef __UTILS_H__
#define __UTILS_H__

#include <Arduino.h>
#include <WiFi.h>

// WIFI Credentials
#define SSID "KSAFE G"
#define PASSWORD "kashif123"

// WebServer Port
#define HTTP_PORT 80 

namespace Utils {
  struct Device {
    String name;
    uint8_t pin;
    bool state;
  };

  void initPorts(Utils::Device *devices, size_t devicesCount);
  void WifiConnectivityLED(uint8_t pin);

}

#endif // __UTILS_H__
