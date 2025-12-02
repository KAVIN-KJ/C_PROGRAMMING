#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <time.h>

// WiFi credentials
const char* ssid = "KJKR";
const char* password = "12345678";

// HTTPS server endpoint
const char* serverUrl = "https://simply-genuine-llama.ngrok-free.app/log";

// LogEntry structure
struct LogEntry {
  char uid[16];
  char location[64];
  char timestamp[32];
};

// FreeRTOS queue
QueueHandle_t logQueue;

// Get timestamp in "YYYY-MM-DD HH:MM:SS"
String getTimestamp() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return "1970-01-01 00:00:00";
  char buf[20];
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(buf);
}

// Background HTTP task
void httpTask(void* pvParameters) {
  LogEntry entry;
  WiFiClientSecure client;
  client.setInsecure();  // skip SSL verification for ngrok testing

  for (;;) {
    if (xQueueReceive(logQueue, &entry, portMAX_DELAY)) {
      HTTPClient http;
      http.begin(client, serverUrl);  // HTTPS
      http.addHeader("Content-Type", "application/json");

      String body = "{ \"uid\": \"" + String(entry.uid) +
                    "\", \"location\": \"" + String(entry.location) +
                    "\", \"timestamp\": \"" + String(entry.timestamp) + "\" }";

      Serial.println("Sending: " + body);
      http.POST(body); // fire-and-forget
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
  Serial.println("\nWiFi connected");

  // Init NTP
  configTime(19800, 0, "pool.ntp.org");

  // Create queue
  logQueue = xQueueCreate(10, sizeof(LogEntry));
  if (!logQueue) {
    Serial.println("Queue creation failed");
    while (1);
  }

  // Start HTTP task
  xTaskCreate(httpTask, "HTTP Task", 8192, NULL, 1, NULL);
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    int index = c - '1';

    const char* uids[] = {"UID123A","UID456B","UID789C","UID321D","UID654E"};
    const char* locations[] = {"North Gate","South Parking","Admin Block","Canteen","Orchids Apartment"};

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
