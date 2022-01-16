#include <SPImem.h>

  //SPImem m;


void setup() {
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext,LOW);
  SPI.begin();
  flash.SectorErase(0x01);
  flash.WriteByte(0x01, 0xAA);
  byte b = flash.ReadByte(0x01);  
}

void loop() {}
