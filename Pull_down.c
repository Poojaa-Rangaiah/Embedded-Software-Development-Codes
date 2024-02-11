// PIC16F877A Configuration Bit Settings

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
unsigned char val=0;    //val ranges between 0 to 255
void main(void) {
    TRISC = 0x0F;   //setting C as the input port
    TRISD = 0x00;   //setting D as the output port
    PORTC = 0x00;   //initializing PORTC is 0
    PORTD = 0x00;   //initializing PORTD is 0
    while(1) {
        val = PORTC;    //checking the PORTC for input
        switch(val) {
            case 0x01:          //checking if RC0 is pressed
                PORTD = 0x10;   // RD4 = 1 and RD2 = 0
                break;          //break the loop
            case 0x02:          //checking if RC1 is pressed
                PORTD = 0x14;   // RD4 = 1 and RD2 = 1
                break;          //break the loop
            case 0x04:          //checking if RC2 is pressed
                PORTD = 0x04;   // RD4 = 0 and RD2 = 1
                break;          //break the loop
            case 0x08:          //checking if RC3 is pressed
                PORTD = 0x00;   // RD4 = 0 and RD2 = 0
                break;          //break the loop
        }
    }
}
