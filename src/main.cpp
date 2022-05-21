#include <Arduino.h>
#include <WiFi.h>

#define SSID "KSAFE G"
#define PASSWORD "kashif123"

// Prototypes 
void setupWifi(const char *ssid, const char *password, wifi_mode_t wifiMode);

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  setupWifi(SSID, PASSWORD, WIFI_STA);
  Serial.println(millis());
}

void loop() {
  delay(150);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(150);
  digitalWrite(LED_BUILTIN, LOW);
}

void setupWifi(const char *ssid, const char *password, wifi_mode_t wifiMode = WIFI_STA) {
  WiFi.mode(wifiMode);
  WiFi.begin(ssid, password);
  Serial.printf("Connecting to %s...\n", SSID);

  uint32_t counter = 0;
  uint32_t retryDelay = 500;
  while (WiFi.status() != WL_CONNECTED) {
    delay(retryDelay);
    Serial.printf("Retrying...%u\n", ++counter);
  }

  Serial.println(WiFi.localIP());
  Serial.printf("Wifi strength is %u\n", WiFi.RSSI());
}
