#include <Arduino.h>
#include <WiFi.h>
#include <IRremote.h>
#include "MyWifi.h"
#include "MyWebserver.h"
#include "Utils.h"

WiFiServer *pWebServer;
Utils::Device devices[] = {
  {"wifi", LED_BUILTIN, false, 0x0},
  { "led0", 4, false, 0xe9169f00},
  { "led1", 5, false, 0xb8479f00},
  { "led2", 21, false, 0xa8579f00}
};

const size_t DEVICES_CONNECTED = sizeof(devices) / sizeof(Utils::Device);

void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");

  // IR Receiver Setting 
  IrReceiver.begin(IR_RECEIVER_PIN);

  // Initialize the ports
  initPorts(devices, DEVICES_CONNECTED);

  // Connecting to WiFi
  MyWifi::setupWifi(SSID, PASSWORD);

  // Starting the HTTP WebServer 
  pWebServer = MyWebServer::initWebServer(HTTP_PORT);
  if (pWebServer == nullptr) {
    Serial.println("ERROR: Web Server not started, Wifi not connected");
  }
}

void loop() {
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

  if (WiFi.status() != WL_CONNECTED) {
    MyWebServer::closeWebServer();
    pWebServer = nullptr;

    MyWifi::setupWifi(SSID, PASSWORD, true);
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
