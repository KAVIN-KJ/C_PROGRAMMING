#include <WiFi.h>
#include <HTTPClient.h>

// Replace with your network credentials
const char* ssid     = "KJKR";
const char* password = "12345678";

// Example API endpoint (you can replace it with any valid API)
const char* serverName = "https://jsonplaceholder.typicode.com/todos/1";

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Make HTTP GET Request
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    Serial.println("Sending GET request...");
    http.begin(serverName);  // Specify the URL

    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      String payload = http.getString();
      Serial.println("Response:");
      Serial.println(payload);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end(); // Free resources
  } else {
    Serial.println("WiFi Disconnected");
  }
}

void loop() {
  // Nothing for now
}
