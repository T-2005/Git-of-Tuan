# Git-of-Tuan - Báo cáo BTL - XLTH - 26/4/2025

# Họ tên và mã sinh viên

	Lê Ngọc Tuấn - B23DCDT275 - DSPSP25PROJ11
	Hoàng Hải Đăng - B23DCDT303 - DSPSP25PROJ11
	Trần Đại Hiệp - B23DCDT087 - DSPSP25PROJ11
	Nguyễn Văn Toàn - B23DCDT259 - DSPSP25PROJ11

## Thư viện dùng trong dự án
	"Adafruit_ST7735.h" dùng cho màn hình hiển thị oled ST7735
## Mô tả ngắn gọn về dự án
	- Đề tài: Thu thập tín hiệu từ module cảm biến siêu âm HC-SR04 
và hiện thị tín hiệu lên màn hình Oled Giao tiếp SPI ST7735.
	- Tóm tắt nội dung: Dữ liệu hiển thị lên màn hình là tín hiệu
của HCSr04, giá trị trung bình thu được sau 10 lần đo.Nếu vượt 
quá 100 thì in ra “overload”, ngược lại in ra giá trị trung bình.

## Công cụ: 
- Oled ST7735 150x200
- Cảm biến siêu âm HCSr04
- Esp 32 - 38 chân
- Nút nhấn
- Trở 220 ohm
- Dây điện
- Board cắm

## 1. Thư viện khai báo chân: pin.h
	
		#ifndef pin_H
		#define pin_H
		#include <Arduino.h>
		  extern int st; // khai báo chân chốt IC74HC595
		  extern int ds; // khai báo chân data
		  extern int sh; // khai báo chân clock
		  extern int trig; // chân phát tín hiệu của sr04
		  extern int echo; // chân thu tín hiệu của sr04
		  extern int button; // nút nhấn 
		class pin_init {
		  public:
		  void init();
		};
		#endif 

## 2. Thư viện thực hiện khai báo chân: pin.cpp


		#include "pin.h"
		#include <Arduino.h>
		  int st = 35;
		  int ds = 32;
		  int sh = 0;
		  int trig = 25;
		  int echo = 26;
		  int button = 33;
		void pin_init :: init(){
			pinMode(trig, OUTPUT); // chân phát tín hiệu
			 pinMode(echo, INPUT); // chân thu tín hiệu
			pinMode(ds, OUTPUT);
			pinMode(sh, OUTPUT);
			pinMode(st, OUTPUT);
			pinMode(button, INPUT);
		}

## 3. Thư viện sr04: sr04.h
	
		
		#ifndef sr04_H
		#define sr04_H
		
		#include <Arduino.h>
		extern int distance;
		extern int button;
		extern int check;
		class display {		  
			public: 
			void read_hc_sr04();
			void sang_led();
			void start_hc_sr04();
		};
		#endif 
		
