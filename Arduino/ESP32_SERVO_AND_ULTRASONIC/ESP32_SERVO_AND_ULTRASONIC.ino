#include <ESP32Servo.h>

#define TRIG_PIN 5
#define ECHO_PIN 18
#define SERVO_PIN 19

Servo myServo;

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  myServo.attach(SERVO_PIN, 500, 2400); // ESP32 servo pulse width in microseconds
  myServo.write(0); // Start position
}

long readUltrasonicDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH);
  long distance = duration * 0.034 / 2; // cm
  return distance;
}

void loop() {
  long distance = readUltrasonicDistance();
  Serial.println(distance);
  
  if (distance <= 10) { // Object detected within 10cm
    int currentPos = myServo.read();
    int newPos = currentPos + 30;
    if(newPos > 180) newPos = 0; // Limit servo rotation
    myServo.write(newPos);
    delay(500); // Give servo time to move
  }
  delay(200); // Sensor reading delay
}
