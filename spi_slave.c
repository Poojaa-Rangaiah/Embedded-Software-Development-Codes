//#include <xc.h>
//#define _XTAL_FREQ 4000000
//void slave_init(void);
//char spi_read(void);
//char data;
//void main(void) {
//    slave_init();
//    TRISB = 0x00;
//    while(1) {
//        spi_read();
//        PORTB = data;
//    }
//}
//void slave_init(void) {
//    SSPCON|=0x20;
//    TRISC = 0x18;
//    ADCON1 = 0x04;   
//}
//char spi_read() {
//    char data;
//    if(SSPSTAT&0x01) {
//        data = SSPBUF;
//        PIR1&=0xF7;
//        SSPCON &= ~(0x40);
//        return data;
//    }
//}
//void __interrupt() ISR(void) {
//    if(PIR1&0x08) {
//        data = SSPBUF;
//        PIR1&=~(0x08);
//    }
//}

#include <xc.h> 
void SPI_Slave_Init();
char SPI_Read();
char Data;
void main(void) {
  SPI_Slave_Init();
  TRISB = 0x00;
  while(1) {
    PORTB = Data;
  }
  return;
}
void SPI_Slave_Init() {
  SSPM0 = 0;
  SSPM1 = 0;
  SSPM2 = 1;
  SSPM3 = 0;
  SSPEN = 1;
  CKP = 0;
  CKE = 0;
  SMP = 0;      // Sample bit
  TRISC5 = 0;   // SDO 
  TRISC4 = 1;   // SDI 
  TRISC3 = 1;   // SCK 
  PCFG3 = 0;    // ADCON1
  PCFG2 = 1;
  PCFG1 = 0;
  PCFG0 = 0;
  TRISA5 = 1;   // SS
  SSPIE = 1;
  PEIE = 1;
  GIE = 1;
}
//char SPI_Read() {
//  char Data;
//  if(BF) {
//    Data = SSPBUF;
//    BF = 0;
//    SSPIF = 0;
//    SSPOV = 0;
//    return Data;
//  }
//}
void __interrupt() ISR(void) {
  if(SSPIF) {
    Data = SSPBUF;
    SSPIF = 0;
  }
}