#ifndef __ESP32_security_system_hpp__
#define __ESP32_security_system_hpp__


#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>

// Global variables
extern String header;

// functions
void handleClient(WiFiClient client);

#endif //__ESP32_security_system_hpp__