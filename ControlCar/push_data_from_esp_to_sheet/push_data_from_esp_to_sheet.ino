#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <WiFiClientSecure.h>

Adafruit_MPU6050 mpu;

// WiFi credentials
const char* ssid = "Nha Tro Vui Ve";
const char* password = "66668888";

// Web App URL
const char* serverName = "https://script.google.com/macros/s/AKfycbyuhmEyyeylN8w_suVhNGsoCbg3yGpvOO6Efazc3b0Igyzg2uTk1hmuz4mp0h9OsNo-5g/exec";
void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  // Khởi tạo MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);

  delay(100);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // Tạo client bảo mật nhưng bỏ qua chứng chỉ
    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient https;
    if (https.begin(client, serverName)) {
      https.addHeader("Content-Type", "application/json");

      // Lấy dữ liệu MPU6050
      sensors_event_t a, g, temp;
      mpu.getEvent(&a, &g, &temp);

      int AX = (int)a.acceleration.x;
      int AY = (int)a.acceleration.y;
      int AZ = (int)a.acceleration.z;

      Serial.printf("AX=%d | AY=%d | AZ=%d\n", AX, AY, AZ);

      // Tạo JSON
      String jsonData = "{\"AX\":" + String(AX) +
                        ",\"AY\":" + String(AY) +
                        ",\"AZ\":" + String(AZ) + "}";

      int httpResponseCode = https.POST(jsonData);

      if (httpResponseCode > 0) {
        Serial.println("HTTP Response code: " + String(httpResponseCode));
        Serial.println("Response: " + https.getString());
      } else {
        Serial.println("Error code: " + String(httpResponseCode));
      }

      https.end();
    } else {
      Serial.println("Unable to connect to server!");
    }
  }

  delay(500); // gửi mỗi 5 giây
}