## 4. Thư viện thực hiện chức năng sr04: sr04.cpp
	
		
		#include "sr04.h"
		#include "pin.h"
		int distance = 0;
		int check = 0;
		void display :: read_hc_sr04() // đọc cảm biến sr04
		{
			unsigned long duration; // biến đo thời gian
			digitalWrite(trig, 0); // tắt chân trig
			delay(2);
			digitalWrite(trig, 1); // phát xung
			delay(5);
			digitalWrite(trig, 0); // tắt chân trig
			duration = pulseIn(echo, HIGH); 
			distance = int(duration / 2 / 29.412);
		}

		void display :: sang_led()
		{
		  for(int i = 0; i < 8; i++)
		  {
		   digitalWrite(st, 0);  
		   shiftOut(ds, sh, LSBFIRST, 0xFF); // LSBFIRST và MSBFIRST 
		   digitalWrite(st, 1);
		  }
		}

		 void display :: start_hc_sr04()
		   { 
			 if(digitalRead(button) == HIGH) // kiem tra neu button bat thi doi 20ms
			 {
			   delay(20);      
				 if(digitalRead(button) == HIGH) // neu button bat thi kiem tra bien check
				 {
					 if(check == 0) // neu check = 0 -> gan bien check = 1 va den sang
					 {
					   check = 1;  				                    
					 }
					 else 
					 {
					  
					   check = 0;       
					 }	
				 }
			   while(digitalRead(button) == HIGH); // vong lap vo han cho den khi nut nhan dc nhan 1 lan nua
			 }

## 5. Thư viện màn oled : tft.h

	#ifndef tft_H 
	#define tft_H

	#include <Arduino.h>

	extern int TFT_CS;    // Dùng để chọn màn hình khi giao tiếp SPI
	extern int TFT_RST;    // reset
	extern int TFT_DC;     // Xác định dữ liệu gửi đến màn hình là lệnh hay dữ liệu hiển thị

	extern int cnt;
	extern float tb;

	 class play {
	  
		public: 
	   void hienthi_man_hinh();
	   void display_gtr_tb();
	   void gtri_tb();
	   void pain_grap();
	};

	#endif

## 6. Thư viện thực hiện chức năng màn oled: tft.cpp

	//======================khai báo thư viện màn oled===================
		#include <Adafruit_ST7735.h>
		#include <Adafruit_ST7789.h>
		#include <Adafruit_ST7796S.h>
		#include <Adafruit_ST77xx.h>
	//======================khai báo thư viện màn oled===================
	
	
		#include "tft.h" // khai báo thư viện màn tft
		#include "sr04.h" // khai báo thư viện HCSr04
		#include "pin.h" // khai báo thư viện các chân GPIO cần dùng

		int TFT_CS   = 5; // Dùng để chọn màn hình khi giao tiếp SPI
		int TFT_RST    = 4;// reset
		int TFT_DC     = 2;// Xác định dữ liệu gửi đến màn hình là lệnh hay dữ liệu hiển thị
		
		display display ; // khai báo biến toàn cục

		Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST); // khởi tạo đối tượng cho màn hình TFT

		int cnt = 0; // khai báo biến đếm (lên 10 thì dừng đo khoảng cách)
		float tb = 0.0; // dùng để tính giá trị trung bình

		void play :: hienthi_man_hinh()
		{
		  tft.initR(INITR_BLACKTAB); // hiển thị lên màn hình
		  
		  tft.setRotation(1);// góc quay hiển thị 
		  tft.fillScreen(ST77XX_BLACK); // xóa màn hình với 1 màu đen
		  tft.drawLine(5, 80, 200, 80, ST77XX_WHITE); // Trục X // tft.drawLine(x1, y1, x2, y2, color)
		  tft.drawLine(5, 80, 5, 0, ST77XX_WHITE);   // Trục y
		  
		  tft.setCursor(0, 120); // setup vị trí in 
		  tft.print("Khoang cach TB: ");
		}
		void play :: pain_grap()
		{
		  display.start_hc_sr04();
			 if(check == 1 && cnt < 10)
			 {					  
				tft.fillRect(0, 90, 200, 30, ST77XX_BLACK); // Xóa vùng hiển thị số 
				tft.fillRect( 10, 0, 200, 80, ST77XX_BLACK); // xóa vùng biểu đồ cột
				while(check == 1 && cnt < 10)  // hiển thị 10 cột 
				 {			   
					  for(int i = -5; i < 145 && cnt <= 9; i+= 15)
					  {
						  display.read_hc_sr04(); // giá trị trả về distance
						  Serial.println(distance);
							  tft.fillRect(i + 15, 80 - distance, 10, distance, ST77XX_BLUE); // hiển thị biểu đồ
							  tft.setCursor(i + 15, 90); // hiển thị số
							  if(distance >= 100) 
							  {
								tft.print("O");
							  }
							  else tft.print(distance);	
							  tb += distance; // tính giá trị trung bình
							  delay(1000);
							  cnt++;
					  }
			   }
			 }
		   display_gtr_tb();  // hiển thị giá trị trung bình sau 10 lần đo			
		}        
			   
		void play :: display_gtr_tb() // thực hiện chức năng in giá trị trung bình ra màn hình
		{
		  if(cnt == 10)
		  {
			gtri_tb();
			cnt = 0;
			while(check == 1)
			{
			  display.start_hc_sr04();
			}
		  }
			 if(check == 0 && cnt == 0) 
			 { 	   
			   tft.fillRect(0, 90, 200, 30, ST77XX_BLACK); // Xóa vùng hiển thị số 
				tft.fillRect( 10, 0, 200, 80, ST77XX_BLACK); // xóa vùng biểu đồ cột
				 tft.fillRect(90, 120, 200, 30, ST77XX_BLACK); // xóa vùng hiển thị gtri trung binh
				 while(check == 0)
				 {
				   display.start_hc_sr04(); // đọc nút ấn - trả giá trị về biến check
					tft.setCursor(30, 50); // setup vị trí in 
					display.start_hc_sr04(); //đọc nút ấn - trả giá trị về biến check
					tft.print("Waitting...");	
					display.start_hc_sr04(); //đọc nút ấn - trả giá trị về biến check
					 tft.setCursor(90, 120); // setup vị trí in 
					  display.start_hc_sr04(); //đọc nút ấn - trả giá trị về biến check
					 tft.print("Loading...");		 
					 display.start_hc_sr04(); //đọc nút ấn - trả giá trị về biến check	   
				 }
			  }  
		}		 
		  

		void play :: gtri_tb()
		{   
				 tb =(float)(tb / 10);
				  tft.fillRect(90, 120, 200, 30, ST77XX_BLACK); // xóa vùng hiển thị gtri trung binh
				  while( cnt == 10)
				  {
					tft.setCursor(90, 120); 
					if(tb > 100) 
					{
					  tft.print("overload");					   
					  cnt = 0;
					}
					else 
					{
					  tft.print(tb);   			
					  cnt = 0;
					}	
				  }   
		}

## 7. Code Tạo biểu đồ cột trên oled

				
		 for(int i = 0; i < 135; i+= 15) // hiển thị 10 cột 
		 {
				display.read_hc_sr04(); // đọc nút ấn
				Serial.println(distance);
			   tft.fillRect(i + 15, 80-distance, 10, distance, ST77XX_BLUE); // hiển thị biểu đồ
			   tft.setCursor(i + 15, 90); // hiển thị số
			   if(distance >= 100) tft.print("O");
			   else tft.print(distance); // in ra giá trị khoảng cách đo được

			delay(1000);
		 }
		 tft.fillRect(0, 90, 200, 60, ST77XX_BLACK); // Xóa vùng hiển thị số 
		  tft.fillRect( 15, 0, 200, 80, ST77XX_BLACK); // xóa vùng biểu đồ cột


# B. Mục tiêu nhiệm vụ
- Đọc tín hiệu mà cảm biến HCSr04 trả về, biến đổi và đưa tín hiệu đó về dạng khoảng cách đơn vị cm
- Hiện thị khoảng cách lên màn hình Oled dưới dạng biểu đồ cột
- Hiện thị giá trị trung bình dưới dạng số thực sau dấu phẩy 2 chữ số, nếu nhỏ hơn 100 cm thì in ra giá
trị trung bình còn không thì in ra "Overload"
- Sử dụng nút ấn đọc giá trị cảm biến 10 lần, sau 10 lần ấn nút để reset và đo lại từ đâu

  