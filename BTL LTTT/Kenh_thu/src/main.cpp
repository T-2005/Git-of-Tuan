#include <Arduino.h>
#include <screen.h>


/*
  tft.drawPixel(x, y, color); Vẽ điểm
  tft.drawLine(x0, y0, x1, y1, color); Vẽ đường

  tft.drawRect(x, y, w, h, color);    // viền chữ nhật
  tft.fillRect(x, y, w, h, color);    // tô đầy

  tft.drawCircle(x, y, r, color);     // viền hình tròn
  tft.fillCircle(x, y, r, color);     // tô đầy

  tft.drawTriangle(x0, y0, x1, y1, x2, y2, color);  tam giác
  tft.fillTriangle(x0, y0, x1, y1, x2, y2, color);

  tft.drawRoundRect(x, y, w, h, r, color); elip F
  tft.fillRoundRect(x, y, w, h, r, color);

  tft.setCursor(x, y);
  tft.setTextColor(ILI9341_WHITE);      // màu chữ
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK); // chữ + nền
  tft.setTextSize(2);
  tft.println("Hello TFT!");

  Hàm	Chức năng
  tft.fillScreen(color)	Tô toàn màn hình
  tft.setRotation(n)	Xoay màn hình (0–3)
  tft.width() / tft.height()	Lấy kích thước hiện tại
  tft.invertDisplay(true/false)	Đảo màu
  tft.drawBitmap(x, y, bitmap, w, h, color)	Vẽ ảnh đơn sắc
  tft.drawRGBBitmap(x, y, bitmap, w, h)	Vẽ ảnh màu (16-bit RGB565)




*/



screen scr;

void setup()
{
  pinMode(_trap, INPUT);
  Serial.begin(115200);
  scr.init();
}


 
void loop()
{
  scr._solve();
}


