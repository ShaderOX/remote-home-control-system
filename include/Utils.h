#ifndef __UTILS_H__
#define __UTILS_H__

#include <Arduino.h>
#include <WiFi.h>

// WIFI Credentials
#define SSID "Asad"
#define PASSWORD "password"

// WebServer Port
#define HTTP_PORT 80 
#define IR_RECEIVER_PIN 19 

#define WIFI_RETRY_COUNT 25 

#define WIFI_TIMEOUT 5000 

namespace Utils {
  struct Device {
    String name;
    uint8_t pin;
    bool state;
    uint32_t irCode;
  };

  void initPorts(Utils::Device *devices, size_t devicesCount);
  void WifiConnectivityLED(uint8_t pin);

}

#endif // __UTILS_H__
