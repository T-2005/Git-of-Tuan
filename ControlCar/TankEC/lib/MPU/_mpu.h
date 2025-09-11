#ifndef _mpu_H
#define _mpu_H
extern float _Gx, _Gy, _Gz;

class _mpu{
   public:
   static void gyroSetUp();
   static void PrintAngles();
   static void init();
   static void Update();
   static void readMpu();
};

#endif