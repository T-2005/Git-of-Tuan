#include <Arduino.h>
#include <HardwareSerial.h>

// RS485 UART pins
#define RXD2 16
#define TXD2 17

HardwareSerial rs485(2);

/* ============ CRC16 MODBUS ============ */
uint16_t modbusCRC(uint8_t *buf, uint8_t len)
{
  uint16_t crc = 0xFFFF;
  for (uint8_t pos = 0; pos < len; pos++)
  {
    crc ^= buf[pos];
    for (uint8_t i = 0; i < 8; i++)
    {
      if (crc & 0x0001)
      {
        crc >>= 1;
        crc ^= 0xA001;
      }
      else
        crc >>= 1;
    }
  }
  return crc;
}

/* ============ SEND READ COMMAND (0x03) ============ */
void readHoldingRegisters(uint8_t slave, uint16_t startReg, uint16_t regCount)
{
  uint8_t frame[8];

  frame[0] = slave;
  frame[1] = 0x03;
  frame[2] = highByte(startReg);
  frame[3] = lowByte(startReg);
  frame[4] = highByte(regCount);
  frame[5] = lowByte(regCount);

  uint16_t crc = modbusCRC(frame, 6);
  frame[6] = lowByte(crc);
  frame[7] = highByte(crc);

  // Debug: In ra frame gửi đi
  Serial.print("TX: ");
  for (int i = 0; i < 8; i++)
  {
    Serial.printf("%02X ", frame[i]);
  }
  Serial.println();

  rs485.write(frame, 8);
  rs485.flush();
}

/* ============ READ RESPONSE ============ */
bool readResponse(uint8_t *buf, uint8_t maxLen, uint8_t &actualLen)
{
  unsigned long t = millis();
  uint8_t i = 0;

  // Clear buffer trước
  while (rs485.available()) rs485.read();

  while (millis() - t < 1000)
  {
    if (rs485.available())
    {
      buf[i++] = rs485.read();
      
      // Nếu nhận đủ header, tính độ dài cần nhận
      if (i == 3 && buf[1] == 0x03)
      {
        maxLen = 5 + buf[2];  // slave + func + len + data + CRC(2)
      }
      
      if (i >= maxLen)
      {
        actualLen = i;
        
        // Debug: In ra frame nhận được
        Serial.print("RX: ");
        for (int j = 0; j < i; j++)
        {
          Serial.printf("%02X ", buf[j]);
        }
        Serial.println();
        
        // Verify CRC
        uint16_t receivedCRC = buf[i - 2] | (buf[i - 1] << 8);
        uint16_t calculatedCRC = modbusCRC(buf, i - 2);
        
        Serial.printf("CRC: Received=0x%04X, Calculated=0x%04X\n", 
                      receivedCRC, calculatedCRC);
        
        if (receivedCRC == calculatedCRC)
        {
          Serial.println("✓ CRC OK");
          return true;
        }
        else
        {
          Serial.println("❌ CRC Error!");
          return false;
        }
      }
    }
  }
  
  actualLen = i;
  if (i > 0)
  {
    Serial.print("RX (timeout): ");
    for (int j = 0; j < i; j++)
    {
      Serial.printf("%02X ", buf[j]);
    }
    Serial.println();
  }
  else
  {
    Serial.println("❌ No response (timeout)");
  }
  
  return false;
}

/* ============ TEST FUNCTIONS ============ */

// Test 1: Đọc địa chỉ slave
void test1_checkAddress()
{
  uint8_t rxBuf[20];
  uint8_t rxLen;
  
  Serial.println("\n=== TEST 1: Check Slave Address ===");
  
  readHoldingRegisters(0x01, 0x0000, 1);
  delay(100);
  
  if (readResponse(rxBuf, 20, rxLen))
  {
    uint16_t addr = (rxBuf[3] << 8) | rxBuf[4];
    Serial.printf("✓ G6-S Address: %d (0x%02X)\n", addr, addr);
  }
  else
  {
    Serial.println("❌ FAILED: No response");
    Serial.println("   -> Check: Wiring, Power, Baudrate");
  }
}

// Test 2: Đọc PM2.5 (1 register đơn giản)
void test2_readPM25()
{
  uint8_t rxBuf[20];
  uint8_t rxLen;
  
  Serial.println("\n=== TEST 2: Read PM2.5 ===");
  
  readHoldingRegisters(0x01, 0x0001, 1);
  delay(100);
  
  if (readResponse(rxBuf, 20, rxLen))
  {
    uint16_t pm25 = (rxBuf[3] << 8) | rxBuf[4];
    Serial.printf("✓ PM2.5: %d ug/m3\n", pm25);
  }
  else
  {
    Serial.println("❌ FAILED");
  }
}

