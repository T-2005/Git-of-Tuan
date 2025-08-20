    /*
  TB6612FNG-Dual-Driver
  made on 28 oct 2020
  by Amir Mohammad Shojaee @ Electropeak
  Home

*/

#include "car.h"
#include "firebase.h"
#include "carhouse.h"

void setup() {
  Serial.begin(9600);
  //car::init();
  carhouse::init_house();
  firebase::wifi_connection();
  firebase :: connection_firebase();
  
  //car::start();
}
 
void loop() {
  firebase :: read_firebase_speed();
firebase :: read_firebase_start();
        firebase :: read_firebase_forward();
firebase :: read_firebase_back();
firebase :: read_firebase_right();
firebase :: read_firebase_left();

  Serial.print("gia tri start: ");
  Serial.println(value_of_start);
  Serial.print("gia tri forward: ");
  Serial.println(value_of_forward);
  Serial.print("gia tri speed: ");
  Serial.println(value_of_speed);
  Serial.print("gia tri back: ");
  Serial.println(value_of_back);

  
      while(value_of_start == 0 && value_of_forward == 1 )
      {
        firebase :: read_firebase_start();
        firebase :: read_firebase_forward();
        carhouse :: motor_1_Tien(value_of_speed);
       carhouse :: motor_2_Tien(value_of_speed);
       Serial.println("xe tien");
      }
      while(value_of_start == 0 && value_of_back == 1 )
      {firebase :: read_firebase_start();
      firebase :: read_firebase_back();
         carhouse :: motor_1_Lui(value_of_speed);
      carhouse :: motor_2_Lui(value_of_speed);
      Serial.println("xe lui");
      }
      while(value_of_start == 0 && value_of_turn_left == 1 )
      {firebase :: read_firebase_start();
      firebase :: read_firebase_left();
        carhouse :: motor_1_Tien(value_of_speed);
      carhouse :: motor_2_Lui(value_of_speed);
      Serial.println("xe quay trai");
      }
    while(value_of_start == 0 && value_of_turn_right == 1 )
      {firebase :: read_firebase_start();
      firebase :: read_firebase_right();
       carhouse :: motor_1_Lui(value_of_speed);
      carhouse :: motor_2_Tien(value_of_speed);
      Serial.println("xe quay phai");
      }
 
  

}
