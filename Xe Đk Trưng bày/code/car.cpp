#include "car.h"

int PWM1 = 25;
int AIN1 = 13;
int AIN2 = 14;
int ENA = 33;

int PWM2 = 26;
int BIN1 = 12;
int BIN2 = 27;  
int speed;
void car :: init()
{
  pinMode(PWM1, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
}
void car:: start() // cho xe hoat dong
{
  digitalWrite(ENA, HIGH);
}
void car:: end() // xe ko hoat dong
{
  digitalWrite(ENA, LOW);
}
void car :: forward(int& speed) // di thang
{
  digitalWrite(AIN1,HIGH); 
  digitalWrite(AIN2,LOW);
   analogWrite(PWM1,speed); 
  digitalWrite(BIN1,HIGH); 
  digitalWrite(BIN2,LOW);

  analogWrite(PWM2, speed); 
 
}
void car :: back(int& speed)
{
  digitalWrite(AIN1,LOW); 
  digitalWrite(AIN2,HIGH);
   analogWrite(PWM1,speed);

  digitalWrite(BIN1,LOW); 
  digitalWrite(BIN2,HIGH);

  analogWrite(PWM2,speed);
}
void car :: turn_right_rotation(int& speed)
{
  digitalWrite(AIN1,HIGH); 
  digitalWrite(AIN2, LOW);
  analogWrite(PWM1,speed); 
  digitalWrite(BIN1,LOW); 
  digitalWrite(BIN2,HIGH);
  analogWrite(PWM1,speed); 
  analogWrite(PWM2,speed);
}
void car :: turn_left_rotation(int& speed)
{
  digitalWrite(AIN1,LOW); 
  digitalWrite(AIN2, HIGH);
  analogWrite(PWM1,speed); 
  digitalWrite(BIN1,HIGH); 
  digitalWrite(BIN2,LOW);
  analogWrite(PWM1,speed); 
  analogWrite(PWM2,speed); 
}
void car :: wheel_A(int value_AIN1, int )
