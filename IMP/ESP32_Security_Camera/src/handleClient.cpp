#include "ESP32_security_system.hpp"

String header;

// TODO: Smazat counter, byl jen na test
int counter = 0;

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
          if (header.indexOf("GET /plus") >= 0) {
            counter++;
          } else if (header.indexOf("GET /counter") >= 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/plain");
            client.println("Connection: close");
            client.println();
            client.println(counter); // Vrátí hodnotu počítadla
            break;
          }

          // Poskytnutí HTML stránky
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("Connection: close");
          client.println();

          client.println("<!DOCTYPE html><html>");
          client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
          client.println("<link rel=\"icon\" href=\"data:,\">");
          client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
          client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
          client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
          client.println(".button2 {background-color: #555555;}</style></head>");
          client.println("<body><h1>ESP32 Web Server</h1>");
          client.println("<p><button disabled=\"true\" class=\"button\" onclick=\"sendPlusRequest()\">+</button></p>");
          client.println("<p>Cislo: <span id=\"counter\">0</span></p>");
          client.println("<script>");
          client.println("function sendPlusRequest() {");
          client.println("fetch('/plus').then(response => { if (response.ok) updateCounter(); });");
          client.println("}");
          client.println("function updateCounter() {");
          client.println("fetch('/counter').then(response => response.text()).then(data => {");
          client.println("document.getElementById('counter').textContent = data; });");
          client.println("}");
          client.println("document.addEventListener('DOMContentLoaded', updateCounter);");
          client.println("</script>");
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
