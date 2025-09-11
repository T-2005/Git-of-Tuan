
#include "dcMotor.h"
 



const int freq = 5000;
const int resolution = 8;  // PWM 8-bit (0-255)

DCMotor::DCMotor(int pwmL, int in1L, int in2L, int pwmR, int in1R, int in2R) {
    _pwmL = pwmL; _in1L = in1L; _in2L = in2L;
    _pwmR = pwmR; _in1R = in1R; _in2R = in2R;

    // Khởi tạo chân output
    pinMode(_in1L, OUTPUT);
    pinMode(_in2L, OUTPUT);
    pinMode(_in1R, OUTPUT);
    pinMode(_in2R, OUTPUT);

    // Khởi tạo kênh PWM
    _pwmChannelL = 0;
    _pwmChannelR = 1;
    ledcSetup(_pwmChannelL, freq, resolution);
    ledcAttachPin(_pwmL, _pwmChannelL);
    ledcSetup(_pwmChannelR, freq, resolution);
    ledcAttachPin(_pwmR, _pwmChannelR);
}

void DCMotor::motorControl(int pwmChannel, int in1, int in2, int speed, bool forward) {
    if (speed == 0) {
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
    } else if (forward) {
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
    } else {
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
    }
    ledcWrite(pwmChannel, speed);
}

void DCMotor::drive(int speedL, int speedR, bool forward) {
    motorControl(_pwmChannelL, _in1L, _in2L, speedL, forward);
    motorControl(_pwmChannelR, _in1R, _in2R, speedR, forward);
}

void DCMotor::stop() {
    motorControl(_pwmChannelL, _in1L, _in2L, 0, true);
    motorControl(_pwmChannelR, _in1R, _in2R, 0, true);
}

void DCMotor::forward(int speed) {
    drive(speed, speed, true);
}

void DCMotor::backward(int speed) {
    drive(speed, speed, false);
}

void DCMotor::turnLeft(int speed) {
    motorControl(_pwmChannelL, _in1L, _in2L, speed/2, false);
    motorControl(_pwmChannelR, _in1R, _in2R, speed, true);
}

void DCMotor::turnRight(int speed) {
    motorControl(_pwmChannelL, _in1L, _in2L, speed, true);
    motorControl(_pwmChannelR, _in1R, _in2R, speed/2, false);
}
