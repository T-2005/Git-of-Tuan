    /*
  TB6612FNG-Dual-Driver
  made on 28 oct 2020
  by Amir Mohammad Shojaee @ Electropeak
  Home

*/

#include "car.h"


int pot = 500;
void setup() {
  Serial.begin(9600);
  car::init();
  car::start();
}
 
void loop() {
  car::forward(pot);
}
