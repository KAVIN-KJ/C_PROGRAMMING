#include<Arduino.h>

int count1 = 0;
int count2  = 0;

TaskHandle_t task1_handler, task2_handler;

void task1(void* parameters){
  for(;;){
    Serial.print("Task 1 Counter ");
    Serial.println(count1++);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void task2(void* parameters){
  for(;;){ 
    // if(count1>10){
    //   vTaskSuspend(task1_handler);
    // }
    // if(count2>20){
    //   vTaskResume(task1_handler);
    // }
    Serial.print("Task 2 Counter ");
    Serial.println(count2++);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  xTaskCreate(
     task1, // Function name
     "Task 1", 
     // task name
     1000, 
     // stack size - includes the considerations of 
     // local variables used and the overall function size
     NULL, 
     // task (function) parameters usually passed as
     // pointers to structs
     1, 
     // task priority
     &task1_handler 
     /*
      task handle - acts as an accespoint to the task
      so that other external tasks or events can control the 
      behavior of the current task like 
          * suspend
          * resume
          * terminate etc,..
     */ 
  );

  xTaskCreate(
     task2, // Function name
     "Task 2", // task name
     1000, // stack size
     NULL, // task parameters
     1, // task priority
     &task2_handler // task handle
  );
}

void loop() {
  if(Serial.available()){
    String input = Serial.readStringUntil('\n');
    int val = input.toInt();
    switch(val){
      case 1 : 
        if(eTaskGetState(task1_handler)==eSuspended)
          vTaskResume(task1_handler);
        else
          vTaskSuspend(task1_handler);
        break;
      case 2 :
        if(eTaskGetState(task2_handler)==eSuspended)
          vTaskResume(task2_handler);
        else
          vTaskSuspend(task2_handler);
        break;
      case 3 : 
          vTaskSuspend(task1_handler);
          vTaskSuspend(task2_handler);
        break;
      default:
        vTaskResume(task1_handler);
        vTaskResume(task2_handler);
    }
  }
}
