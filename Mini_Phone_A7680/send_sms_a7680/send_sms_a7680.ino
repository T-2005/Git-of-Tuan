#include <Arduino.h>
#include <HardwareSerial.h>

#define TX 17
#define RX 16
#define led_done 18
#define led_failed 19
#define sim Serial2
#define baud 115200

void sim_read(unsigned long timeout = 2000)
{
    unsigned long t = millis();
    while (millis() - t < timeout)
    {
        while (sim.available())
        {
            char c = sim.read();
            Serial.write(c);
        }
    }
    Serial.println();
}

void send_cmd(const char *cmd, unsigned long timeout = 2000)
{
    Serial.print(">> ");
    Serial.println(cmd);
    sim.println(cmd);
    sim_read(timeout);
}

bool send_sms(const char *phone_number, const char *sms)
{
    // Tắt cả 2 LED trước khi bắt đầu
    digitalWrite(led_done, LOW);
    digitalWrite(led_failed, LOW);
    
    // Xóa buffer cũ
    while(sim.available()) sim.read();
    
    // Text mode
    sim.println("AT+CMGF=1");
    delay(500);
    if (!sim.find("OK")) {
        Serial.println("CMGF failed");
        digitalWrite(led_failed, HIGH);
        return false;
    }
    
    // Xóa buffer
    while(sim.available()) sim.read();

    // CMGS
    sim.print("AT+CMGS=\"");
    sim.print(phone_number);
    sim.println("\"");

    // Đợi dấu >
    unsigned long t = millis();
    bool found = false;
    while (millis() - t < 5000) {
        if (sim.find(">")) {
            found = true;
            break;
        }
    }
    
    if (!found) {
        Serial.println("No prompt >");
        digitalWrite(led_failed, HIGH);
        return false;
    }

    // Gửi nội dung
    sim.print(sms);
    delay(100);
    sim.write(0x1A);  // Ctrl+Z

    // Đợi kết quả
    t = millis();
    while (millis() - t < 15000) {
        if (sim.find("+CMGS:")) {
            Serial.println("SMS SENT OK");
            digitalWrite(led_done, HIGH);
            digitalWrite(led_failed, LOW);
            sim_read(2000); // Đọc hết response
            return true;
        }
    }

    Serial.println("SMS FAILED");
    digitalWrite(led_failed, HIGH);
    digitalWrite(led_done, LOW);
    return false;
}
bool make_call(const char *phone_number, unsigned long ring_duration = 15000)
{
    Serial.println("=== Making Voice Call ===");
    
    digitalWrite(led_done, LOW);
    digitalWrite(led_failed, LOW);
    
    while(sim.available()) sim.read();
    
    sim.print("ATD");
    sim.print(phone_number);
    sim.println(";");
    
    Serial.print("Calling: ");
    Serial.println(phone_number);
    
    delay(2000);
    
    bool call_initiated = false;
    unsigned long t = millis();
    
    while (millis() - t < 10000) {  // Tăng timeout lên 10s
        while (sim.available()) {
            String response = sim.readString();
            Serial.print(response);
            
            if (response.indexOf("OK") >= 0) {
                call_initiated = true;
                break;
            }
            
            if (response.indexOf("ERROR") >= 0 || 
                response.indexOf("NO CARRIER") >= 0) {
                Serial.println("✗ Call failed");
                digitalWrite(led_failed, HIGH);
                return false;
            }
        }
        
        if (call_initiated) break;
    }
    
    if (!call_initiated) {
        Serial.println("✗ No response");
        digitalWrite(led_failed, HIGH);
        return false;
    }
    
    Serial.println("✓ Call initiated");
    digitalWrite(led_done, HIGH);
    
    Serial.print("Ringing for ");
    Serial.print(ring_duration / 1000);
    Serial.println(" seconds...");
    
    delay(ring_duration);
    
    // Ngắt máy
    Serial.println("=== Hanging Up ===");
    sim.println("ATH");
    delay(1000);
    sim_read(2000);
    Serial.println("✓ Call ended");
    
    return true;
}
void setup()
{
    Serial.begin(115200);
    pinMode(led_done, OUTPUT);
    pinMode(led_failed, OUTPUT);
    
    digitalWrite(led_done, LOW);
    digitalWrite(led_failed, LOW);
    
    sim.begin(baud, SERIAL_8N1, RX, TX);
    
    delay(3000);  // Tăng delay để module khởi động

    send_cmd("AT");
    delay(500);
    
    send_cmd("ATI");
    delay(500);
    
    send_cmd("AT+CPIN?");
    delay(1000);
    
    send_cmd("AT+CSQ");
    delay(500);
    
    send_cmd("AT+CIMI");
    delay(500);
    
    // Gửi SMS
    send_sms("+84388515014", "Test SMS from ESP32!");
    
    // Test gọi điện (BỎ COMMENT nếu muốn test)
    // delay(2000);
    // make_call("ATD+84388515014;", 30000);  // Gọi điện
  
 

void loop() {}