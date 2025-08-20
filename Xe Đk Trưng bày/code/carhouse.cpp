#include "carhouse.h"
#include <Arduino.h>
int IN1	= 0;
int IN2	= 4;
int IN3	= 33;
int IN4	= 32;
int MAX_sp = 500; //từ 0-255;
int MIN_sp = 0;
void carhouse :: init_house()
{
	pinMode(IN1, OUTPUT);
	pinMode(IN2, OUTPUT);
	pinMode(IN3, OUTPUT);
	pinMode(IN4, OUTPUT);
}
 
void carhouse :: stop()
{
	digitalWrite(IN1, LOW);
	digitalWrite(IN2, LOW);
	digitalWrite(IN3, LOW);
	digitalWrite(IN4, LOW);
}
 
void  carhouse :: motor_1_Tien(int& sp) { //sp: từ 0 - MAX_sp
	sp = constrain(sp, MIN_sp, MAX_sp); // ví dụ sp > max_sp => sp = max_sp và ngược lại 
	analogWrite(IN1, sp);
	digitalWrite(IN2, LOW);// chân này không có PWM
	
	Serial.println("Banh 1 T");
}
 
void  carhouse :: motor_1_Lui(int& sp) {
	sp = constrain(sp, MIN_sp, MAX_sp);
	digitalWrite(IN1, LOW);// chân này không có PWM
	analogWrite(IN2, sp);
	Serial.println("Banh 1 L");
}
 
void  carhouse :: motor_2_Tien(int& sp) { //sp: từ 0 - MAX_sp
	sp = constrain(sp, MIN_sp, MAX_sp);
		analogWrite(IN3, sp);
	digitalWrite(IN4, LOW);// chân này không có PWM

	
	Serial.println("Banh 2 T");
}
 
void  carhouse :: motor_2_Lui(int& sp) {
	sp = constrain(sp, MIN_sp, MAX_sp);
		digitalWrite(IN3, LOW);// chân này không có PWM
	analogWrite(IN4,  sp);

	Serial.println("Banh 2 L");
}
 
