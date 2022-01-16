#include <SPImem.h>

  SPImem m;


void setup() {
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext,LOW);
  SPI.begin();
  m.SectorErase(0x01);
  m.WriteByte(0x01, 0xAA);
  byte b = m.ReadByte(0x01);  
}

void loop() {}
