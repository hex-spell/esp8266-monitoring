#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>

OneWire oneWire(14);
DallasTemperature sensors(&oneWire);
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
AsyncWebSocketClient *wsClient;
char message[160] = "";

const char* SSID = "wifi_name";
const char* PASS = "wifi_pass";

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  if (type == WS_EVT_CONNECT)
  {
    wsClient = client;
  }
  else if (type == WS_EVT_DISCONNECT)
  {
    wsClient = nullptr;
  }
}

void setup()
{
  sensors.begin();
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASS);
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.printf("WiFi Failed!\n");
    return;
  }

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  // Start webserver
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);
  server.begin();

  // Pins
  pinMode(D6, INPUT);
}

void loop()
{
  // If client is connected ...
  if (wsClient != nullptr && wsClient->canSend())
  {
    sensors.requestTemperatures(); 
    float temperatureC = sensors.getTempCByIndex(0);
    sprintf(message, "Temperature: %f ºC Fire: %s", temperatureC, digitalRead(D6) ? "alarm" : "none");
    wsClient->text(message);
    delay(5000);
  }
  // Wait 10 ms
  delay(10);
}