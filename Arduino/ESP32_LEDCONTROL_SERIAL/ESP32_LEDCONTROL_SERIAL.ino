#define LED 2
 void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    char c = Serial.read();
    if(c=='1'){
      digitalWrite(LED, HIGH);
    }
    else if(c=='0'){
      digitalWrite(LED, LOW);
    }
    else{
      Serial.println("Invalid Command");
    }
  }

}
