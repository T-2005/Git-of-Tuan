#include <Arduino.h>
#include <MPU6050_light.h>
#include "Wire.h"
#include "_mpu.h"
MPU6050 mpu(Wire);

long angle = 0.0;
unsigned long timer = 0;
float _Gx = 0.0, _Gy = 0.0, _Gz = 0.0;
void _mpu::gyroSetUp()
{
  byte status = mpu.begin();
  Serial.print("MPU6050 status: ");
  Serial.println(status);
  while (status != 0)
  {
  } // stop everything if could not connect to MPU6050

  Serial.println("Calculating offsets, do not move MPU6050");
  delay(1000);
  mpu.calcOffsets(true, true); // gyro and accelero
  Serial.println("Done!\n");
}

void _mpu::PrintAngles()
{
  Serial.print("X: ");
  Serial.print(mpu.getAngleX());
  Serial.print("\tY: ");
  Serial.print(mpu.getAngleY());
  Serial.print("\tZ: ");
  Serial.print(mpu.getAngleZ());
  Serial.print("\n");
}
void _mpu::readMpu()
{
  _Gx = mpu.getAngleX();
  _Gy = mpu.getAngleY();
  _Gz = mpu.getAngleZ();
}
void _mpu::init()
{
  Serial.println("Starting");
  Wire.begin();
  gyroSetUp();
}

void _mpu::Update()
{
  mpu.update();
  if ((millis() - timer) > 1000)
  {
    PrintAngles();
    timer = millis();
  }
}