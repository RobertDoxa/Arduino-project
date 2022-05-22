/*
  Pinovi:
  
  |  Arduino   |     MAX7219      |
  | ---------- | ---------------- |
  | MOSI (D11) | DIN (D1)         |
  | SCK (D13)  | CLK (D13)        |
  | I/O (D7)   | LOAD/CS (D12)    |
  
  by: Robert Dokša
  
 */
#include <SPI.h>
#define LOAD_PIN 7

void maxTransfer(uint8_t address, uint8_t value) 
{
  digitalWrite(LOAD_PIN, LOW);      // LOAD -> LOW
  SPI.transfer(address);            // Posalji registru adresu
  SPI.transfer(value);              // Posalji value
  digitalWrite(LOAD_PIN, HIGH);     // LOAD -> HIGH
}

void setup() 
{
  pinMode(LOAD_PIN, OUTPUT);     // LOAD -> OUTPUT
  SPI.setBitOrder(MSBFIRST);     // Most Significant Bit first
  SPI.begin();                   // Pokreni SPI
  Serial.begin(9600);            // Pokreni serial comms

  maxTransfer(0x0F, 0x00);
  maxTransfer(0x09, 0xFF);  // Mode B
  maxTransfer(0x0A, 0x00);  // Intenzitet -> 1/32 ciklusa rada
  maxTransfer(0x0B, 0x03);  // Omogući 4 znamenke
  maxTransfer(0x0C, 0x01);  // Uključi chip
}

void loop()
{
  maxTransfer(0x04, 0x00);                // Tisucica = 0 by default
  for(int L=0x01; L<=0x06; L++)
  {
    maxTransfer(0x03, 0x00);              // Stotica = 0 by default
    for(int k=0x01; k<=0x0A; k++)
    {
      maxTransfer(0x02, 0x00);            // Desetica = 0 by default
      for(int j=0x01; j<=0x06; j++)
      {
        for (int i=0x00; i<=0x09; i++)
        {
          maxTransfer(0x01, i);           // Seconds count (0-9)
          delay(1000);
        }
        maxTransfer(0x02, j);             // Desetica count (0-5)
      }
      maxTransfer(0x03, k);               // Stotica count (0-9)
    }
    maxTransfer(0x04, L);                 // Tisucica count (0-5)
  }
}
