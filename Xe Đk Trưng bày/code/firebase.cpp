#include "firebase.h"
#include <FirebaseESP32.h> // library of firebaseESP32
#include <WiFi.h>
#include "car.h"
#include "carhouse.h"
FirebaseData fbData;
FirebaseAuth auth;
FirebaseConfig config;

// const char* ssid = "Xuong";
// const char* password = "68686868";

const char* ssid = "Nha Tro Vui Ve";
const char* password = "66668888";

#define Firebase_Host  "control-car-29223-default-rtdb.firebaseio.com/"
#define Firebase_Auth  "z2f5DxH8fGz4b8ohlKv1m72KfGsbDnGU8xDTaHns"




int value_of_start = 0 ;
int value_of_forward = 0;
int value_of_back = 0;
int value_of_turn_left = 0;
int value_of_turn_right = 0;
int value_of_speed = 0;

// valiable to firebase from esp32
const char* start = "/start";
const char* forward = "/forward";

const char* back = "/back";
const char* turn_left = "/turn_left";
const char* turn_right = "/turn_right";
const char* speed = "/speed";
// káº¿t ná»‘i Wifi
void firebase :: wifi_connection() // setup
{
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wifi connected!");
}

// káº¿t ná»‘i Ä‘áº¿n firebase
void firebase :: connection_firebase() // setup
{
  config.host = Firebase_Host ;
  config.signer.tokens.legacy_token = Firebase_Auth;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}


void firebase :: read_firebase_start()
{
    Firebase.getInt(fbData, start);
    value_of_start = fbData.intData();
}  
void firebase :: read_firebase_forward()
{
  Firebase.getInt(fbData, forward);
  value_of_forward = fbData.intData();
}    

void firebase :: read_firebase_back()    
{
   Firebase.getInt(fbData, back);
    value_of_back = fbData.intData();
}  
void firebase :: read_firebase_left()
{
  Firebase.getInt(fbData, turn_left);
    value_of_turn_left = fbData.intData();
}

void firebase :: read_firebase_right()
{
  Firebase.getInt(fbData, turn_right);
    value_of_turn_right = fbData.intData();
}  
void firebase :: read_firebase_speed()
{
  Firebase.getInt(fbData, speed);
    value_of_speed = fbData.intData();
}  
     
    
     


void firebase :: control_car()
 {
   if(value_of_start == 1){
    car::start();
   }
   else car ::end();
    if(value_of_forward == 1) car :: forward(value_of_speed);
     if(value_of_back == 1) car :: back(value_of_speed);
     if(value_of_turn_left == 1) car :: turn_left_rotation(value_of_speed);
     if(value_of_turn_right == 1) car :: turn_right_rotation(value_of_speed);   
}
void firebase :: control_carhouse()
{
  
  
    if(value_of_forward == 1) 
    {
      carhouse :: motor_1_Tien(value_of_speed);
       carhouse :: motor_2_Tien(value_of_speed);
       Serial.println("xe tien");
    }
     if(value_of_back == 1) 
    {
      carhouse :: motor_1_Lui(value_of_speed);
      carhouse :: motor_2_Lui(value_of_speed);
      Serial.println("xe lui");
    }
     if(value_of_turn_left == 1) 
    {
      carhouse :: motor_1_Tien(value_of_speed);
      carhouse :: motor_2_Lui(value_of_speed);
      Serial.println("xe quay trai");
    }
     if(value_of_turn_right == 1) 
    {
      carhouse :: motor_1_Lui(value_of_speed);
      carhouse :: motor_2_Tien(value_of_speed);
      Serial.println("xe quay phai");
    }
}

