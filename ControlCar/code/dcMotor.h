#ifndef DCMOTOR_H
#define DCMOTOR_H

#include <Arduino.h>

class DCMotor {
public:
    DCMotor(int pwmL, int in1L, int in2L, int pwmR, int in1R, int in2R);
    void drive(int speedL, int speedR, bool forward);
    void stop();
    void forward(int speed);
    void backward(int speed);
    void turnLeft(int speed);
    void turnRight(int speed);
    int _pwmL, _in1L, _in2L;
    int _pwmR, _in1R, _in2R;
    int _pwmChannelL, _pwmChannelR;
    void motorControl(int pwmChannel, int in1, int in2, int speed, bool forward);
};

#endif
