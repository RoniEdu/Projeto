#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// config Rede
const char *ssid = "PROF.RAFAEL"; // Roteador
const char *password = "";
IPAddress ip(192, 168, 1, 179);     // ip do dispositivo
IPAddress gateway(192, 168, 1, 1);  // gateway
IPAddress subnet(255, 255, 255, 0); // mascara

ESP8266WebServer server(80);
String page;

// porta digital
const int sensor = 4;
const int Led = 5;
unsigned long evento = 0;

void setup()
{

  Serial.begin(9600);

  // habilitar porta
  pinMode(sensor, INPUT_PULLUP);
  pinMode(Led, OUTPUT);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  server.on("/", []()
            {

     page = "<!DOCTYPE html> \
             <html> \
             <head> <meta http-equiv='refresh' content='0' </head> \
             <body> \
             <h1>Sensor to Node MCU Web Server</h1><h3>Data:</h3> <h4></h4> \
             </body>\
             </html>";

    server.send(200, "text/html", page); });
}

void loop()
{

  int output = digitalRead(sensor);
  int led2 = digitalRead(Led);
  if (output == HIGH)
  {
    if (millis() - evento > 25)
    {
      Serial.println("Reconheci o som");
      Serial.println(millis());
      Serial.println(evento);
      Serial.println(millis() - evento);
      digitalWrite(Led, LOW);
      if(led2 == LOW)
        digitalWrite(Led, HIGH);
    }
    evento = millis();
  }
  server.handleClient();
}