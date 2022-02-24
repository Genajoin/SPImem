#include "SPImem.h"
#define WRITE_ADDRESS 0x00
#define CS_PIN GPIO4
#define DATA_LEN 100

SPImem flash(CS_PIN, &SPI1);
byte data[DATA_LEN];

void printBytes(uint32_t addr, byte *data, size_t len)
{
  Serial.print(addr, HEX);
  Serial.print(":");
  for (size_t i = 0; i < len; i++)
  {
    if (data[i] < 0x10)
      Serial.print('0');
    Serial.print(data[i], HEX);
  }
  Serial.println();
}

void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
    ;
  };
  flash.begin();
  unsigned int addr = WRITE_ADDRESS;

  Serial.println("Read");
  flash.NotBusy();
  flash.ReadBytes(addr, data, DATA_LEN);
  printBytes(addr, data, DATA_LEN);
  Serial.println("Erase");
  flash.SectorErase(WRITE_ADDRESS);
  Serial.println("Read");
  flash.NotBusy();
  flash.ReadBytes(addr, data, DATA_LEN);
  printBytes(addr, data, DATA_LEN);
  Serial.println("Write");
  flash.NotBusy();
  for (size_t i = 0; i < DATA_LEN; i++)
  {
    data[i] = i;
  }
  printBytes(addr, data, DATA_LEN);
  flash.WriteBytes(addr, data, DATA_LEN);
  Serial.println("Read");
  flash.NotBusy();
  flash.ReadBytes(addr, data, DATA_LEN);
  printBytes(addr, data, DATA_LEN);
}

void loop() {}