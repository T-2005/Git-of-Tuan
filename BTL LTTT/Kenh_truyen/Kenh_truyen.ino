#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define BT1 32
#define BT2 33
#define BT3 25

LiquidCrystal_I2C lcd1(0x26, 16, 2); // LCD phát
LiquidCrystal_I2C lcd2(0x27, 16, 2); // LCD nhận
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
  Serial2.begin(115200, SERIAL_8N1, 16, 17);   // UART2: RX=16, TX=17
  while (Serial2.available()) Serial2.read();  // clear buffer
  
  Serial.println("ESP32 Ready - LE NGOC TUAN");

  pinMode(BT1, INPUT_PULLUP);
  pinMode(BT2, INPUT_PULLUP);
  pinMode(BT3, INPUT_PULLUP);
}

void loop() {
  bitErr = 0;
  // --- GỬI DỮ LIỆU từ PC -> STM32 ---
  if (Serial.available()) {
    String t = Serial.readStringUntil('\n');
    t.trim();
    if (t.length() > 0) {
      Serial2.print(t);
      Serial2.write('\n');
      Serial.print("Da gui: ");
      Serial.println(t);
    }
  }

  // --- NHẬN DỮ LIỆU từ STM32 -> LCD ---
  _lcd_display();
}

void _lcd_display(void) {
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
      delay(100); // chờ hiển thị
    }
    Serial.print("Bit BSC:   ");
    Serial.println(a);
    BER = (float)bitErr / n;
    Serial.print("Ti so BER: "); Serial.println(BER, 4);
    // lcd2.setCursor(0, 1);
    // lcd2.print("BER=");
    // lcd2.print(BER, 2);

    // Xóa buffer UART tránh đọc rác
    while (Serial2.available()) Serial2.read();
  }
}
