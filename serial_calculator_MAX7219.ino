/*
  Pinovi:
  
  |  Arduino   | MAX7219  |
  | ---------- | -------- |
  | MOSI (D11) | DIN      |
  | SCK (D13)  | CLK      |
  | I/O (D7)   | LOAD/CS  |
  
  by: Robert Dokša
 */
#include <SPI.h>
#define LOAD_PIN 7

uint32_t num1;    // prvi broj
uint32_t num2;    // drugi broj
long rez;        // rezultat
char op;         // operacija
bool error = 0;

uint8_t jed, des, sto, tis, dt, st, m, dm;

void setup() 
{
  pinMode(LOAD_PIN, OUTPUT);    // LOAD -> OUTPUT
  SPI.setBitOrder(MSBFIRST);    // Most Significant Bit first
  SPI.begin();                  // Pokreni SPI
  Serial.begin(9600);           // Pokreni serial comms

  dataTransfer(0x0F, 0x00);
  dataTransfer(0x09, 0xFF);      // Mode B
  dataTransfer(0x0A, 0x00);      // Intenzitet -> 1/32 ciklusa rada
  dataTransfer(0x0B, 0x07);      // Omogući sve znamenke
  dataTransfer(0x0C, 0x01);      // Uključi chip 
}

void loop()
{
  for(uint8_t i=1; i<9; i++)
  {
    dataTransfer(i, 0x0F);       // Clearaj sve znamenke
  }
  num1Func();
  opFunc();
  num2Func();
  rezFunc();        // Izračun
  printSer();      // Ispis u serial monitor
  printBroj(rez);
  
  rez=0;
  num1=0;
  num2=0;
  delay(2500);
}

void dataTransfer(uint8_t address, uint8_t value) 
{
  digitalWrite(LOAD_PIN, LOW);      // LOAD -> LOW
  SPI.transfer(address);            // Posalji registru adresu
  SPI.transfer(value);              // Posalji value
  digitalWrite(LOAD_PIN, HIGH);     // LOAD -> HIGH
}

void rezFunc()
{
  switch(op)
  {
    case '+':
      rez = num1 + num2;
      break;
    case '-':
      rez = num1 - num2;
      break;
    case '*':
      rez = num1 * num2;
      break;
    case '/':
      rez = num1 / num2;
      break;
    default:
      error = 1;
  }
}

void printSer()
{
  if(error == 1)
  {
    Serial.println("ERROR");
  }
  else
  {
    Serial.print(num1);
    Serial.print(" ");
    Serial.print(op);
    Serial.print(" ");
    Serial.print(num2);
    Serial.print(" = ");
    Serial.print(rez);
    Serial.println();

    dataTransfer(0x01, rez);
  }
  error = 0;
}

void num1Func()
{
  while(Serial.available() == 0){}
  
  num1 = Serial.parseInt();
  printBroj(num1);
}

void num2Func()
{
  while(Serial.available() == 0){}
  
  num2 = Serial.parseInt();
  Serial.println(num2);
  printBroj(num2);
  delay(1000);
}

void opFunc()
{
  while(Serial.available() == 0){}
  
  op = Serial.read();
  Serial.println(op);
}

void printBroj(uint32_t num)
{
  if(num < 10)
  {
    for(uint8_t i=2;i<=8;i++)
    {
      dataTransfer(i, 0x0F);
    }
    dataTransfer(0x01, num);
  }
  else if(num > 9 and num < 100)
  {
    for(uint8_t i=3;i<=8;i++)
    {
      dataTransfer(i, 0x0F);
    }
    jed = num % 10;
    des = (num / 10U) % 10;
    dataTransfer(0x01, jed);
    dataTransfer(0x02, des);
    
  }
  else if(num > 99 and num < 1000)
  {
    for(uint8_t i=4;i<=8;i++)
    {
      dataTransfer(i, 0x0F);
    }
    jed = num % 10;
    des = (num / 10U) % 10;
    sto = (num / 100U) % 10;
    dataTransfer(0x01, jed);
    dataTransfer(0x02, des);
    dataTransfer(0x03, sto);
  }
  else if(num > 999 and num < 10000)
  {
    for(uint8_t i=5;i<=8;i++)                          
    {
      dataTransfer(i, 0x0F);
    }
    jed = num % 10;
    des = (num / 10U) % 10;
    sto = (num / 100U) % 10;
    tis = (num / 1000U) % 10;
    dataTransfer(0x01, jed);
    dataTransfer(0x02, des);
    dataTransfer(0x03, sto);
    dataTransfer(0x04, tis);
  }
  else if(num > 9999 and num < 100000)
  {
    for(uint8_t i=6;i<=8;i++)
    {
      dataTransfer(i, 0x0F);
    }
    jed = num % 10;
    des = (num / 10U) % 10;
    sto = (num / 100U) % 10;
    tis = (num / 1000U) % 10;
    dt = (num / 10000U) % 10;
    dataTransfer(0x01, jed);
    dataTransfer(0x02, des);
    dataTransfer(0x03, sto);
    dataTransfer(0x04, tis);
    dataTransfer(0x05, dt);
  }
  else if(num > 99999 and num < 1000000)
  {
    for(uint8_t i=7;i<=8;i++)
    {
      dataTransfer(i, 0x0F);
    }
    jed = num % 10;
    des = (num / 10U) % 10;
    sto = (num / 100U) % 10;
    tis = (num / 1000U) % 10;
    dt = (num / 10000U) % 10;
    st = (num / 100000U) % 10;
    dataTransfer(0x01, jed);
    dataTransfer(0x02, des);
    dataTransfer(0x03, sto);
    dataTransfer(0x04, tis);
    dataTransfer(0x05, dt);
    dataTransfer(0x06, st);
  }
  else if(num > 999999 and num < 10000000)
  {
    dataTransfer(0x08, 0x0F);
    jed = num % 10;
    des = (num / 10U) % 10;
    sto = (num / 100U) % 10;
    tis = (num / 1000U) % 10;
    dt = (num / 10000U) % 10;
    st = (num / 100000U) % 10;
    m = (num / 1000000U) % 10;
    dataTransfer(0x01, jed);
    dataTransfer(0x02, des);
    dataTransfer(0x03, sto);
    dataTransfer(0x04, tis);
    dataTransfer(0x05, dt);
    dataTransfer(0x06, st);
    dataTransfer(0x07, m);
  }
  else if(num > 9999999 and num < 100000000)
  {
    jed = num % 10;
    des = (num / 10U) % 10;
    sto = (num / 100U) % 10;
    tis = (num / 1000U) % 10;
    dt = (num / 10000U) % 10;
    st = (num / 100000U) % 10;
    m = (num / 1000000U) % 10;
    dm = (num / 10000000U) % 10;
    dataTransfer(0x01, jed);
    dataTransfer(0x02, des);
    dataTransfer(0x03, sto);
    dataTransfer(0x04, tis);
    dataTransfer(0x05, dt);
    dataTransfer(0x06, st);
    dataTransfer(0x07, m);
    dataTransfer(0x08, dm);
  }
  else if(num > 99999999)
  {
    for(uint8_t i=1;i<=8;i++)
    {
      dataTransfer(i, 0x0A);
    }
  }
}

