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
#define _XTAL_FREQ 6000000
void init(void);
void pwmchange(void);
unsigned char pwmH20, pwmL20, pwmH60, pwmL60, pwmH90, pwmL90;
void main(void) {
    init();             //Initialization
    while(1) {
        pwmchange();    //PWM
    }
}
void init() {
    TRISC = 0xFD;   //PORTC<1>
    CCP2CON = 0x3C; //Random value only for MSB
    CCPR2L = 0x2E;  //Random value
    T2CON = 0x06;   //TMR2_PRESCALER_VALUE
    PR2 = 0x2E;     //Time Period
    pwmH20 = 0x09;  //CCPR2L (8 bits)
    pwmL20 = 0x20;  //CCP2CON<5:4>
    pwmH60 = 0x1C;  //CCPR2L (8 bits)
    pwmL60 = 0x10;  //CCP2CON<5:4>
    pwmH90 = 0x2A;  //CCPR2L (8 bits)
    pwmL90 = 0x10;  //CCP2CON<5:4>
}
void pwmchange() {
    CCPR2L = pwmH20;                    //CCPR2L (8 bit)
    CCP2CON = (CCP2CON&0xCF)|pwmL20;    //CCP2CON<5:4>
    __delay_ms(3000);                    
    CCPR2L = pwmH60;                    //CCPR2L (8 bit)
    CCP2CON = (CCP2CON&0xCF)|pwmL60;    //CCP2CON<5:4>
    __delay_ms(3000);
    CCPR2L = pwmH90;                    //CCPR2L (8 bit)
    CCP2CON = (CCP2CON&0xCF)|pwmL90;    //CCP2CON<5:4>
    __delay_ms(3000);
}