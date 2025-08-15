# Git-of-Tuan - Code Leanbot

		/*
		  Basic Leanbot Motion

		  Wait for TB1A+TB1B touch signal, then go straight for 100 mm, then stop.

		  More Leanbot examples at  https://git.pythaverse.space/leanbot/Examples
		*/


		#include <Leanbot.h>                    // use Leanbot library

		void setup() {
		  Leanbot.begin(); // initialize Leanbot
		   LbIRLine.doManualCalibration(TB1A);
		}


		void loop() {
		  LbMission.begin( TB1A + TB1B );       // start mission when both TB1A and TB1B touched
		 
		//   nv1_2_3();
		//  end_game_xanh();
		//   end_game_blue();
		//  end_game_red();
		 end();
		  LbMission.end();                      // stop, finish mission
		}
		void end()
		{
		  move(-2000, 390);
		  turnL(1500, 70);
		  move(2000, 320);
		  delay(5000);
		}

		void nv1_2_3()
		{
		  move(2000, 670);

		   move(-2000, 10);
		   
		  LbGripper.close();
		  LbMotion.stopAndWait();

		  move(1500, 250);

		  LbMotion.runLR(1500, 0);
		  LbMotion.waitRotationDeg(85);
		  LbMotion.stopAndWait();

		 move(1000, 190);
		  
		  LbMotion.runLR(-1500, -1500);
		  LbMotion.waitDistanceMm(45);
		  LbGripper.open();

		}

		void end_game_red()
		{
		  move(-1500, 110);
		  turnR(1500, 70);
		  move(1500, 150);
		  turnR(1500, 70);
		  goLine();
		  LbMotion.stopAndWait();
		  
		  move(-1500, 10);
		  LbGripper.close();
		  LbMotion.stopAndWait();
		  turnL(1500, 160);
		  goLine();
		  move(-1500, 35);
		  
		}

		void end_game_xanh()
		{
		  LbMotion.runLR(1500, 0);
		  LbMotion.waitRotationDeg(65);
			   
			 move(1500, 300);
		  
		   move(-1500, 10);

			 LbGripper.close();  
		   LbMotion.stopAndWait();  
			  
			  

			goLine();
			LbMotion.stopAndWait();

			move(-1500, 30);
			LbGripper.open();
			LbMotion.stopAndWait();
		}
		void end_game_blue()
		{
		  move(-1500, 90); // lui 90 cm

		  turnL(1000, 85); // quay trai 90

		  move(1500, 120);

		  
		  move(-1500, 10);

		  LbGripper.close();
		  LbMotion.stopAndWait();

		 turnR(1000, 165);

		  goLine();
		  LbMotion.stopAndWait();

		 move(-1500, 30);

			LbGripper.open();
			LbMotion.stopAndWait();
		}
		void move(int v, int distance)
		{
			LbMotion.runLR(v, v);
			LbMotion.waitDistanceMm(distance);
			LbMotion.stopAndWait();
		}

		void turnL(int v, int deg)
		{
			LbMotion.runLR(-v, +v);
			LbMotion.waitRotationDeg(deg); // quay 90
			LbMotion.stopAndWait();
		}

		void turnR(int v, int deg)
		{
			 LbMotion.runLR(+v, -v);
			LbMotion.waitRotationDeg(deg); // quay 90
			LbMotion.stopAndWait();
		}



		int speed = 1500;

		void goLine()
		{
		   do{
			 followLineCarefully();
		  }while(LbIRLine.isBlackDetected());
		}
		void followLineCarefully() {             // Follow the black line, if you don't see the black line, read it several more times to be sure
		  for (int i = 0; i < 30; i++) {         // go up to 30*1mm = 30mm when the black line is not visible
			runFollowLine();
			if ( LbIRLine.isBlackDetected() ) {
			  break;
			}
			LbMotion.waitDistanceMm(1);          // go 1mm and then read the sensor again
		  }
		}
		void runFollowLine()
		{
		  byte linevalue = LbIRLine.read();
		  
			switch(linevalue)
			{
			  case 0b0100:
			  case 0b1110:
				Serial.println("quay trai");
				LbMotion.runLR(0, +speed);
			  break;
			  
			  case 0b1100:
			  case 0b1000:
			   Serial.println("dich trai");
				LbMotion.runLR(-speed, +speed);
			  break;

			  case 0b0010:
			  case 0b0111:
				 Serial.println("quay phai");
				LbMotion.runLR(+speed, 0);
			  break;

			  case 0b0011:
			  case 0b0001:
			   Serial.println("dich phai");
				LbMotion.runLR(+speed, -speed);
			  break;

			  default: LbMotion.runLR(speed, speed);
			  Serial.println("di thang");
			  
			}
		  
		} 