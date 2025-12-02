#include <WiFi.h>
#include "time.h"

// Replace with your WiFi credentials
const char* ssid       = "KJKR";
const char* password   = "12345678";

// NTP server and timezone offset
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;  // GMT+5:30 for IST (India Standard Time)
const int   daylightOffset_sec = 0;

void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S"); // e.g., Monday, October 4 2025 14:35:00
}

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");

  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  
  // Print time once connected
  printLocalTime();
}

void loop() {
  // Print time every 10 seconds
  printLocalTime();
  delay(1000);
}
