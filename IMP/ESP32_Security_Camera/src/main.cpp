#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>

#include "ESP32_security_system.hpp"

#define interuptPinForMe GPIO_NUM_13

// config data for WiFi
const char* ssid = "xhamad03's AP";
const char* password = "123456";

// config data for server
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

  // if camera setup failed, end setup
  if( setupCamera() ) return;
  // if SD card setup failed, end setup
  if( setupSDcard() ) return;
}



void loop() {
  // just waiting for client (ESP32 with PIR senzor or user's devices)
  WiFiClient client = server.available();
  if (client) {
    handleClient(client);
  }
}
