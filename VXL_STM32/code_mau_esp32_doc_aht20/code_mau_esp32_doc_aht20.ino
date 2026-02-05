#include <Wire.h>
#include <Adafruit_AHTX0.h>  // Correct library for AHT20

// Create an instance of the AHT20 sensor
Adafruit_AHTX0 aht;  // Use AHTX0 instead of AHT10

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  Serial.println("AHT20 Sensor Example");

  // Initialize I2C communication
  if (!aht.begin()) {  // No parameter needed
    Serial.println("Failed to find AHT20 sensor! Check wiring.");
    while (1);
  }
  Serial.println("AHT20 sensor initialized.");
}

void loop() {
  // Get sensor data
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp); // Retrieve sensor readings

  // Print temperature and humidity
  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(humidity.relative_humidity);
  Serial.println(" %");

  // Delay between readings
  delay(2000);
}

  

  

