#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
//slave
#include <xc.h>
#define _XTAL_FREQ 6000000
void init(void);
void delay(unsigned int);
void cmd(unsigned char);
void data(unsigned char);
unsigned char val, x, TX[5]={"TX : "}, RX[5]={"RX : "};
void main(void) {
    init();                             //initialization
    while(1) {
        if(PIR1&0x20) {                 //To check if anything is received
            if(RCREG == 'A') {
                cmd(0x80);              //display received character
                for(x=0; x<5; x++) {
                    data(RX[x]);
                } data(RCREG);
                TXREG = 'a';            //To transmit in return for receiving
                cmd(0xC0);              //display transmitted character
                for(x=0; x<5; x++) {
                    data(TX[x]);
                } data(TXREG);
            }
            else if(RCREG == 'B') {
                cmd(0x80);
                for(x=0; x<5; x++) {
                    data(RX[x]);
                } data(RCREG);
                TXREG = 'b';
                cmd(0xC0);
                for(x=0; x<5; x++) {
                    data(TX[x]);
                } data(TXREG);
            }
            else if(RCREG == 'C') {
                cmd(0x80);
                for(x=0; x<5; x++) {
                    data(RX[x]);
                } data(RCREG);
                TXREG = 'c';
                cmd(0xC0);
                for(x=0; x<5; x++) {
                    data(TX[x]);
                } data(TXREG);
            }
            else if(RCREG == 'D') {
                cmd(0x80);
                for(x=0; x<5; x++) {
                    data(RX[x]);
                } data(RCREG);
                TXREG = 'd';
                cmd(0xC0);
                for(x=0; x<5; x++) {
                    data(TX[x]);
                } data(TXREG);
            }
        }
    }
}
void init(void) {
    TRISC = 0xC0;       //Initialize for TX and RX
    TRISD = 0x00;       //For LCD
    TXSTA = 0x20;       //Setup to transmit
    RCSTA = 0x90;       //Setup to receive and enables serial port for transmission
    SPBRG = 0x09;       //Baud Rate Generator
    cmd(0x38);
    delay(100);
    cmd(0x38);
    delay(100);
    cmd(0x38);
    delay(100);
    cmd(0x38);          //Function set
    delay(100);
    cmd(0x0C);          //LCD on with B3 = 1
    delay(100);
    cmd(0x06);          //Right shift command
    delay(100);
    cmd(0x01);          //clear LCD
    delay(100);
}
void cmd(unsigned char i) {
    PORTC &= 0xF7;      //RC3
    PORTD = i;
    PORTC |= 0x01;      //RC0
    PORTC &= ~(0x01);   //RC0
    delay(100);
}
void data(unsigned char i) {
    PORTC |= 0x08;      //RC3
    PORTD = i;
    PORTC |= 0x01;      //RC0
    PORTC &= ~(0x01);   //RC0
    delay(100);
}
void delay(unsigned int x) {
    while(--x);
}