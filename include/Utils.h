#ifndef __UTILS_H__
#define __UTILS_H__

#include <Arduino.h>
#include <WiFi.h>

// WIFI Credentials
#define SSID "A1 Lahori"
#define PASSWORD "password"

// WebServer Port
#define HTTP_PORT 80 
#define IR_RECEIVER_PIN 19 

#define WIFI_RETRY_COUNT 25 

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
