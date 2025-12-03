#include<Arduino.h>

TaskHandle_t millis_handler;

unsigned long prev = 0;

void millisTask(void* params){
  for(;;){
    vTaskSuspend(NULL);
    unsigned long now = millis();
    if(prev==0){
      prev = millis();
      Serial.println("Calling the function for first time");
      continue;
    }
    Serial.printf("Seconds elapsed since last call : %lu\n",(now-prev)/1000);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  xTaskCreate(
    millisTask,
    "Millis Function",
    1000,
    NULL,
    1,
    &millis_handler
  );
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    String ip = Serial.readStringUntil('\n');
    ip.trim();
    if(ip == "time"){
      vTaskResume(millis_handler);
    }
  }
}
