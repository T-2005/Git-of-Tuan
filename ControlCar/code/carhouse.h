#ifndef carhouse__H
#define carhouse__H

extern int IN1;
extern int IN2;
extern int IN3;
extern int IN4;
extern int MAX_sp; //từ 0-255;
extern int MIN_sp;

class carhouse{
  public :
  static void init_house();
  static void stop();
  static void motor_trai_Tien(int& sp);
  static void motor_phai_Tien(int& sp);
  static void motor_trai_Lui(int& sp);
  static void motor_phai_Lui(int& sp);

};
#endif