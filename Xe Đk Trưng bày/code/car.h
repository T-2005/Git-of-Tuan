#ifndef car_H
#define car_H
#include <Arduino.h>
#include "firebase.h"
extern int PWM1 ;
extern int AIN1 ;
extern int AIN2 ;
extern int ENA ;

extern int PWM2 ;
extern int BIN1 ;
extern int BIN2 ;

class car{
  public:
  static void init();
  static void start();
  static void end();
  static void forward(int& v);
  static void back(int& v);
  static void turn_right_rotation(int& v);
  static void turn_left_rotation(int& v);
};

#endif
