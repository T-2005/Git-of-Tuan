#ifndef firebase_H
#define firebase_H
#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h> // library of firebaseESP32




// creat valiable
extern FirebaseData fbData;
extern FirebaseAuth auth;
extern FirebaseConfig config;

// Wifi
extern const char* ssid;
extern const char* password;
// Firebase declare
// extern const char* Firebase_Host;
// extern const char* Firebase_Auth;
// biáº¿n há»©ng

extern int value_of_start;
extern int value_of_forward;
extern int value_of_back;
extern int value_of_turn_left;
extern int value_of_turn_right;
extern int value_of_speed;


extern const char* start;
extern const char* forward;
extern const char* back;
extern const char* turn_left;
extern const char* turn_right;
extern const char* speed;
class firebase {
  public:
  static void wifi_connection();
  static void connection_firebase();
  
  static void control_car();
  static void control_carhouse();
  static void read_firebase_start();
  static void read_firebase_forward();
  static void read_firebase_back();
  static void read_firebase_left();
  static void read_firebase_right();
  static void read_firebase_speed();
};


#endif 