#include "MyWifi.h"

void MyWifi::setupWifi(const char *ssid, const char *password, bool isReconnecting, wifi_mode_t wifiMode) {
  WiFi.disconnect();
  WiFi.mode(wifiMode);
  WiFi.begin(ssid, password);

  if (!isReconnecting)
    Serial.printf("Connecting to %s...\n", ssid);

  uint32_t counter = 0;
  uint32_t retryDelay = 500;

  currentTime = millis();
  uint32_t diff = currentTime - lastTime;
  bool shouldAttemptReconnect = diff > waitInterval;

  if (isReconnecting && shouldAttemptReconnect) {
    lastTime = currentTime;
  }
  else return;

  while (WiFi.status() != WL_CONNECTED) {
    delay(retryDelay);
    Serial.printf("Retrying...%u\n", ++counter);

    if (counter >= (isReconnecting ? 1 : WIFI_RETRY_COUNT)) {
      Serial.printf("ERROR: Couldn't connect to WiFi, will retry in %ums", waitInterval);
      break;
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    WiFi.setAutoReconnect(true);
    Serial.println(WiFi.localIP());
    Serial.printf("Wifi strength is %u\n", WiFi.RSSI());
  }
}
