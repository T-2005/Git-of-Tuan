#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "esp_system.h"
#define BT1 32
#define BT2 33
#define BT3 25
#define BT4 5
LiquidCrystal_I2C lcd1(0x27, 16, 2); // LCD phát
LiquidCrystal_I2C lcd2(0x26, 16, 2); // LCD nhận
char a[33]; // 32 ký tự + '\0'

float p = 0.300; // Xác suất lỗi bit
float BER = 0.000;
int bitErr = 0;
void setup() {
  lcd1.init();
  lcd1.backlight();
  lcd1.clear();
  lcd1.print("Waiting...");

  lcd2.init();
  lcd2.backlight();
  lcd2.clear();
  lcd2.print("Waiting...");

  Serial.begin(115200);                      
  Serial2.begin(115200, SERIAL_8N1, 16, 17);   // UART2: RX=16, TX=17 -> kenh phat
  Serial1.begin(115200, SERIAL_8N1, 18, 19); // UART1: RX = 18, TX = 19 -> kenh thu
  while (Serial2.available()) Serial2.read();  // clear buffer
  
  Serial.println("ESP32 Ready - LE NGOC TUAN");

  pinMode(BT1, INPUT_PULLUP);
  pinMode(BT2, INPUT_PULLUP);
  pinMode(BT3, INPUT_PULLUP);
  pinMode(BT4, INPUT_PULLUP);
  Serial1.print("P=");
  Serial1.print(p);
}

void loop() {
  bitErr = 0;
  // --- GỬI DỮ LIỆU từ PC -> STM32 ---
  //if (Serial.available()) 
  // Serial.println(_button(BT1));
  // if(_button(BT1))
  {
    button2();
    button3();
  }
    
  // --- NHẬN DỮ LIỆU từ STM32 -> LCD ---
  button();
}

void _lcd_display(void) {
  char t = '1';
    // if (t.length() > 0) {
      Serial2.print(t);
      Serial2.write('\n');
      // Serial1.print(t);
      // Serial1.print('\n');
      Serial.print("Da gui: ");
      Serial.println(t);
  // đọc data gửi từ kênh phát 
  unsigned long startTime = millis();
  while (!Serial2.available() && millis() - startTime < 1000) {
    delay(10);  // chờ tối đa 1 giây
  }

  if (Serial2.available()) {
    int n = Serial2.readBytes(a, 32);
    a[n] = '\0'; // kết thúc chuỗi an toàn

    lcd1.clear();
    lcd2.clear();
    Serial.print("Nhan duoc: ");
    Serial.println(a);
    // In lên LCD1 (chuỗi gốc)
    lcd1.setCursor(0, 0);
    for (int i = 0; i < 16 && a[i] != '\0'; i++)
      lcd1.print(a[i]);
    
    if (n > 16) {
      lcd1.setCursor(0, 1);
      for (int i = 16; i < n && a[i] != '\0'; i++)
        lcd1.print(a[i]);
    }

    // Mô phỏng lỗi bit và in lên LCD2
    lcd2.setCursor(0, 0);
    for (int i = 0; i < n; i++) {
      float t = random(0, 1000) / 1000.0; // giá trị ngẫu nhiên [0,1)
      if (t < p) {
        // Đảo bit nếu gặp lỗi
        if (a[i] == '0') a[i] = '1';
        else if (a[i] == '1') a[i] = '0';
        bitErr++;
      }

      // In ra LCD2
      if (i == 16) lcd2.setCursor(0, 1);
      lcd2.print(a[i]);
     // delay(100); // chờ hiển thị
    }
    Serial.print("Gia tri p: "); Serial.println(p);
    Serial.print("Bit BSC:   ");
    Serial.println(a);
    BER = (float)bitErr / n;
  
    Serial1.print("BER=");
    Serial1.println(BER, 4);
    Serial.print("Ti so BER: "); Serial.println(BER, 4);
   
    // Xóa buffer UART tránh đọc rác
    while (Serial2.available()) Serial2.read();
  }
}

void button() {
  static bool lastState = LOW;  // nhớ trạng thái nút lần trước
  bool currentState = digitalRead(BT1);

  if (lastState == LOW && currentState == HIGH) {  // phát hiện nhấn xuống
    delay(20);  // chống dội
    if (digitalRead(BT1) == HIGH) {
      _lcd_display();   // Gửi dữ liệu ngay khi nhấn
     // Serial1.print("Gia tri p: "); Serial1.print(p); Serial1.write('\n');
    }
  }

  lastState = currentState;  // cập nhật trạng thái
}

 void button2()
{
  static bool lastState2 = LOW;  // nhớ trạng thái nút lần trước
  bool currentState2 = digitalRead(BT2);

  if (lastState2 == LOW && currentState2 == HIGH) {  // phát hiện nhấn xuống
    delay(20);  // chống dội
    if (digitalRead(BT2) == HIGH && p <= 1.0) {
      p = p + 0.1;
      Serial1.print("P=");
      Serial1.println(p, 3);
    }

  }

  lastState2 = currentState2;  // cập nhật trạng thái
}
  void button3()
{
  static bool lastState3 = LOW;  // nhớ trạng thái nút lần trước
  bool currentState3 = digitalRead(BT3);

  if (lastState3 == LOW && currentState3 == HIGH) {  // phát hiện nhấn xuống
    delay(20);  // chống dội
    if (digitalRead(BT3) == HIGH && p >= 0.0 ) {
      p = p - 0.1;
      Serial1.print("P=");
      Serial1.println(p, 3);
      // Serial1.print("Gia tri xac suat loi p la: "); Serial1.print(p); Serial1.write('\n');
    }
  }

  lastState3 = currentState3;  // cập nhật trạng thái
}
  void button4()
{
  static bool lastState4 = LOW;  // nhớ trạng thái nút lần trước
  bool currentState4 = digitalRead(BT4);

  if (lastState4 == LOW && currentState4 == HIGH) {  // phát hiện nhấn xuống
    delay(20);  // chống dội
    if (digitalRead(BT4) == HIGH ) {
      
      
      Serial1.println(2);
      // Serial1.print("Gia tri xac suat loi p la: "); Serial1.print(p); Serial1.write('\n');
    }
  }

  lastState4 = currentState4;  // cập nhật trạng thái
}