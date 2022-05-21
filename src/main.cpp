#include <Arduino.h>
#include <WiFi.h>
#include "MyWifi.h"
#include "MyWebserver.h"
#include "Utils.h"

WiFiServer *pWebServer;
Utils::Device devices[] = {
  {"wifi", LED_BUILTIN, false},
  { "led0", 4, false },
  { "led1", 5, false }
};

const size_t DEVICES_CONNECTED = sizeof(devices) / sizeof(Utils::Device);

void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");

  // Initialize the ports
  initPorts(devices, DEVICES_CONNECTED);

  // Connecting to WiFi
  MyWifi::setupWifi(SSID, PASSWORD);

  // Starting the HTTP WebServer 
  pWebServer = MyWebServer::initWebServer(HTTP_PORT);
  if (pWebServer == nullptr) {
    Serial.println("ERROR: Web Server not started, Wifi not connected");
    return;
  }
}

void loop() {
  Utils::WifiConnectivityLED(LED_BUILTIN);

  // Creating a WebServer
  MyWebServer::runWebServer(devices, DEVICES_CONNECTED);
}
