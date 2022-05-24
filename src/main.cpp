#include <Arduino.h>
#include <WiFi.h>
#include <IRremote.h>
#include "MyWifi.h"
#include "MyWebserver.h"
#include "Utils.h"

WiFiServer *pWebServer;
Utils::Device devices[] = {
  {"wifi", LED_BUILTIN, false, UINT32_MAX},
  { "led0", 4, false, 0xfb04ef00},
  { "led1", 5, false, 0xf807ef00},
};

const size_t DEVICES_CONNECTED = sizeof(devices) / sizeof(Utils::Device);

uint32_t currentTimestamp = 0;
uint32_t previousTimestamp = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");

  // IR Receiver Setting 
  IrReceiver.begin(IR_RECEIVER_PIN, true, 21);

  // Initialize the ports
  initPorts(devices, DEVICES_CONNECTED);

  // Connecting to WiFi
  MyWifi::setupWifi(SSID, PASSWORD);

  // Creating the HTTP WebServer 
  pWebServer = MyWebServer::initWebServer(HTTP_PORT);
  if (pWebServer == nullptr) {
    Serial.println("ERROR: Web Server not started, Wifi not connected");
  }
}

void loop() {
  currentTimestamp = millis();
  Utils::WifiConnectivityLED(LED_BUILTIN);

  // Creating a WebServer
  if (pWebServer != nullptr) {
    MyWebServer::runWebServer(devices, DEVICES_CONNECTED);
  }
  else {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Wifi Connected... Retrying to start the Web Server");
      pWebServer = MyWebServer::initWebServer(HTTP_PORT);
      if (pWebServer == nullptr) {
        Serial.println("ERROR: Web Server not started, Wifi not connected");
      }
    }
  }

  if (WiFi.status() != WL_CONNECTED && (currentTimestamp - previousTimestamp > WIFI_TIMEOUT)) {
    if (pWebServer != nullptr) {
      MyWebServer::closeWebServer();
      pWebServer = nullptr;
    }

    Serial.println("Reconnecting WiFi.... ");
    WiFi.disconnect();
    Serial.println("Disconnected WiFi.... ");
    WiFi.reconnect();
    if (WiFi.status() == WL_CONNECTED)
      Serial.println("Wifi Connected");
    previousTimestamp = currentTimestamp;
  }

  // Check for IR Receiver
  if (IrReceiver.decode()) {
    uint32_t irCode = IrReceiver.decodedIRData.decodedRawData;
    Serial.printf("IR Signal Received %x\n", irCode);

    for (size_t i = 0; i < DEVICES_CONNECTED; i++) {
      if (devices[i].irCode == irCode) {
        devices[i].state = !devices[i].state;
        digitalWrite(devices[i].pin, devices[i].state);
        Serial.printf("%s %s\n", devices[i].name.c_str(), devices[i].state ? "ON" : "OFF");
      }
    }

    IrReceiver.resume();
  }
}
