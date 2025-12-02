#define LED 2   // Built-in LED is usually on GPIO 2

void setup() {
  pinMode(LED, OUTPUT);
}

void loop() {
  digitalWrite(LED, HIGH);  // Turn on
  delay(500);
  digitalWrite(LED, LOW);   // Turn off
  delay(500);
}
