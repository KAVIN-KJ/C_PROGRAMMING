#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>
#include <SPI.h>
#include <MFRC522.h>

// ------------------- WiFi Configuration -------------------
const char* ssid     = "KJKR";
const char* password = "12345678";

// ------------------- Server Endpoint ----------------------
const char* serverUrl = "https://simply-genuine-llama.ngrok-free.app/log";

// ------------------- RFID Configuration -------------------
#define SS_PIN   5     // SDA / SS
#define RST_PIN  22    // RST
#define SCK_PIN  18
#define MISO_PIN 19
#define MOSI_PIN 23

MFRC522 rfid(SS_PIN, RST_PIN);

// ------------------- Log Structure ------------------------
struct LogEntry {
  char uid[16];
  char location[64];
  char timestamp[32];
};

// ------------------- FreeRTOS Queue ------------------------
QueueHandle_t logQueue;

// ------------------- Time Helper --------------------------
String getTimestamp() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "1970-01-01 00:00:00";
  }
  char buffer[20];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(buffer);
}

// ------------------- Background HTTP Task -----------------
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

      Serial.println("Sending: " + body);
      http.POST(body);
      http.end();
    }
  }
}

// ------------------- Setup -------------------
void setup() {
  Serial.begin(115200);
  delay(500);

  // WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");

  // Time (NTP)
  configTime(19800, 0, "pool.ntp.org");
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) Serial.println("Failed to obtain time");

  // SPI + RFID Init
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);
  rfid.PCD_Init();
  Serial.println("RFID reader initialized");

  // Create Queue
  logQueue = xQueueCreate(10, sizeof(LogEntry));
  if (logQueue == NULL) {
    Serial.println("Error creating queue!");
    while (1);
  }

  // Start HTTP Task
  xTaskCreate(httpTask, "HTTP Task", 8192, NULL, 1, NULL);
  Serial.println("System ready. Tap RFID tag...");
}

// ------------------- Loop -------------------
void loop() {
  // Look for new cards
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  // Construct UID string
  String uidStr = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) uidStr += "0";
    uidStr += String(rfid.uid.uidByte[i], HEX);
    if (i < rfid.uid.size - 1) uidStr += ":";
  }
  uidStr.toUpperCase();

  Serial.println("Tag detected: " + uidStr);

  // Map known tags to locations
  const char* location = "SKCT MAIN GATE";

  // Prepare Log Entry
  LogEntry entry;
  strncpy(entry.uid, uidStr.c_str(), sizeof(entry.uid));
  strncpy(entry.location, location, sizeof(entry.location));
  String ts = getTimestamp();
  strncpy(entry.timestamp, ts.c_str(), sizeof(entry.timestamp));

  // Queue the log
  if (xQueueSend(logQueue, &entry, 0) == pdTRUE)
    Serial.println("Queued: " + uidStr + " @ " + ts);
  else
    Serial.println("Queue full — log dropped!");

  // Stop communication
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  delay(1000);
}
