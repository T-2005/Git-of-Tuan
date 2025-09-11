
int touch_2 = 4;
int led = 27;
void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  

  
  
 int A_2 = touchRead(touch_2);
while(A_2 < 20) {
  digitalWrite(led, HIGH);
   A_2 = touchRead(touch_2);
  
}
digitalWrite(led, LOW);
Serial.println(A_2); delay(200);
}
