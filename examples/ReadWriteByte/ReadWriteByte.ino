#include "SPImem.h"
#define WRITE_ADDRESS 0x01
#define CS_PIN GPIO1

SPImem flash(CS_PIN, &SPI);

void printByte(uint32_t addr)
{
  Serial.print(addr, HEX);
  Serial.print(":");
  Serial.println(flash.ReadByte(addr), HEX);
}

void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
    ;
  };
  flash.begin();
  printByte(WRITE_ADDRESS);
  flash.SectorErase(WRITE_ADDRESS);
  flash.NotBusy();
  printByte(WRITE_ADDRESS);
  flash.WriteByte(WRITE_ADDRESS, 0xAA);
  flash.NotBusy();
  printByte(WRITE_ADDRESS);
}

void loop() {}