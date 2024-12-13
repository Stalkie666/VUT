#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define PIR_CIDLO GPIO_NUM_13

const char* ssid = "xhamad03's AP";
const char* password = "123456";

const char* serverNamePhoto = "http://192.168.4.1/takepicture";

volatile bool motionDetected = false;

void motionDetection(){
  motionDetected = true;
}


void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid,password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  pinMode(PIR_CIDLO,INPUT);
  attachInterrupt(PIR_CIDLO,motionDetection,RISING);
}



String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "--"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}



void loop() {
  if( WiFi.status() == WL_CONNECTED && motionDetected){
    String retVal = httpGETRequest(serverNamePhoto);
    Serial.println("Fotka no." + retVal + " porizena");
    motionDetected = false;
  }
}

