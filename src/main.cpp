#include <Arduino.h>
#include <WiFi.h>

#define SSID "KSAFE G"
#define PASSWORD "kashif123"

#define HTTP_PORT 80 

#define TEST_LED_PIN 4

// Global Variables 
String clientHeader = "";

uint32_t currentTime = 0;
uint32_t lastTime = 0;
const uint32_t waitInterval = 2000;

uint8_t TEST_LED_STATE = false;

// Prototypes 
void setupWifi(const char *ssid, const char *password, wifi_mode_t wifiMode = WIFI_STA);

WiFiServer server(HTTP_PORT);

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  setupWifi(SSID, PASSWORD);

  pinMode(TEST_LED_PIN, OUTPUT);

  // Starting the HTTP WebServer 
  server.begin();
  Serial.println("HTTP server listening at http://" + WiFi.localIP().toString());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    delay(150);
    digitalWrite(LED_BUILTIN, HIGH);
  }
  delay(150);
  digitalWrite(LED_BUILTIN, LOW);

  // Creating a WebServer
  WiFiClient client = server.available();
  if (client) {
    currentTime = millis();
    if (client.connected() && (currentTime - lastTime) > waitInterval) {
      Serial.printf("Client Connected (%s)...\n", client.remoteIP().toString().c_str());
    }
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.printf("%c", c);
        clientHeader += c;

        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();


            if (clientHeader.indexOf("GET / ") != -1) {
              // client.println("<!DOCTYPE HTML>");
              // client.println("<html>");
              // client.println("<head>");
              // client.println("<title>ESP8266 Web Server</title>");
              // client.println("</head>");
              // client.println("<body>");
              // client.println("<h1>ESP8266 Web Server</h1>");
              // client.println("</body>");
              // client.println("</html>");

              String html = "<!DOCTYPE html>\r\n<html lang=\"en\">\r\n\r\n<head>\r\n  <meta charset=\"UTF-8\">\r\n  <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\r\n  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n  <title>ESP32 | Remote Home Control System</title>\r\n</head>\r\n\r\n<body>\r\n  <button>Toggle LED</button>\r\n</body>\r\n\r\n<script defer>\r\n  const button = document.querySelector(\"button\");\r\n  button.addEventListener(\"click\", () => {\r\n    fetch(\"/led/toggle\").then(response => response.json()).then(data => {\r\n      console.log(data);\r\n    }).catch(e => {\r\n      console.log(e);\r\n    });\r\n  });\r\n</script>\r\n\r\n</html>\r\n";
              client.println(html);
            }

            else if (clientHeader.indexOf("GET /led/toggle") != -1) {
              TEST_LED_STATE = !TEST_LED_STATE;
              digitalWrite(TEST_LED_PIN, TEST_LED_STATE);
              client.printf("{\"state\": %s }", TEST_LED_STATE ? "true" : "false");
              client.println();
            }

            break;
          }
          else {
            currentLine = "";
          }
        }
        else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    clientHeader = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

void setupWifi(const char *ssid, const char *password, wifi_mode_t wifiMode) {
  WiFi.mode(wifiMode);
  WiFi.begin(ssid, password);
  Serial.printf("Connecting to %s...\n", SSID);

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
