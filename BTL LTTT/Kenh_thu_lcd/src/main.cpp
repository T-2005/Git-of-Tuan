#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x26, 16, 2);  // LCD hiển thị

String inputData = "";   // Chuỗi tạm để đọc UART
float p_value = 0.0;
float ber_value = 0.0;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 32, 33); // RX = 32, TX = 33

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Waiting...");
}

void loop() {
  // Kiểm tra dữ liệu từ UART
  if (Serial1.available()) {
    inputData = Serial1.readStringUntil('\n'); // đọc 1 dòng
    inputData.trim();

    Serial.print("Nhan duoc: ");
    Serial.println(inputData);

    // Kiểm tra chuỗi có bắt đầu bằng P= hay BER=
    if (inputData.startsWith("P=")) {
      p_value = inputData.substring(2).toFloat();
      Serial.print("Cap nhat P = ");
      Serial.println(p_value, 3);
    } 
    else if (inputData.startsWith("BER=")) {
      ber_value = inputData.substring(4).toFloat();
      Serial.print("Cap nhat BER = ");
      Serial.println(ber_value, 4);
    }

    // --- Hiển thị lên LCD ---
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Xac suat P = ");
    lcd.print(p_value, 3);

    lcd.setCursor(0, 1);
    lcd.print("Ti so BER = ");
    lcd.print(ber_value, 4);
  }
}