// Test 3: Đọc nhiều registers (PM2.5, CO2, VOC, Temp, Humidity)
void test3_readAllSensors()
{
  uint8_t rxBuf[50];
  uint8_t rxLen;
  
  Serial.println("\n=== TEST 3: Read All Sensors ===");
  
  readHoldingRegisters(0x01, 0x0001, 5);
  delay(100);
  
  if (readResponse(rxBuf, 50, rxLen))
  {
    uint16_t pm25 = (rxBuf[3] << 8) | rxBuf[4];
    uint16_t co2  = (rxBuf[5] << 8) | rxBuf[6];
    uint16_t voc  = (rxBuf[7] << 8) | rxBuf[8];
    uint16_t temp = (rxBuf[9] << 8) | rxBuf[10];
    uint16_t hum  = (rxBuf[11] << 8) | rxBuf[12];

    Serial.println("✓ Success:");
    Serial.printf("  PM2.5    : %d ug/m3\n", pm25);
    Serial.printf("  CO2      : %d ppm\n", co2);
    Serial.printf("  VOC      : %.1f ppm\n", voc / 10.0);
    Serial.printf("  Temp     : %d °C\n", temp);
    Serial.printf("  Humidity : %d %%RH\n", hum);
  }
  else
  {
    Serial.println("❌ FAILED");
  }
}

// Test 4: Thử các địa chỉ slave khác (nếu bạn không chắc)
void test4_scanSlaveAddresses()
{
  Serial.println("\n=== TEST 4: Scan Slave Addresses ===");
  
  for (uint8_t addr = 1; addr <= 10; addr++)
  {
    uint8_t rxBuf[20];
    uint8_t rxLen;
    
    Serial.printf("Trying address %d (0x%02X)... ", addr, addr);
    
    readHoldingRegisters(addr, 0x0001, 1);
    delay(100);
    
    if (readResponse(rxBuf, 20, rxLen))
    {
      Serial.println("✓ FOUND!");
      return;
    }
    else
    {
      Serial.println("No response");
    }
    
    delay(200);
  }
  
  Serial.println("No device found in range 1-10");
}

void setup()
{
  Serial.begin(115200);
  delay(2000);
  
  // Khởi tạo RS485
  rs485.begin(9600, SERIAL_8N1, RXD2, TXD2);

  Serial.println("\n========================================");
  Serial.println("  G6-S RS485 Communication Test");
  Serial.println("========================================");
  Serial.println("Baudrate: 9600 bps");
  Serial.println("Format: 8N1 (8 data, No parity, 1 stop)");
  Serial.println("Protocol: Modbus RTU");
  Serial.println("========================================\n");

  delay(1000);

  // Chạy các test
  test1_checkAddress();
  delay(2000);
  
  test2_readPM25();
  delay(2000);
  
  test3_readAllSensors();
  delay(2000);
  
  // Nếu các test trên thất bại, thử scan
  // test4_scanSlaveAddresses();
}

void loop()
{
  // Sau khi test xong, chạy đọc liên tục
  uint8_t rxBuf[50];
  uint8_t rxLen;
  
  Serial.println("\n--- Reading G6-S Data ---");
  
  readHoldingRegisters(0x01, 0x0001, 5);
  delay(100);
  
  if (readResponse(rxBuf, 50, rxLen))
  {
    uint16_t pm25 = (rxBuf[3] << 8) | rxBuf[4];
    uint16_t co2  = (rxBuf[5] << 8) | rxBuf[6];
    uint16_t voc  = (rxBuf[7] << 8) | rxBuf[8];
    uint16_t temp = (rxBuf[9] << 8) | rxBuf[10];
    uint16_t hum  = (rxBuf[11] << 8) | rxBuf[12];

    Serial.printf("PM2.5: %d | CO2: %d | VOC: %.1f | T: %d°C | H: %d%%\n", 
                  pm25, co2, voc/10.0, temp, hum);
  }
  
  delay(5000);  // Đọc mỗi 5 giây
}
// ```

// ## Các vấn đề thường gặp và cách khắc phục:

// ### ❌ **Vấn đề 1: Không nhận được response**
// **Nguyên nhân:**
// - Sai cực tính A+/B-
// - Chưa GND chung
// - Sai baudrate
// - G6-S chưa được cấp nguồn
// - Địa chỉ slave không đúng

// **Giải pháp:**
// ```
// 1. Thử đổi A+ và B- (swap them)
// 2. Đảm bảo GND của ESP32, RS485, G6-S nối chung
// 3. Confirm baudrate 9600
// 4. Kiểm tra G6-S có đèn LED báo hoạt động không
// 5. Thử scan địa chỉ với test4_scanSlaveAddresses()
// ```

// ### ❌ **Vấn đề 2: Nhận được data nhưng CRC sai**
// **Nguyên nhân:**
// - Nhiễu trên đường truyền
// - Dây quá dài không có termination resistor

// **Giải pháp:**
// ```
// 1. Thêm điện trở 120Ω giữa A+ và B- (nếu dây >3m)
// 2. Dùng dây xoắn đôi (twisted pair)
// 3. Tách xa nguồn nhiễu (động cơ, relay, etc.)