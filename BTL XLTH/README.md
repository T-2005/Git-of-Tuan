# Git-of-Tuan - Báo cáo BTL - XLTH - 26/4/2025

# A. Công việc đã làm 

## Công cụ: 
- Oled ST7735 150x200
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
## 5. Tạo biểu đồ cột trên oled

				
		 for(int i = 0; i < 135; i+= 15) // hiển thị 10 cột 
		 {
				display.read_hc_sr04();
				Serial.println(distance);
			   tft.fillRect(i + 15, 80-distance, 10, distance, ST77XX_BLUE); // hiển thị biểu đồ
			   tft.setCursor(i + 15, 90); // hiển thị số
			   if(distance >= 100) tft.print("O");
			   else tft.print(distance);

			delay(1000);
		 }
		 tft.fillRect(0, 90, 200, 60, ST77XX_BLACK); // Xóa vùng hiển thị số 
		  tft.fillRect( 15, 0, 200, 80, ST77XX_BLACK); // xóa vùng biểu đồ cột


# B. Cập nhật nhiệm vụ
- cần làm cho biểu đồ cột giữ trạng thái khi sensor đếm đc 10 lần -> ý tưởng thêm vòng 'while' thay 'for' 
- hiện thị giá trị trung bình dưới biểu đồ  