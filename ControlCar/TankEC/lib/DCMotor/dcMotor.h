#ifndef DCMOTOR_H
#define DCMOTOR_H

#include <Arduino.h>

class DCMotor {
public:
    DCMotor(int pwmL, int in1L, int in2L, int pwmR, int in1R, int in2R);
    void drive(int speedL, int speedR);
    void stop();
    void motion(int speedL, int speedR);
    
    int _pwmL, _in1L, _in2L;
    int _pwmR, _in1R, _in2R;
    int _pwmChannelL, _pwmChannelR;
    void motorControl(int pwmChannel, int in1, int in2, int speed);
    float PID_control(float setValue, float readValue, uint32_t lastTimePID, float Kp, float Ki, float Kd); // readValue lÃ  thÃ´ng sá»‘ Ä‘á»c Ä‘Æ°á»£c tá»« ir 3 - > 12; 15, 16
    
    private : 
    float Kp, Ki, Kd;
};

#endif
