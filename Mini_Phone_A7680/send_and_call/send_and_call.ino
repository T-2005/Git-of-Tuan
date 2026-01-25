#include <HardwareSerial.h>

#define SIM_SERIAL   Serial2
#define DEBUG_SERIAL Serial

#define SIM_BAUDRATE 115200
#define SIM_TX_PIN   17
#define SIM_RX_PIN   16
#define SIM_PWR_PIN  15

#define PHONE_NUMBER "0388515014"

/* ================= POWER SIM ================= */
void sim_power_on()
{
    pinMode(SIM_PWR_PIN, OUTPUT);

    digitalWrite(SIM_PWR_PIN, LOW);
    delay(1000);
    digitalWrite(SIM_PWR_PIN, HIGH);
    delay(2000);
    digitalWrite(SIM_PWR_PIN, LOW);

    delay(8000); // chờ SIM boot xong
}

/* ================= READ SIM ================= */
void sim_read(unsigned long timeout = 3000)
{
    unsigned long t = millis();
    while (millis() - t < timeout)
    {
        while (SIM_SERIAL.available())
        {
            DEBUG_SERIAL.write(SIM_SERIAL.read());
        }
    }
}

/* ================= SEND CMD ================= */
void sim_cmd(const char *cmd)
{
    DEBUG_SERIAL.print(">> ");
    DEBUG_SERIAL.println(cmd);

    SIM_SERIAL.println(cmd);
    sim_read();
}

/* ================= SEND SMS ================= */
bool sendSMS(const char *number, const char *text)
{
    sim_cmd("AT+CMGF=1");

    SIM_SERIAL.print("AT+CMGS=\"");
    SIM_SERIAL.print(number);
    SIM_SERIAL.println("\"");

    delay(1000); // chờ '>'

    SIM_SERIAL.print(text);
    SIM_SERIAL.write(0x1A); // Ctrl+Z

    unsigned long t = millis();
    while (millis() - t < 15000)
    {
        if (SIM_SERIAL.find("+CMGS:")) // bat chuoi +CMGS: <mr>
        {
            DEBUG_SERIAL.println("SMS SENT OK");
            sim_read();
            return true;
        }
    }

    DEBUG_SERIAL.println("SMS FAILED");
    sim_read();
    return false;
}

/* ================= SETUP ================= */
void setup()
{
    DEBUG_SERIAL.begin(115200);
    DEBUG_SERIAL.println("\nSystem started");

    sim_power_on();

    SIM_SERIAL.begin(SIM_BAUDRATE, SERIAL_8N1, SIM_RX_PIN, SIM_TX_PIN);

    sim_cmd("AT");
    sim_cmd("ATI");
    sim_cmd("AT+CPIN?");
    sim_cmd("AT+CSQ");
    sim_cmd("AT+CREG?");
    sim_cmd("AT+COPS?");

    DEBUG_SERIAL.println("\nType SMS content and press ENTER:");
}

/* ================= LOOP ================= */
void loop()
{
    if (DEBUG_SERIAL.available())
    {
        String sms = DEBUG_SERIAL.readStringUntil('\n');
        sms.trim();

        if (sms.length() > 0)
        {
            sendSMS(PHONE_NUMBER, sms.c_str());
        }
    }
}
