#include "MyWifi.h"

void MyWifi::setupWifi(const char *ssid, const char *password, wifi_mode_t wifiMode) {
  WiFi.mode(wifiMode);
  WiFi.begin(ssid, password);
  Serial.printf("Connecting to %s...\n", ssid);

  uint32_t counter = 0;
  uint32_t retryDelay = 500;
  while (WiFi.status() != WL_CONNECTED) {
    delay(retryDelay);
    Serial.printf("Retrying...%u\n", ++counter);
  }

  WiFi.setAutoReconnect(true);
  Serial.println(WiFi.localIP());
  Serial.printf("Wifi strength is %u\n", WiFi.RSSI());
}
