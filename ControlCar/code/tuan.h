#ifndef tuan__H
#define tuan__H

extern int Ax;
extern int Ay;
extern int Az;

extern float Kd;
extern float Kp;
extern int setvalue;
extern int preError;
extern float out_PD;
class tuan{
  public :
  static void set_read();
  static void read();
  static void PD();
};

#endif