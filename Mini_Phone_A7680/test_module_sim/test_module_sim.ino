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

// HÀM KIỂM TRA SIM - PHIÊN BẢN CẢI TIẾN
bool check_sim_ready()
{
    Serial.println("=== Checking SIM Status ===");
    
    for (int i = 0; i < 3; i++) {  // Thử 3 lần
        while(sim.available()) sim.read();
        
        sim.println("AT+CPIN?");
        delay(1000);  // Tăng delay
        
        String response = "";
        unsigned long t = millis();
        while (millis() - t < 2000) {
            while (sim.available()) {
                char c = sim.read();
                response += c;
                Serial.write(c);
            }
        }
        Serial.println();
        
        if (response.indexOf("READY") >= 0) {
            Serial.println("✓ SIM is READY");
            return true;
        }
        
        if (response.indexOf("SIM PIN") >= 0) {
            Serial.println("✗ SIM requires PIN");
            return false;
        }
        
        Serial.print("Attempt ");
        Serial.print(i + 1);
        Serial.println(" - Retrying...");
        delay(2000);
    }
    
    Serial.println("✗ SIM NOT READY after 3 attempts");
    return false;
}

// HÀM KIỂM TRA MẠNG - PHIÊN BẢN CẢI TIẾN
bool check_network_registration()
{
    Serial.println("=== Checking Network Registration ===");
    
    // Chờ đăng ký mạng tối đa 30 giây
    for (int i = 0; i < 10; i++) {
        while(sim.available()) sim.read();
        
        sim.println("AT+CREG?");
        delay(500);
        
        String response = "";
        unsigned long t = millis();
        while (millis() - t < 2000) {
            while (sim.available()) {
                char c = sim.read();
                response += c;
                Serial.write(c);
            }
        }
        Serial.println();
        
        // Tìm +CREG: 0,1 hoặc +CREG: 0,5
        int idx = response.indexOf("+CREG:");
        if (idx >= 0) {
            // Lấy ký tự sau dấu phẩy cuối cùng
            int lastComma = response.lastIndexOf(',');
            if (lastComma > 0 && lastComma < response.length() - 1) {
                char stat = response.charAt(lastComma + 1);
                
                Serial.print("Registration status: ");
                Serial.println(stat);
                
                if (stat == '1' || stat == '5') {
                    Serial.println("✓ Registered on network");
                    return true;
                }
            }
        }
        
        Serial.print("Not registered yet. Waiting... (");
        Serial.print(i + 1);
        Serial.println("/10)");
        delay(3000);
    }
    
    Serial.println("✗ Not registered after 30 seconds");
    return false;
}

bool check_signal()
{
    Serial.println("=== Checking Signal Quality ===");
    
    while(sim.available()) sim.read();
    
    sim.println("AT+CSQ");
    delay(500);
    
    String response = "";
    unsigned long t = millis();
    while (millis() - t < 2000) {
        while (sim.available()) {
            char c = sim.read();
            response += c;
            Serial.write(c);
        }
    }
    Serial.println();
    
    int idx = response.indexOf("+CSQ:");
    if (idx >= 0) {
        int rssi = response.substring(idx + 6).toInt();
        Serial.print("RSSI: ");
        Serial.println(rssi);
        
        if (rssi > 0 && rssi < 32) {
            Serial.println("✓ Signal OK");
            return true;
        }
    }
    
    Serial.println("✗ No Signal");
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
    
    Serial.println("\n\n=== SIM MODULE TEST - DEBUG VERSION ===\n");
    
    delay(5000);  // Tăng delay khởi động lên 5s
    
    // Bước 1
    Serial.println("=== Step 1: AT Command ===");
    send_cmd("AT");
    delay(1000);
    
    // Bước 2
    Serial.println("\n=== Step 2: Module Info ===");
    send_cmd("ATI");
    delay(1000);
    
    // Bước 3 - KHÔNG DỪNG NẾU LỖI
    Serial.println("\n=== Step 3: SIM Check ===");
    bool sim_ok = check_sim_ready();
    if (!sim_ok) {
        Serial.println("⚠️ WARNING: SIM not ready, but continuing...");
        digitalWrite(led_failed, HIGH);
        delay(3000);
        digitalWrite(led_failed, LOW);
    }
    delay(1000);
    
    // Bước 4
    Serial.println("\n=== Step 4: Signal Check ===");
    bool signal_ok = check_signal();
    if (!signal_ok) {
        Serial.println("⚠️ WARNING: Weak signal, but continuing...");
    }
    delay(1000);
    
    // Bước 5 - KHÔNG DỪNG NẾU LỖI
    Serial.println("\n=== Step 5: Network Registration ===");
    bool network_ok = check_network_registration();
    if (!network_ok) {
        Serial.println("⚠️ WARNING: Not registered, but continuing...");
        digitalWrite(led_failed, HIGH);
        delay(3000);
        digitalWrite(led_failed, LOW);
    }
    delay(2000);
    
    // Bước 6 - LUÔN THỰC HIỆN
    Serial.println("\n=== Step 6: Get Phone Number ===");
    send_cmd("AT+CNUM");
    delay(1000);
    
    // Bước 7 - LUÔN THỰC HIỆN
    Serial.println("\n=== Step 7: Making Test Call ===");
    Serial.println("⚠️ This will make an actual call!");
    delay(3000);
    
    if (make_call("+84388515014", 10000)) {
        Serial.println("\n✓✓✓ CALL TEST COMPLETED ✓✓✓");
    } else {
        Serial.println("\n✗✗✗ CALL TEST FAILED ✗✗✗");
    }
    
    Serial.println("\n=== ALL STEPS COMPLETED ===");
    
    // Tổng kết
    Serial.println("\n--- SUMMARY ---");
    Serial.print("SIM Ready: ");
    Serial.println(sim_ok ? "✓" : "✗");
    Serial.print("Signal: ");
    Serial.println(signal_ok ? "✓" : "✗");
    Serial.print("Network: ");
    Serial.println(network_ok ? "✓" : "✗");
}

void loop() 
{
    while (sim.available()) {
        char c = sim.read();
        Serial.write(c);
    }
    
    while (Serial.available()) {
        char c = Serial.read();
        sim.write(c);
    }
}