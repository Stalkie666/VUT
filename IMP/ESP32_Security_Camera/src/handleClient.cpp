#include "ESP32_security_system.hpp"

String header;

void SendHTMLPage(WiFiClient client) {
  client.println(F("HTTP/1.1 200 OK"));
  client.println(F("Content-type:text/html"));
  // Keep connection for faster server response, because request for picture will came
  client.println(F("Connection: keep-alive"));
  client.println();
  // HTML header
  client.println(F("<!DOCTYPE html><html>"));
  client.println(F("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"));
  client.println(F("<link rel=\"icon\" href=\"data:,\">"));
  client.println(F("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center; }"));
  client.println(F(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;"));
  client.println(F("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; }"));
  client.println(F(".button2 { background-color: #555555; }"));
  client.println(F("</style></head><body><h1>ESP32 CAM Server</h1>"));
  // HTML body
  String picNum = String(currentPictureNumber);
  client.print(F("<p><img id=\"picture\" src=\"/picture"));
  client.print(picNum);
  client.println(F(".jpg\" alt=\"Picture\" style=\"max-width:100%;height:auto;\"></p>"));


  client.println(F("<div style=\"display: flex; align-items: center; justify-content: center;\">"));
  client.println(F("<button class=\"button\" onclick=\"sendAction('prev')\">Prev</button>"));
  client.println(F("<button class=\"button\" onclick=\"sendAction('takepicture')\">Take photo</button>"));
  client.println(F("<button class=\"button\" onclick=\"sendAction('next')\">Next</button>"));
  client.println(F("</div>"));
  // script for handling actions after clicking on button
  client.println(F("<script>"));
  client.println(F("function sendAction(action) { fetch('/' + action).then(res => res.ok ? res.text().then(updatePicture) : console.error('Action failed:', action)); }"));
  client.println(F("function updatePicture(picIndex) { document.getElementById('picture').src = '/picture' + picIndex + '.jpg'; }"));
  client.println(F("</script></body></html>"));
}

// Function for sending requested pictures
void sendPicture(WiFiClient client, const String& filename) {
  Serial.println("Picture name for send:" + filename);
  File file = SD_MMC.open(filename, FILE_READ);
  if (file) {
    client.println(F("HTTP/1.1 200 OK"));
    client.println(F("Content-Type: image/jpeg"));
    client.println(F("Connection: close"));
    client.println();

    // It is faster when you send data in blocks
    uint8_t buffer[1024];
    size_t bytesRead;
    while ((bytesRead = file.read(buffer, sizeof(buffer))) > 0) {
      client.write(buffer, bytesRead);
    }
    file.close();
  } else {
    client.println(F("HTTP/1.1 404 Not Found"));
    client.println(F("Content-Type: text/plain"));
    client.println(F("Connection: close"));
    client.println();
    client.println(F("File not found"));
  }
}

// Main function for handling clients requests
void handleClient(WiFiClient client) {
  Serial.println("New Client");
  String currentLine = "";

  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      header += c;

      if (c == '\n') {
        if (currentLine.length() == 0) {
          // Recognize clients requests
          if (header.indexOf("GET /picture") >= 0) {
            int startIdx = header.indexOf("GET /") + 5;
            int endIdx = header.indexOf(" ", startIdx);
            String filename = header.substring(startIdx, endIdx);
            if (!filename.startsWith("/")) filename = "/" + filename;
            // sending requested picture
            sendPicture(client, filename);
            break;
          } // on click button PREV
           else if (header.indexOf("GET /prev") >= 0) {
            currentPictureNumber = (currentPictureNumber == 0) ? pictureNumber - 1 : currentPictureNumber - 1;
            client.println(F("HTTP/1.1 200 OK"));
            client.println(F("Content-Type: text/plain"));
            client.println(F("Connection: close"));
            client.println();
            client.println(currentPictureNumber);
            break;
          } // on click button NEXT
          else if (header.indexOf("GET /next") >= 0) {
            currentPictureNumber = (currentPictureNumber == pictureNumber - 1) ? 0 : currentPictureNumber + 1;
            client.println(F("HTTP/1.1 200 OK"));
            client.println(F("Content-Type: text/plain"));
            client.println(F("Connection: close"));
            client.println();
            client.println(currentPictureNumber);
            break;
          } // on click button TAKE A PHOTO or when ESP32 catch motion
          else if (header.indexOf("GET /takepicture") >= 0) {
            takeAPhoto();
            currentPictureNumber = pictureNumber - 1;
            client.println(F("HTTP/1.1 200 OK"));
            client.println(F("Content-Type: text/plain"));
            client.println(F("Connection: close"));
            client.println();
            client.println(currentPictureNumber);
            break;
          }
          // return HTML request
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
