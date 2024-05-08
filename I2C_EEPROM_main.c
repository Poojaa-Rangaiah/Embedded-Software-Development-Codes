// CONFIG
#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "I2C_EEPROM.h"
#define _XTAL_FREQ 16000000
 
void main(void) {
  I2C_Master_Init(100000);
  unsigned int Add1=0x0020, Add2=0x0028, Add3=0x0037;    // Some Random Address
  unsigned char Data1=0x05, Data2='2', Data3='c';        // Some Random Data To Write
 
  EEPROM_Write(Add1, Data1);       // Write 0x05 @ 0x0020
  __delay_ms(10);
  EEPROM_Write(Add2, Data2);       // Write '2' @ 0x0028
  __delay_ms(10);
  EEPROM_Write(Add3, Data3);       // Write 'c' @ 0x0037
  __delay_ms(10);                   // Wait tWR=10ms For Write To Complete

  TRISD = 0x00;
  PORTD = EEPROM_Read(Add1);      // Should Read 0x04
  __delay_ms(1000);
  PORTD = EEPROM_Read(Add2);      // Should Read 0x05
  __delay_ms(1000);
  PORTD = EEPROM_Read(Add3);      // Should Read 0x06
  __delay_ms(1000);
 
  while(1)
  {
     // Stay IDLE .. DO Nothing!
  }
  return;
}