#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>

const char* ssid = "Stalkie's AP";
const char* password = "tvojemama";

IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

WiFiServer server(80);
String header;

// TODO: Smazat counter, byl jen na test
int counter = 0;

void setup() {
  Serial.begin(9600);
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

void handleClient(WiFiClient client) {
  Serial.println("New Client");
  String currentLine = "";

  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      Serial.write(c);
      header += c;

      if (c == '\n') {
        if (currentLine.length() == 0) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("Connection: close");
          client.println();

          if (header.indexOf("GET /plus") >= 0) {
            Serial.println("Plus push");
            counter++;
          }

          client.println("<!DOCTYPE html><html>");
          client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
          client.println("<link rel=\"icon\" href=\"data:,\">");
          client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
          client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
          client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
          client.println(".button2 {background-color: #555555;}</style></head>");
          client.println("<body><h1>ESP32 Web Server</h1>");
          client.println("<p><a href=\"/plus\"><button class=\"button\">+</button></a></p>");
          String str(counter);
          client.println("<p>Cislo: " + str + "</p>");
          client.println("</body></html>");
          client.println();
          break;
        } else {
          currentLine = "";
        }
      } else if (c != '\r') {
        currentLine += c;
      }
    }
  }

  header = "";
  client.stop();
  Serial.println("Client disconnected.");
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    handleClient(client); // Obslouží každého klienta v samostatné funkci
  }
}
