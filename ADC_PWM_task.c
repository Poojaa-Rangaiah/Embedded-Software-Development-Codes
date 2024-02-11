#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
#include <xc.h>
#define _XTAL_FREQ 6000000
void init(void);
void cmd(unsigned char);
void data(unsigned char);
void output(unsigned int);
void delay(unsigned int);
unsigned char n, ch0[7]={"CH0 : "}, ch1[7]={"CH1 : "};
unsigned long loval, hival, volt, val;
unsigned int d3, d2, d1, d0;
void main(void) {
    init();                                        //initialization
    while(1) {
        ADCON0 = 0x81;                             //Setting up ADC
        ADCON0 |= 0x04;                            //SOC
        while(ADCON0&0x04);                        //monitoring ADC
        loval = ADRESL;                            //Lower byte result
        hival = ADRESH;                            //Higher byte result
        val = (hival<<8) + loval;
        cmd(0x80);
        for(n=0; n<6; n++) {
            data(ch0[n]);
        } output(val);                             //CH0 voltage display
        ADCON0 = 0x91;                             //To select the clock conversion rate (B7,B6); CH1 (AN2) (B5,B4,B3) and to power up A/D (B0)
        ADCON1 = 0x82;                             //Right justified (B7), clock conversion rate (B6), selecting ports to be Digital except AN2 (B3 - B0)
        ADCON0 |= 0x04;                            //SOC
        while(ADCON0&0x04);                        //monitoring ADC
        loval = ADRESL;                            //Lower byte result
        hival = ADRESH;                            //Higher byte result
        val = (hival<<8) + loval;                      
        volt = (val*338)/1023;                     //Calibrating voltage for CH1
        cmd(0xC0);
        for(n=0; n<6; n++) {
            data(ch1[n]);
        } output(volt);                            //CH1 voltage display
        CCPR1L = (volt>>2);                        //storing 10 bit of data
        CCP1CON = (CCP1CON&0xCF)+((val&0x03)<<4);  //storing <5:4>
        delay(1000);
    }
}
void init(void) {
    TRISC = 0x00;           //Configure PORTC as output
    TRISD = 0x00;           //Configure PORTD as output
    TRISA = 0x05;           //Configure PORTA(AN0 & AN2) as input
    ADCON0 = 0x81;          //To select the clock conversion rate (B7,B6); CH (AN0 & AN2) (B5,B4,B3) and to power up A/D (B0)
    ADCON1 = 0x8E;          //Right justified (B7), clock conversion rate (B6), selecting all other ports to be Digital except AN0 (analog) (B3 - B0)
    CCP1CON = 0x3C;         //Random value only for MSB and LSB is for PWM mode, configuring Duty cycle register
    CCPR1L = 0x2E;          //Random value
    T2CON = 0x06;           //Configuring time period register
    PR2 = 0x5D;             //Time period register
    cmd(0x38);
    delay(100);
    cmd(0x38);
    delay(100);
    cmd(0x38);
    delay(100);
    cmd(0x38);              //Function Set command
    delay(100);
    cmd(0x0C);              //Display ON
    delay(100);
    cmd(0x06);              //Right shift
    delay(100);
    cmd(0x01);              //clear LCD
    delay(100);
}
void output(unsigned int n) {       //integer to char conversion
    d3 = (unsigned int)(n/1000);
    d2 = (unsigned int)(n-(d3*1000))/100;
    d1 = (unsigned int)(n-(d3*1000)-(d2*100))/10;
    d0 = (unsigned int)(n-(d3*1000)-(d2*100)-(d1*10));
    if (d3>0) {
        data(0x30+d3); data(0x30+d2);
        data(0x30+d1); data(0x30+d0);
    }
    else if (d2>0) {
        data(' '); data(0x30+d2);
        data(0x30+d1); data(0x30+d0);
    }
    else if (d1>0) {
        data(' '); data(' ');
        data(0x30+d1); data(0x30+d0);
    }
    else if (d0>0) {
        data(' '); data(' ');
        data(' '); data(0x30+d0);
    }
    else {
        data(' '); data(' ');
        data(' '); data(0x30+d0);
    }
}
void cmd(unsigned char i) {     //LCD command
    PORTC &= 0xF7;      //RC3
    PORTD = i;
    PORTC |= 0x01;      //RC0
    PORTC &= ~(0x01);   //RC0
    delay(100);
}
void data(unsigned char i) {    //LCD data
    PORTC |= 0x08;      //RC3
    PORTD = i;
    PORTC |= 0x01;      //RC0
    PORTC &= ~(0x01);   //RC0
    delay(100);
}
void delay(unsigned int x) {    //Delay
    while(--x);
}