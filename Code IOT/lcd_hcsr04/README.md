# Git-of-Tuan 

			#include <Wire.h>
			#include <LiquidCrystal_I2C.h>
				int trig = 11; //= 25;
				int echo = 12; //= 26;

			int led[3] = {7}; //{17, 16, 4};
			int button = 6 ;//18;

			int tb = 0;
		unsigned char check = 0;

			LiquidCrystal_I2C lcd(0x27, 16, 2);
			void setup() {
			  Serial.begin(9600);
			  lcd.init();
			  lcd.backlight();
			  lcd.clear();
			  pinMode(trig, OUTPUT); // chân phát tín hiệu
			  pinMode(echo, INPUT); // chân thu tín hiệu
			for(int i = 0; i < 3; i++)
			{
			  pinMode(led[i], OUTPUT);
			}
			  lcd.setCursor(0, 0);
				lcd.print("KC: ");
				lcd.setCursor(0, 1);
			  lcd.print("TB: ");
			lcd.setCursor(4, 1);
			lcd.print("dang cho... ");
			for(int i = 0; i < 3; i++)
			{
			  digitalWrite(led[i], 1);
			}
		   
			}
		  int cnt = 0;
		 int distance; // biến đo khoảng cách
			void loop() {
			start_hc_sr04();
		  
		   in();	
				
			}

		  void in()
		  {
			float gtri = tb / 10;
			  

			  if(cnt == 10)
			  {
				 lcd.setCursor(4, 1); 
				lcd.print("                ");
				lcd.setCursor(4, 1);
				if(gtri > 100)
				 lcd.print("Out of Range");
				else lcd.print(gtri);
				Serial.println(gtri);
				delay(1000);     
				 tb = 0;
				cnt = 0;
			  }	   
			  
		  }

		  void start_hc_sr04()
		  { 
			if(digitalRead(button) == HIGH) // kiem tra neu button bat thi doi 20ms
			 {
			  delay(20);      
				if(digitalRead(button) == HIGH) // neu button bat thi kiem tra bien check
				{
					if(check == 0) // neu check = 0 -> gan bien check = 1 va den sang
					{
					  check = 1;  
					  while(cnt < 10)
					  {
						read_hc_sr04();
						sang_led();
						
					   lcd.setCursor(4, 0); 
						lcd.print("    ");
						lcd.setCursor(4, 0);
						lcd.print(distance);
					   delay(1000); // chờ đọc khoảng cách tiếp theo
						  tb = tb + distance;               
							cnt++;

						   
					  } 
					   tat_led();                   
					}
					else // nguoc lai
					{
					  lcd.setCursor(4, 1);
					  lcd.print("dang cho... ");
					  check = 0;       
					}
					
				}
			  while(digitalRead(button) == HIGH); // vong lap vo han cho den khi nut nhan dc nhan 1 lan nua
			 }
		   Serial.println(check);

		  }
		void sang_led()
		{
		  for(int i = 0; i < 3; i++)
		  {
			digitalWrite(led[i], 0 );
			  //delay(500);
		   // digitalWrite(led[i], 0);
		  
		  }
		}
		void tat_led()
		{
		  for(int i = 0; i < 3; i++)
		  {
			digitalWrite(led[i], 1);
		  }
		}
		void read_hc_sr04()
		{
			unsigned long duration; // biến đo thời gian
			digitalWrite(trig, 0); // tắt chân trig
			delay(2);
			digitalWrite(trig, 1); // phát xung
			delay(5);
			digitalWrite(trig, 0);
			duration = pulseIn(echo, HIGH);
			distance = int(duration / 2 / 29.412);
		}