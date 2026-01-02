#ifndef ZIGBEE_MODE_ED
#error "Please select Zigbee End Device mode"
#endif

#include "Zigbee.h"

#define BUTTON_PIN 9
#define ENDPOINT_ID 1

ZigbeeSwitch zbSwitch(ENDPOINT_ID);
bool ledState = false;

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n=== END DEVICE ===");
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  zbSwitch.setManufacturerAndModel("Espressif", "EDSwitch");
  zbSwitch.allowMultipleBinding(true);
  
  Zigbee.addEndpoint(&zbSwitch);
  
  // QUAN TRỌNG: End Device KHÔNG dùng setRebootOpenNetwork
  
  Serial.println("Khởi động End Device...");
  if (!Zigbee.begin()) {  // KHÔNG truyền tham số role
    Serial.println("ERROR: Khởi động thất bại!");
    ESP.restart();
  }
  
  Serial.println("✓ End Device đã khởi động!");
  Serial.println("→ Đang tìm Coordinator...\n");
  
  // Chờ kết nối với mạng
  Serial.print("Đang join network");
  int timeout = 0;
  while (!Zigbee.connected() && timeout < 300) {  // 30 giây
    Serial.print(".");
    delay(100);
    timeout++;
  }
  
  if (!Zigbee.connected()) {
    Serial.println("\n✗ KHÔNG thể kết nối mạng!");
    Serial.println("Kiểm tra:");
    Serial.println("1. Coordinator đã chạy chưa?");
    Serial.println("2. Coordinator có mở mạng không?");
    Serial.println("3. Cả 2 board cùng kênh Zigbee?");
    Serial.println("\n→ Khởi động lại sau 5 giây...");
    delay(5000);
    ESP.restart();
  }
  
  Serial.println("\n✓ Đã kết nối mạng!");
  Serial.println("→ Đang binding với Light...\n");
  
  // Chờ binding
  Serial.print("Đang binding");
  timeout = 0;
  while (!zbSwitch.bound() && timeout < 300) {  // 30 giây
    Serial.print(".");
    delay(100);
    timeout++;
  }
  
  if (!zbSwitch.bound()) {
    Serial.println("\n✗ KHÔNG thể binding!");
    Serial.println("→ Thử reset cả 2 thiết bị");
    while(1) delay(1000);
  }
  
  Serial.println("\n✓✓✓ ĐÃ BINDING THÀNH CÔNG! ✓✓✓");
  Serial.println("\nNhấn nút BOOT:");
  Serial.println("  Nhấn xuống → ON");
  Serial.println("  Thả ra    → OFF\n");
}

void loop() {
  static bool lastState = HIGH;
  static unsigned long lastTime = 0;
  
  bool currentState = digitalRead(BUTTON_PIN);
  
  if (currentState != lastState) {
    if (millis() - lastTime > 50) {
      lastTime = millis();
      
      if (lastState == HIGH && currentState == LOW) {
        if (!ledState) {
          Serial.println("→ Gửi ON");
          zbSwitch.lightOn();
          ledState = true;
        }
      }
      else if (lastState == LOW && currentState == HIGH) {
        if (ledState) {
          Serial.println("→ Gửi OFF");
          zbSwitch.lightOff();
          ledState = false;
        }
      }
    }
  }
  
  lastState = currentState;
  delay(10);
}
