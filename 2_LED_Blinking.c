// 'C' source line config statements

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
#define _XTAL_FREQ 6000000  //Clock frequency of PIC16f877A
void main()
{
	TRISD=0x7B; //Sets the 2nd and 7th pin of PORTD as output while the rest as input.
    while(1)
    {
        PORTD=0x04;     // LED at RD7 = 0 and RD2 = 1
        __delay_ms(3000);//delays the command for 3sec
        PORTD=0x80;     // LED at RD7 = 1 and RD2 = 0
        __delay_ms(3000);//delays the command for 3sec
        PORTD=0x84;     // LED at RD7 = 1 and RD2 = 1
        __delay_ms(3000);//delays the command for 3sec
        PORTD=0x00;     // LED at RD7 = 0 and RD2 = 0
        __delay_ms(3000);//delays the command for 3sec
    }
}
//void delay (unsigned int i) {
//    while(--i);
//}
//void main(void) {
//    TRISC = 0xFE;
//    while(1) {
//        RC0 = 1;
//        delay(1000);
//        RC0 = 0;
//        delay(1000);
//    }
//}