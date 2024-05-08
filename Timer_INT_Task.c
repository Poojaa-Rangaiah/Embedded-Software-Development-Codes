#include <xc.h>
#define _XTAL_FREQ 6000000
void init(void);
unsigned char count = 0;
void main(void) {
    init();
    PORTC = 0x02;           //GREEN LED ON
    T1CON |= 0x01;          //TMR1ON
    while(1) {
        PORTD=0x08;         //RED LED ON
        __delay_ms(800);
        PORTD=0x00;         //RED LED OFF
        __delay_ms(200);
    }
}
void init(void) {
    TRISD = 0x00;
    PORTD = 0x00;
    TRISC = 0x00;
    PORTC = 0xFF;
    INTCON = 0xC0;          //En GIE & PEIE
    TMR1 = 3536;            //Pre-loaded TMR1 value
    T1CON  = 0x00;          //PS=1:1
    PIR1&=0xFE;             //Clearing the flag
    PIE1 = 0x01;            //En TMR1IE
}
void __interrupt() TIMER_INT_ISR() {
    if(PIR1&0x01) {         //checking for the TMR1IF enable
        count++;
        if(count==23) {     //clean value of no. of overflows
            PORTC=~PORTC;
            count=0;
        }
    }
    PIR1&=0xFE;             //Clearing the flag
}
