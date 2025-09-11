#include <Arduino.h>
#include "dcMotor.h"
#include "tuan.h"
int pwmL = 25;
int in1L = 13;
int in1R = 14;
int ENA = 33;

int pwmR = 26;
int in2L = 27;
int in2R = 12; 

DCMotor robot(PWML, in1L, in2L, PWMR, in1R, in2R);

void setup() {
    Serial.begin(115200);
    
}

void loop() {
    
 robot.forward(200);
    delay(2000);
  

 

   robot.backward(150);
    delay(2000);
  
  
  

    robot.turnRight(180);
    delay(1000);
  
    robot.turnLeft(180);
    delay(1000);
  
  
   
    robot.stop();
    delay(1000);
   

 
    
}
