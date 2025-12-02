#define LED1 1
#define LED2 0
#define LED3 13
#define del 100

void setup() {
  // put your setup code here, to run once:
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

}

void loop() {
  // BLINKS RX, TX AND BUILT-IN LED one by one in series
  digitalWrite(LED3, HIGH);
  delay(del);
  digitalWrite(LED1, HIGH);
  delay(del);
  digitalWrite(LED2, HIGH);
  delay(del);
  digitalWrite(LED3, LOW);
  delay(del);
  digitalWrite(LED1, LOW);
  delay(del);
  digitalWrite(LED2, LOW);
  delay(del);
}
