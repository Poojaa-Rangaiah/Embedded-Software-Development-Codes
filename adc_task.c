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
void cmd(unsigned char i);
void data(unsigned char i);
void output(unsigned int i);
void delay(unsigned int);
unsigned int m, d0, d1, d2, d3, hival, loval;
unsigned char raw[10]={"RAW DATA: "}, cal[10]={"CAL DATA: "}, x;
unsigned long voltage, value;       //without 'long' the voltage at 665 shown 0V and at 1023 shown 35V so there had a necessisity to add 65V from 665
void main(void) { 
    init();
    while(1) {
        ADCON0 |= 0x04;     //Start Of Conversion
        while(ADCON0&0x04); //To monitor if the conversion is done
        hival = ADRESH;
        loval = ADRESL;
        value = (hival<<2)+(loval>>6);
        voltage = (value*100)/1023;
        cmd(0x80);
        for(x=0;x<10;x++) {
            data(raw[x]);
        } output(value);
        cmd(0xC0);
        for(x=0;x<10;x++) {
            data(cal[x]);
        } output(voltage); 
        data('V');
        delay(1000);
    }
}
void init(void) {
    TRISA = 0x04;       //CH2 (AN2)
    TRISC = 0x00;
    TRISD = 0x00;
    ADCON0 = 0x91;      //(B7,B6) clock conversion rate; (B5,B4,B3) CH1 (AN1); (B0) Power ON A/D
    ADCON1 = 0x02;      //(B7) left justified; (B6) finalize clock conversion; (B3-B0) making ports analog and digital
    cmd(0x38);
    delay(100);
    cmd(0x38);
    delay(100);
    cmd(0x38);
    delay(100);
    cmd(0x38);          //function set
    delay(100);
    cmd (0x0C);         //LCD ON with (B3 - 1)
    delay(100);
    cmd (0x06);         //Right shift
    delay(100);
    cmd (0x01);         //Clear LCD
    delay(100);
}
void output(unsigned int n) {
    d3 = (unsigned int)(n/1000);
    d2 = (unsigned int)(n-(d3*1000))/100;
    d1 = (unsigned int)(n-(d3*1000)-(d2*100))/10;
    d0 = (unsigned int)(n-(d3*1000)-(d2*100)-(d1*10));
    if (d3>0) {
        data(0x30+d3);
        data(0x30+d2);
        data(0x30+d1);
        data(0x30+d0);
    }
    else if (d2>0) {
        data(' ');
        data(0x30+d2);
        data(0x30+d1);
        data(0x30+d0);
    }
    else if (d1>0) {
        data(' ');
        data(' ');
        data(0x30+d1);
        data(0x30+d0);
    }
    else if (d0>0) {
        data(' ');
        data(' ');
        data(' ');
        data(0x30+d0);
    }
    else {
        data(' ');
        data(' ');
        data(' ');
        data(0x30+d0);
    }
}
void cmd (unsigned char i) {
    PORTC &= 0xF7;
    PORTD = i;
    PORTC |= 0x01;
    PORTC &= ~(0x01);
    delay(100);
}
void data (unsigned char i) {
    PORTC |= 0x08;
    PORTD = i;
    PORTC |= 0x01;
    PORTC &= ~(0x01);
    delay(100);
}
void delay (unsigned int k) {
    while(--k);
}