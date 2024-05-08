#include <xc.h>
#include "I2C_EEPROM.h"
#define _XTAL_FREQ 16000000
#define EEPROM_Address_R 0xA1
#define EEPROM_Address_W 0xA0

void I2C_Master_Init(const unsigned long baud) {
  SSPCON = 0b00101000;
  SSPCON2 = 0;
  SSPADD = (_XTAL_FREQ/(4*baud))-1;
  SSPSTAT = 0;      // Read Only except 2 MSBs
  TRISC3 = 1;       // SCL
  TRISC4 = 1;       // SDA
}
void I2C_Master_Wait() {
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F)); // Only if the condition fails, the bus is idle
}
void I2C_Master_Start() {
  I2C_Master_Wait();
  SEN = 1;      // SSPCON2<0>
}
void I2C_Master_RepeatedStart() {
  I2C_Master_Wait();
  RSEN = 1;     // SSPCON2<1>
}
void I2C_Master_Stop() {
  I2C_Master_Wait();
  PEN = 1;      // SSPCON2<2>
}
unsigned char I2C_Master_Write(unsigned char data) {
  I2C_Master_Wait();
  SSPBUF = data;
  while(!SSPIF);    // Wait Until Completion
  SSPIF = 0;        // PIR1<3>
  return ACKSTAT;   // SSPCON2<6>
}
unsigned char I2C_Read_Byte(void) {
  I2C_Master_Wait();
  RCEN = 1;         // SSPCON2<3>
  while(!SSPIF);    // Wait Until Completion PIR1<3>
  SSPIF = 0;        // Clear The Interrupt Flag Bit
  I2C_Master_Wait();
  return SSPBUF;    // Return The Received Byte
}
void I2C_ACK(void) {
  ACKDT = 0;        // 0 -> ACK, SSPCON2<5>
  I2C_Master_Wait();
  ACKEN = 1;        // Send ACK, SSPCON2<4>
}
void I2C_NACK(void) {
  ACKDT = 1;        // 1 -> NACK, SSPCON2<5>
  I2C_Master_Wait();
  ACKEN = 1;        // Send NACK, SSPCON2<4>
}
void EEPROM_Write(unsigned int add, unsigned char data) {
  I2C_Master_Start();
  while(I2C_Master_Write(EEPROM_Address_W))     // If ACK from slave isn't received then repeated start
    I2C_Master_RepeatedStart();
//  I2C_Master_Write(EEPROM_Address_W);
//  I2C_ACK();
  I2C_Master_Write(add>>8);                   // register add MSB
//  I2C_ACK();
  I2C_Master_Write((unsigned char)add);       // register add LSB
//  I2C_ACK();
  I2C_Master_Write(data);                     // 1 byte data
//  I2C_ACK();
  I2C_Master_Stop();
}
// EEPROM Write Page
void EEPROM_Write_Page(unsigned int add, unsigned char* data, unsigned char len) {
  I2C_Master_Start();
  while(I2C_Master_Write(EEPROM_Address_W))     // If ACK from slave isn't received then repeated start
    I2C_Master_RepeatedStart();
//  I2C_Master_Write(EEPROM_Address_W);
  I2C_ACK();
  I2C_Master_Write(add>>8);                     // register add MSB 
  I2C_ACK();
  I2C_Master_Write((unsigned char)add);         // register add LSB
  I2C_ACK();
  for(unsigned int i=0; i<len; i++)
    I2C_Master_Write(data[i]);                  // len byte data written
  I2C_ACK();
  I2C_Master_Stop();
}
unsigned char EEPROM_Read(unsigned int add) {
  unsigned char Data;
  I2C_Master_Start();                       // Start bit
  while(I2C_Master_Write(EEPROM_Address_W)) // If no ACK from slave is received for A0(1010 0000) then repeated start for slave address
    I2C_Master_RepeatedStart();
//  I2C_Master_Write(EEPROM_Address_W);
//  I2C_ACK();
  I2C_Master_Write(add>>8);                 // MSB of register add
//  I2C_ACK();
  I2C_Master_Write((unsigned char)add);     // LSB of register add
//  I2C_ACK();
  I2C_Master_RepeatedStart();               // Repeated start to mention Read bit to get data from slave
  I2C_Master_Write(EEPROM_Address_R);       // same slave add with read bit enabled, A1(1010 0001)
//  I2C_ACK();
  Data = I2C_Read_Byte();                   // get the data
  I2C_NACK();
  I2C_Master_Stop();
  return (unsigned char)Data;
}
//---[ Sequential Read ]---
void EEPROM_Read_Page(unsigned int add, unsigned char* data, unsigned int len) {
  I2C_Master_Start();                       // Start bit
  while(I2C_Master_Write(EEPROM_Address_W)) // If no ACK from slave is received for A0(1010 0000) then repeated start for slave address
    I2C_Master_RepeatedStart();
  I2C_Master_Write(EEPROM_Address_W);
  I2C_ACK();
  I2C_Master_Write(add>>8);                 // MSB of register add
  I2C_ACK();
  I2C_Master_Write((unsigned char)add);   // LSB of register add
  I2C_ACK();
  I2C_Master_RepeatedStart();               // Repeated start to mention Read bit to get data from slave
  I2C_Master_Write(EEPROM_Address_R);       // same slave add with read bit enabled, A1(1010 0001);
  I2C_ACK();
  for(unsigned int i=0; i<len; i++) {
    if (i==len-1) {
        data[i] = I2C_Read_Byte();          // get array of data
        I2C_NACK();
    }
    else {
        data[i] = I2C_Read_Byte();          // get array of data
        I2C_ACK();
    }
 }I2C_NACK();
 I2C_Master_Stop();
}