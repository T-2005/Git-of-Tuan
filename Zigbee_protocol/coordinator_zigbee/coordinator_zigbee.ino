#ifndef ZIGBEE_MODE_ZCZR
#error "Zigbee coordinator mode is not selected in Tools->Zigbee mode"
#endif

#include "Zigbee.h"

#define LIGHT_ENDPOINT_NUMBER 5
#define LED_PIN 8

ZigbeeLight zbLight = ZigbeeLight(LIGHT_ENDPOINT_NUMBER);

void onLightChange(bool state) {
  digitalWrite(LED_PIN, state ? HIGH : LOW);
  Serial.println("\n════════════════════════════════");
  Serial.printf("🔔 Nhận lệnh: %s\n", state ? "ON" : "OFF");
  Serial.printf("   LED: %s\n", state ? "SÁNG 💡" : "TẮT");
  Serial.println("════════════════════════════════\n");
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n=== COORDINATOR ===");
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  zbLight.setManufacturerAndModel("Espressif", "CoordLight");
  zbLight.onLightChange(onLightChange);
  
  Zigbee.addEndpoint(&zbLight);
  
  // QUAN TRỌNG: Mở mạng ngay khi reboot
  Zigbee.setRebootOpenNetwork(180);  // Mở 180 giây
  
  Serial.println("Khởi động Coordinator...");
  if (!Zigbee.begin(ZIGBEE_COORDINATOR)) {
    Serial.println("ERROR: Khởi động thất bại!");
    ESP.restart();
  }
  
  Serial.println("✓ Coordinator đã sẵn sàng!");
  Serial.println("✓ Mạng đang MỞ trong 180 giây");
  Serial.println("→ Bây giờ hãy reset End Device\n");
}

void loop() {
  delay(100);
}