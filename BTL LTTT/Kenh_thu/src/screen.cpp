#include "screen.h"

int check = 0;
int tmp;
int TFT_CS = 15;
int TFT_RST = 4;
int TFT_DC = 2;
int _trap = 32;
int a[100] = {0}, k = 0;
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
void screen ::init()
{
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);
  tft.fillScreen(ST7735_WHITE);

  // tft.drawRect(40, 0, 90, 128, ST7735_RED);  // h2
  // tft.drawRect(90, 0, 140, 128, ST7735_RED); // h3
  tft.drawRect(0, 0, 70, 15, ST7735_GREEN); // h1-c1
  tft.setCursor(5, 5);
  tft.setTextSize(1);
  tft.setTextColor(ST7735_BLACK);
  tft.println("SIMULATION:");

  tft.drawRect(70, 0, 110, 15, ST7735_GREEN); // h1-c2
  tft.setCursor(85, 5);
  tft.setTextSize(1);
  tft.setTextColor(ST7735_BLACK);
  tft.println("BSC");

  tft.drawRect(110, 0, 160, 15, ST7735_GREEN); // h1-c3

  tft.drawLine(80, 15, 80, 110, green);
  tft.drawLine(0, 50, 160, 50, ST7735_GREEN); // hang 2
  tft.drawLine(0, 110, 160, 110, green);      // hang 3
}

void screen ::_solve(void)
{

  if (Serial.available() > 0)
  {
    String cmd = Serial.readStringUntil('\n'); // đọc chuỗi ký tự gửi từ vdk hoặc máy tính
    cmd.trim();                                // xóa kí tự xuống dòng

    if (cmd == "turn off")
    {
      delay(20);
      Serial.println("3");
      if (cmd == "turn off")
      {
        Serial.println("2");
        if (check == 1)
        {
          Serial.println("1");
          for (int i = k - 1; i >= 0; i--)
          {
            tft.setCursor(5 + 5 * (k - 1 - i), 25); // mã tín hiệu bắt đầu từ x = 5, y = 25, mỗi bit chiếm 5 pixel

            tft.print(a[i]);
            Serial.print(a[i]);
          }
          for (int i = k - 1; i >= 0; i--)
          {
            gach_chan(5 + 5 * (i), 33);
            delay(500);
            tft.drawLine(5 + 5 * (i), 33, 5 + 5 * (i) + 5, 33, white);
          }

          delay(400);
          Serial.println();
        }
        else
        {
          check = 1;
          int i = 0;
          while (i < 10)
          {
            __int_to_binary(tmp, a, &k);
            i++;
            tmp = analogRead(_trap);
            Serial.println("5");
          }
          tft.fillRect(0, 16, 69, 20, white);
        }
      }
      while (Serial.readStringUntil('\n') == "turn off")
        ;
    }
  }
}
void screen ::gach_chan(int x, int y)
{
  tft.drawLine(x, y, x + 5, y, green);
}
void screen ::__int_to_binary(int n, int a[], int *k)
{
  while (n != 0)
  {
    a[(*k)++] = n % 2;
    n /= 2;
  }
}