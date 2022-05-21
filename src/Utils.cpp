#include "Utils.h"

void Utils::initPorts(Utils::Device *devices, size_t devicesCount) {
  for (uint8_t i = 0; i < devicesCount; i++) {
    pinMode(devices[i].pin, OUTPUT);
  }
}

void Utils::WifiConnectivityLED(uint8_t pin) {
  if (WiFi.status() == WL_CONNECTED) {
    delay(150);
    digitalWrite(LED_BUILTIN, HIGH);
  }
  delay(150);
  digitalWrite(LED_BUILTIN, LOW);
}
