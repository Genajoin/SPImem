#include "SPImem.h"
#define WRITE_ADDRESS 0x01
#define CS_PIN GPIO10
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

void printByte(uint32_t addr)
{
  Serial.print(addr, HEX);
  Serial.print(":");
  Serial.println(flash.ReadByte(addr), HEX);
}

void setup()
{
  // pinMode(Vext,OUTPUT);
  // digitalWrite(Vext,LOW);
  Serial.begin(115200);
  SPI1.begin(SCK1, MISO1, MOSI1, CS_PIN);
  SPI1.setFrequency(6000000);
  while (!Serial)
  {
    ;
  };
  Serial.println("Send command to Serial:");
  Serial.println("  e - Erase sector;");
  Serial.println("  w - write one byte;");
  Serial.println("  r - read one byte;");
  Serial.println("  b - wait for write complete;");
  Serial.println("  s - write a part of bytes;");
  Serial.println("  p - read a part of bytes;");
  Serial.println("  [0-9] - write one byte at address of shift [0-9] of memory sector 1.");
  Serial.println("Sample: eb1bp - erase, wait, write one byte at address 1, wait  read and print bytes of first sector.");
  Serial.println("");
}

void loop()
{
  if (Serial.available() == 0)
    return;

  char c = Serial.read();

  switch (c)
  {
  case 48 ... 57: // write one byte at Num position
    flash.WriteByte(c - 48, millis() % 2 ? 0xcc : 0xbb);
    break;
  case 'e': // erase
    flash.SectorErase(WRITE_ADDRESS);
    break;
  case 'w': // write one byte
    flash.WriteByte(WRITE_ADDRESS, 0xAA);
    break;
  case 'r': // read one byte
    printByte(WRITE_ADDRESS);
    break;
  case 'b': // wait for write complete
    flash.NotBusyWait();
    break;
  case 's': // write a part of bytes
    for (size_t i = 0; i < DATA_LEN; i++)
    {
      data[i] = i;
    }
    flash.WriteBytes(WRITE_ADDRESS, data, DATA_LEN);
    break;
  case 'p': // read a part of bytes
    flash.ReadBytes(WRITE_ADDRESS, data, DATA_LEN);
    printBytes(WRITE_ADDRESS, data, DATA_LEN);
    break;
  }
}
