#include <SPI.h>
#include <MFRC522.h>

// Pin definitions (match wiring above)
#define SS_PIN   5    // SDA / SS
#define RST_PIN 22    // RST

MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  SPI.begin(18, 19, 23); // SCK, MISO, MOSI (explicit VSPI pins)
  rfid.PCD_Init();
  Serial.println("RFID reader initialized");
}

void loop() {
  // Look for new cards
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  // Print UID
  // Serial.print("UID: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) Serial.print("0");
    Serial.print(rfid.uid.uidByte[i], HEX);
    Serial.print(i < rfid.uid.size - 1 ? ":" : "");
  }
  Serial.println();

  // Example: Get type
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.print("PICC type: ");
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Halt PICC and stop encryption
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  delay(500);
}
