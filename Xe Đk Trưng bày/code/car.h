#ifndef car_H
#define car_H
#include <Arduino.h>

extern int PWM1 ;
extern int AIN1 ;
extern int AIN2 ;
extern int ENA ;

extern int PWM2 ;
extern int BIN1 ;
extern int BIN2 ;
extern int speed;
class car{
  public:
  static void init();
  static void start();
  static void end();
  static void forward(int& speed);
  static void back(int& speed);
  static void turn_right_rotation(int& speed);
  static void turn_left_rotation(int& speed);
};

#endif
