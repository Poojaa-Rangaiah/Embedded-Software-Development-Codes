//#include <xc.h>
//#define _XTAL_FREQ 4000000
//#define UP RB0
//#define DOWN RB1
//#define SEND RB2
//void master_init(void);
//void spi_write(char);
//void main(void) {
//    master_init();
//    char data=0;
//    TRISB = 0x07;
//    TRISD = 0x00;
//    PORTD = 0x00;
//    while(1) {
//        if(UP){
//            data++;
//            __delay_ms(250);
//        }
//        if(DOWN){
//            data--;
//            __delay_ms(250);
//        }
//        if(SEND){
//            spi_write(data);
//            __delay_ms(250);
//        }
//        PORTD = data;
//    }
//}
//void master_init(void) {
//    SSPCON = 0x20;
//    TRISC = 0x10;
//}
//void spi_write(char data) {
//    SSPBUF = data;
//}
 
#include <xc.h>
#define _XTAL_FREQ 4000000
#define UP RB0
#define Down RB1
#define Send RB2
void SPI_Master_Init();
void SPI_Write(char);
void main(void) {
  SPI_Master_Init();
  char Data = 0;
  TRISB = 0x07;
  TRISD = 0x00;
  PORTD = 0x00;
  while(1) {
    if (UP) {    // Increment
      Data++;
      __delay_ms(250);
    }
    if (Down) {  // Decrement
      Data--;
      __delay_ms(250);
    }
    if (Send) {   // Send the Data
      SPI_Write(Data);
      __delay_ms(250);
    }
    PORTD = Data;
  }
} 
void SPI_Master_Init() {
  SSPM0 = 0;
  SSPM1 = 0;
  SSPM2 = 0;
  SSPM3 = 0;
  SSPEN = 1;
  CKP = 0;
  CKE = 0;
  SMP = 0;
  TRISC5 = 0;   // SDO
  TRISC4 = 1;   // SDI
  TRISC3 = 0;   // SCK
  SSPIE = 1; 
  PEIE = 1; 
  GIE = 1;
}
void SPI_Write(char Data) {
  SSPBUF = Data;
}