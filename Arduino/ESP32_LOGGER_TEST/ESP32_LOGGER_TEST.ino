#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>

// WiFi credentials
const char* ssid     = "KJKR";
const char* password = "12345678";

// Server endpoint
const char* serverUrl = "https://simply-genuine-llama.ngrok-free.app/log";

// Struct to represent a log entry (use fixed-size char arrays)
struct LogEntry {
  char uid[16];
  char location[64];
  char timestamp[32];
};

// FreeRTOS queue
QueueHandle_t logQueue;

// Function to format local time
String getTimestamp() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "1970-01-01 00:00:00";
  }
  char buffer[20];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(buffer);
}

// Background task to send logs
void httpTask(void *pvParameters) {
  LogEntry entry;
  for (;;) {
    if (xQueueReceive(logQueue, &entry, portMAX_DELAY)) {
      HTTPClient http;
      http.begin(serverUrl);
      http.addHeader("Content-Type", "application/json");

      String body = "{ \"uid\": \"" + String(entry.uid) +
                    "\", \"location\": \"" + String(entry.location) +
                    "\", \"timestamp\": \"" + String(entry.timestamp) + "\" }";

      Serial.println(body);
      http.POST(body);  // Fire-and-forget
      http.end();
    }
  }
}

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  // Init NTP
  configTime(19800, 0, "pool.ntp.org"); // GMT+5:30 IST
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
  }

  // Create queue
  logQueue = xQueueCreate(10, sizeof(LogEntry));
  if (logQueue == NULL) {
    Serial.println("Error creating queue");
    while (1);
  }

  // Start HTTP task
  xTaskCreate(httpTask, "HTTP Task", 8192, NULL, 1, NULL);
}

void loop() {
  // Simulate receiving serial commands "1", "2", etc.
  if (Serial.available()) {
    char c = Serial.read();
    int index = c - '1';

    const char* uids[] = {"UID123A", "UID456B", "UID789C", "UID321D", "UID654E"};
    const char* locations[] = {"North Gate", "South Parking", "Admin Block", "Canteen", "Orchids Apartment"};

    if (index >= 0 && index < 5) {
      LogEntry entry;
      strncpy(entry.uid, uids[index], sizeof(entry.uid));
      strncpy(entry.location, locations[index], sizeof(entry.location));
      String ts = getTimestamp();
      strncpy(entry.timestamp, ts.c_str(), sizeof(entry.timestamp));

      if (xQueueSend(logQueue, &entry, 0) == pdTRUE) {
        Serial.println("Queued: " + String(entry.uid) + " @ " + String(entry.timestamp));
      } else {
        Serial.println("Queue full, log dropped!");
      }
    }
  }
}
