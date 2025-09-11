#include <Arduino.h>
#include "dcMotor.h"
#include "_mpu.h"
// #define PWML 14
// #define in1L 12
// #define in2L 13
// #define PWMR 25
// #define in1R 26
// #define in2R 27

#define PWML 13
#define in1L 14
#define in2L 12
#define PWMR 32
#define in1R 33
#define in2R 25
DCMotor robot(PWML, in1L, in2L, PWMR, in1R, in2R);

float Kp = 2;
float Ki = 0.1;
float Kd = 0.1;
float setpoint = 0.0; // set goc 0 do
uint32_t lastTimePID = 0.0;
float PID_outPut = 0.0;

int speed = 250;
void setup() 
{
    Serial.begin(115200);
   // _mpu::init();
}

float PID_value()
{
  float readValue = 0.0;
   _mpu::Update();
   _mpu::readMpu();
  readValue = _Gx; Serial.print("Gia tri _Gx: "); Serial.println(_Gx);
/* xử lí giá trị x, y, z -> đưa về trạng thái ban đầu là 0.0
   tìm readValue
*/

  float sp = robot.PID_control(setpoint, readValue, lastTimePID, Kp, Ki, Kd);
  return sp;
}

void can_bang()
{
  float PID = PID_value();
  int lsp = (int) (speed - PID);
  int rsp = (int) (speed + PID);

  if (lsp > 250) {
    lsp = 250;
  }
  if (lsp < 0) {
    lsp = 0;
  }
  if (rsp > 250) {
    rsp = 250;
  }
  if (rsp < 0) {
    rsp = 0;
  }
  Serial.print("lsp = "); Serial.println(lsp);
  Serial.print("rsp = "); Serial.println(rsp);
  robot.motion(lsp, rsp);
}

void loop() {
   // _mpu::Update();
//   can_bang();
//    _mpu::readMpu();
     
    robot.motion(250, 250);
  
}
