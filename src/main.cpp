#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "WebSocketClient.h"

OneWire oneWire(14);
DallasTemperature sensors(&oneWire);
char message[160] = "";

const char *SSID = "wifi_name";
const char *PASS = "wifi_password";
const char *WS_SERVER = "192.168.1.2"; // Change this
const int WS_PORT = 80; // Change this

WebSocketClient ws(false);

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
  pinMode(D6, INPUT);
}

void loop()
{
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  sprintf(message, "{\"temp\": \"%f\", \"fire\": %s}", temperatureC, digitalRead(D6) ? "true" : "false");
  Serial.println(message);

  if (!ws.isConnected())
  {
    ws.connect(WS_SERVER, "/ws?a=123&t=listener", WS_PORT);
  }
  else
  {
    ws.send(message);
    String msg;
    if (ws.getMessage(msg))
    {
      Serial.println(msg);
    }
  }
  
  delay(2000);
}