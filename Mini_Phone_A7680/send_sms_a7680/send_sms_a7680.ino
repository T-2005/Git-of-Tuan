#include <Arduino.h>
#include <HardwareSerial.h>

#define TX 17
#define RX 16

#define sim Serial2
#define baud 115200

void sim_read(unsigned long timeout = 2000)
{
    unsigned long t = millis();
    while (millis() - t < timeout)
    {
        while (sim.available())
        {
            Serial.write(sim.read());
        }
    }
    Serial.println();
}

void send_cmd(const char *cmd)
{
    Serial.print(">> ");
    Serial.println(cmd);
    sim.println(cmd);
    sim_read();
}
bool send_sms(const char *phone_number, const char *sms)
{
    // Chế độ text
    sim.println("AT+CMGF=1");
    sim_read();

    // Gửi lệnh CMGS
    sim.print("AT+CMGS=\"");
    sim.print(phone_number);
    sim.println("\"");

    // Đợi dấu '>'
    unsigned long t = millis();
    while (millis() - t < 5000)
    {
        if (sim.find(">"))
        {
            break;
        }
    }

    // Gửi nội dung SMS
    sim.print(sms);
    sim.write(0x1A); // Ctrl+Z

    // Đợi phản hồi gửi thành công
    t = millis();
    while (millis() - t < 15000)
    {
        if (sim.find("+CMGS:"))
        {
            Serial.println("SMS SENT OK");
            sim_read();
            return true;
        }
    }

    Serial.println("SMS FAILED");
    sim_read();
    return false;
}

void setup()
{
    Serial.begin(115200);
    sim.begin(baud, SERIAL_8N1, RX, TX);

    delay(2000);

    send_cmd("AT");           // Check AT Command

    send_cmd("ATI");          // Product infor

    send_cmd("AT+CPIN?");     // Check SIM Slot

    send_cmd("AT+CSQ");       // Check Signal Quality

    send_cmd("AT+CIMI");
    send_cmd("AT+CHUP"); // kiem tra xem co ho tro goi ko

    send_sms("0388515014", "hihi!");



    // Chỉ gọi khi SIM READY + có mạng
    // send_cmd("ATD123456789;");
}

void loop() {}
