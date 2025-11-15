#include <Arduino.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_ST7789.h>
#include <Adafruit_ST7796S.h>
#include <Adafruit_ST77xx.h>
#include <stream.h>
#include <stdint.h> 

#define green ST7735_GREEN
#define black ST7735_BLACK
#define red   ST7735_RED
#define white ST7735_WHITE

// Chân kết nối
int TFT_CS = 15;
int TFT_RST = 4;
int TFT_DC = 2;

// Khởi tạo màn hình
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Vị trí dòng hiện tại
int y = 5;
const int lineHeight = 14; // khoảng cách giữa các dòng

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 32, 33); // UART1: RX=32, TX=33

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.fillScreen(white);

  tft.setTextSize(2);
  tft.setTextColor(black);
  tft.setCursor(5, y);
  tft.println("Waiting data...");
  y += lineHeight;
}

void loop() {
  if (Serial1.available() > 0) {
    String t = Serial1.readStringUntil('\n');
    t.trim();

    Serial.print("Nhan duoc: ");
    Serial.println(t);

    // In ra dòng mới
    tft.setTextSize(2);
    tft.setTextColor(black);
    tft.setCursor(5, y);
    tft.println(t);

    // Tăng vị trí xuống dòng
    y += lineHeight;

    // Nếu vượt quá chiều cao màn hình thì xóa màn hình và in lại từ đầu
    if (y > tft.height() - lineHeight) {
      delay(500); // chờ 0.5s để người xem đọc kịp
      //tft.fillScreen(white);
      y = 5;
    }
  }
}
