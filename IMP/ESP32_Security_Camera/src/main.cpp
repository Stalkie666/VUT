#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>

#include "ESP32_security_system.hpp"

const char* ssid = "Stalkie's AP";
const char* password = "tvojemama";

IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

WiFiServer server(80);




void setup() {
  Serial.begin(115200);
  Serial.print("Setting AP (Access Point)…");

  if (!WiFi.softAPConfig(local_IP, gateway, subnet)) {
    Serial.println("Failed to configure static IP");
    return;
  }

  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  server.begin();
}



void loop() {
  WiFiClient client = server.available();
  if (client) {
    handleClient(client); // Obslouží každého klienta v samostatné funkci
  }
}
