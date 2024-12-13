#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define PIR_CIDLO GPIO_NUM_13

// config info for wifi connection
const char* ssid = "xhamad03's AP";
const char* password = "123456";

// only command, which is needed for taking photo
const char* serverNamePhoto = "http://192.168.4.1/takepicture";

volatile bool motionDetected = false;

// interupt handler
void motionDetection(){
  motionDetected = true;
}


void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid,password);
  // just debug info
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  // set interupt
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

