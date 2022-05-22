/*
  Približavanjem predmeta ultrazvučnom senzoru udaljenosti, brzina vrtnje step motora raste.
  Također, udaljenost je prikazana na 7-segmentnom displayu.
  Maksimalna udaljenost: 50 cm
  Minimalna udaljenost: 2 cm
 
  Korištene komponente: HC-SR04 senzor udaljenosti,
                        28BYL-48 step motor (ULN2003 driver),
                        MAX7219 7-segmentni LED display.
  Pinovi:

  |  Arduino   | MAX7219  |
  | ---------- | -------- |
  | MOSI (D11) | DIN      |
  | SCK (D13)  | CLK      |
  | I/O (D7)   | LOAD/CS  |

  by: Robert Dokša
*/
#include <SPI.h>

#define A 2
#define B 3
#define C 4
#define D 5

#define LOAD_PIN 7

#define ECH 9
#define TRG 8

int distance, value;
double duration;

void setup() 
{
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(TRG, OUTPUT);
  pinMode(ECH, INPUT);
  
  pinMode(LOAD_PIN, OUTPUT);    // LOAD -> OUTPUT
  SPI.setBitOrder(MSBFIRST);    // Most Significant Bit first
  SPI.begin();                  // Pokreni SPI
  Serial.begin(9600);           // Pokreni serial comms

  dataTransfer(0x0F, 0x00);
  dataTransfer(0x09, 0xFF);      // Mode B
  dataTransfer(0x0A, 0x00);      // Intenzitet -> 1/32 ciklusa rada
  dataTransfer(0x0B, 0x03);      // Omogući 4 znamenke
  dataTransfer(0x0C, 0x01);      // Uključi chip
}

void loop() 
{
  value = dist();
  calculation(value);
  int motorSpeed = map(value, 1, 50, 1, 5);
  if (motorSpeed > 0) 
  {
    spin(motorSpeed);
    delay(1);
  } 
}

void dataTransfer(uint8_t address, uint8_t data) 
{
  digitalWrite(LOAD_PIN, LOW);
  SPI.transfer(address);
  SPI.transfer(data);
  digitalWrite(LOAD_PIN, HIGH);
}

void calculation(uint8_t num)
{
    int i = 1, j = 0;
    while(num > 0)
    {
        int mod = num % 10;
        num /= 10;
        dataTransfer(i, mod);
        i++;
        j++;
    }
    dataTransfer(i, num);
    dataTransfer(0x0B, j-1);
}

double dist()
{
  digitalWrite(TRG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRG, LOW);
  duration = pulseIn(ECH, HIGH);
  distance = duration * 0.034 / 2;
  if (distance>50) return 50;
  else return distance;
}

void stepControl(int a, int b, int c, int d)
{
  digitalWrite(A, a);
  digitalWrite(B, b);
  digitalWrite(C, c);
  digitalWrite(D, d);
}

void spin(int n){
  stepControl(1,0,0,0);
  delay(n);
  stepControl(1,0,0,1);
  delay(n);
  stepControl(0,0,0,1);
  delay(n);
  stepControl(0,0,1,1);
  delay(n);
  stepControl(0,0,1,0);
  delay(n);
  stepControl(0,1,1,0);
  delay(n);
  stepControl(0,1,0,0);
  delay(n);
  stepControl(1,1,0,0);
  delay(n);
}

