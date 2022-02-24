#include "SPImem.h"
#define WRITE_ADDRESS 0x01
#define CS_PIN GPIO10

SPImem flash(CS_PIN, &SPI1);

void printByte(uint32_t addr)
{
  Serial.print(addr, HEX);
  Serial.print(":");
  Serial.println(flash.ReadByte(addr), HEX);
}

void setup()
{
  Serial.begin(115200);
  SPI1.begin(SCK1, MISO1, MOSI1, CS_PIN);
  while (!Serial)
  {
    ;
  };
}

void loop()
{
  if (Serial.available() == 0)
    return;

  char c = Serial.read();
  uint16_t rxdata;

  switch (c)
  {
  case 48 ... 57:
    flash.WriteByte(c - 48, millis() % 2 ? 0xcc : 0xbb);
    break;
  case 'e':
    flash.SectorErase(WRITE_ADDRESS);
    break;
  case 'w':
    flash.WriteByte(WRITE_ADDRESS, 0xAA);
    break;
  case 'r':
    printByte(WRITE_ADDRESS);
    Serial.println(rxdata);
    break;
  case 'b':
    flash.NotBusy();
    break;
  }
}
