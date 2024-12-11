#include "ESP32_security_system.hpp"

String header;

void SendHTMLPage(WiFiClient client){
  
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
  client.println(".button2 {background-color: #555555;}</style>");
  client.println("<script>");
  client.println("function sendAction(action) {");
  client.println("  fetch('/' + action).then(response => {");
  client.println("    if (response.ok) {");
  client.println("      response.text().then(data => {");
  client.println("        updatePicture(data);");
  client.println("      });");
  client.println("    } else {");
  client.println("      console.error('Action failed:', action);");
  client.println("    }");
  client.println("  }).catch(error => console.error('Fetch error:', error));");
  client.println("}");
  client.println("function updatePicture(picIndex) {");
  client.println("  const img = document.getElementById('picture');");
  client.println("  img.src = '/picture' + picIndex + '.jpg';");
  client.println("}");
  client.println("</script></head>");
  client.println("<body><h1>ESP32 Web Server</h1>");
  String picNum(currentPictureNumber);
  client.println("<p><img id=\"picture\" src=\"/picture" + picNum + ".jpg\" alt=\"Picture\" style=\"max-width:100%;height:auto;\"></p>");
  client.println("<div style=\"display: flex; align-items: center; justify-content: center;\">");
          
  client.println("<button class=\"button\" onclick=\"sendAction('prev')\">Prev</button>");
  client.println("<button class=\"button\" onclick=\"sendAction('takepicture')\">Take photo</button>");
  client.println("<button class=\"button\" onclick=\"sendAction('next')\">Next</button>");
          
  client.println("</div>");
  client.println("</body></html>");
  client.println();
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
          // Detekce požadavků na konkrétní obrázek
          if (header.indexOf("GET /picture") >= 0) {
            // Extrakce názvu souboru
            int startIdx = header.indexOf("GET /") + 5;
            int endIdx = header.indexOf(" ", startIdx);
            String filename = header.substring(startIdx, endIdx);

            // Sestavení absolutní cesty
            if (!filename.startsWith("/")) {
              filename = "/" + filename; // Přidáme '/' na začátek
            }

            // Načtení souboru z SD karty
            File file = SD_MMC.open(filename.c_str(), FILE_READ);
            if (file) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: image/jpeg");
              client.println("Connection: close");
              client.println();

              // Odeslání obsahu souboru
              while (file.available()) {
                client.write(file.read());
              }
              file.close();
            } else {
              client.println("HTTP/1.1 404 Not Found");
              client.println("Content-Type: text/plain");
              client.println("Connection: close");
              client.println();
              client.println("File not found");
            }
            break;
          }
          else if(header.indexOf("GET /prev") >= 0) { // Opravená podmínka
              currentPictureNumber == 0 ? currentPictureNumber = pictureNumber - 1 : currentPictureNumber--;
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: text/plain");
              client.println("Connection: close");
              client.println();
              client.println(currentPictureNumber); // Odeslání nového indexu
              break; // Přerušení cyklu
          }
          else if(header.indexOf("GET /next") >= 0) { // Opravená podmínka
              currentPictureNumber == pictureNumber - 1 ? currentPictureNumber = 0 : currentPictureNumber++;
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: text/plain");
              client.println("Connection: close");
              client.println();
              client.println(currentPictureNumber); // Odeslání nového indexu
              break; // Přerušení cyklu
          }
          else if(header.indexOf("GET /takepicture") >= 0) { // Opravená podmínka
              takeAPhoto();
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: text/plain");
              client.println("Connection: close");
              client.println();
              currentPictureNumber = pictureNumber -1;
              client.println(currentPictureNumber);
              break;
          }

          // Poskytnutí HTML stránky
          SendHTMLPage(client);

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


