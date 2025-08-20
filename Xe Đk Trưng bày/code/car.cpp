#include "car.h"
#include "firebase.h"
int PWM1 = 25;
int AIN1 = 13;
int AIN2 = 14;
int ENA = 33;

int PWM2 = 26;
int BIN1 = 27;
int BIN2 = 12;  

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
void car :: forward(int& v) // di thang
{
  digitalWrite(AIN1,HIGH); 
  digitalWrite(AIN2,LOW);
  analogWrite(PWM1,v);

  digitalWrite(BIN1,HIGH); 
  digitalWrite(BIN2,LOW);
  analogWrite(PWM2, v); 
 
}
void car :: back(int& v)
{
  digitalWrite(AIN1,LOW); 
  digitalWrite(AIN2,HIGH);
  analogWrite(PWM1,v);

  digitalWrite(BIN1,LOW); 
  digitalWrite(BIN2,HIGH);
  analogWrite(PWM2,v);
}
void car :: turn_right_rotation(int& v)
{
  digitalWrite(AIN1,HIGH); 
  digitalWrite(AIN2, LOW);
  analogWrite(PWM1,v); 

  digitalWrite(BIN1,LOW); 
  digitalWrite(BIN2,HIGH);
  analogWrite(PWM2,v);
}
void car :: turn_left_rotation(int& v)
{
  digitalWrite(AIN1,LOW); 
  digitalWrite(AIN2, HIGH);
  analogWrite(PWM1,v); 

  digitalWrite(BIN1,HIGH); 
  digitalWrite(BIN2,LOW);
  analogWrite(PWM2,v); 
}

