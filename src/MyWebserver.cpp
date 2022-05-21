#include "MyWebserver.h"


WiFiServer *MyWebServer::initWebServer(uint8_t port) {
  if (WiFi.status() != WL_CONNECTED) {
    // WiFi not connected, cannot start initialize Web Server
    return nullptr;
  }
  server = new WiFiServer(port);
  server->begin();
  Serial.println("HTTP server listening at http://" + WiFi.localIP().toString());

  return server;
}

WiFiServer *MyWebServer::getWebServerInstance() {
  return server;
}

bool MyWebServer::closeWebServer() {
  if (server != nullptr) {
    server->stopAll();
    server->close();
    delete server;

    return true;
  }
  return false;
}

void MyWebServer::runWebServer(Utils::Device *devices, size_t devicesCount) {
  WiFiClient client = server->available();
  if (client) {
    currentTime = millis();
    if (client.connected() && (currentTime - lastTime) > waitInterval) {
      Serial.printf("Client Connected (%s)...\n", client.remoteIP().toString().c_str());
    }
    String requestPayload = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.printf("%c", c);
        requestHeaderPayload += c;

        if (c == '\n') {
          if (requestPayload.length() == 0) {
            String contentType = "text/json";
            String statusText = "200 OK";
            String response;

            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();


            if (requestHeaderPayload.indexOf("GET / ") != -1) {
              String html = "<!DOCTYPE html>\r\n<html lang=\"en\">\r\n\r\n<head>\r\n  <meta charset=\"UTF-8\">\r\n  <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\r\n  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n  <title>ESP32 | Remote Home Control System</title>\r\n</head>\r\n\r\n<body>\r\n  <button id=\"led0\">LED 0</button>\r\n  <button id=\"led1\">LED 1</button>\r\n</body>\r\n\r\n<script defer>\r\n\r\n  fetch(\'/status\').then(res => res.json()).then(state => {\r\n    const buttons = document.querySelectorAll(\"button\");\r\n    updateButton(buttons, state);\r\n\r\n    buttons.forEach(button => button.addEventListener(\"click\", () => {\r\n      fetch(`/${button.id}/toggle`).then(response => response.json()).then(data => {\r\n        console.log(data);\r\n        state = { ...state, ...data};\r\n        console.log({ newState: state });\r\n        updateButton(buttons, state);\r\n      }).catch(e => {\r\n        console.log(e);\r\n      });\r\n    }));\r\n\r\n    function updateButton(buttons, state) {\r\n      buttons.forEach(button => {\r\n        if (state[button.id]) {\r\n          button.style.backgroundColor = \"green\";\r\n        } else {\r\n          button.style.backgroundColor = \"red\";\r\n        }\r\n      });\r\n    }\r\n\r\n  });\r\n\r\n</script>\r\n\r\n</html>\r\n\r\n\r\n<!-- https://jsonformatter.org/javascript-escape -->\r\n";
              client.println(html);
              client.println();

            }
            else if (requestHeaderPayload.indexOf("GET /led0/toggle") != -1) {
              for (int i = 0; i < devicesCount; i++) {
                if (devices[i].name == "led0") {
                  devices[i].state = !devices[i].state;
                  digitalWrite(devices[i].pin, devices[i].state);
                  client.println("{\"led0\": " + String(devices[i].state) + "}");
                  break;
                }
              }
            }
            else if (requestHeaderPayload.indexOf("GET /led1/toggle") != -1) {
              for (int i = 0; i < devicesCount; i++) {
                if (devices[i].name == "led1") {
                  devices[i].state = !devices[i].state;
                  digitalWrite(devices[i].pin, devices[i].state);
                  client.println("{\"led1\": " + String(devices[i].state) + "}");
                  break;
                }
              }
            }
            else if (requestHeaderPayload.indexOf("GET /status") != -1) {
              String json = "{";
              for (int i = 0; i < devicesCount; i++) {
                if (i > 0) {
                  json += ",";
                }
                json += "\"" + devices[i].name + "\": " + String(devices[i].state);
              }
              json += "}";
              client.println(json);
              client.println();
            }
            else {
              client.println("404");
              client.println();
            }

            break;
          }
          else {
            requestPayload = "";
          }
        }
        else if (c != '\r') {
          requestPayload += c;
        }
      }
    }
    requestHeaderPayload = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}


