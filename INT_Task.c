#include <xc.h>
#define _XTAL_FREQ 6000000
unsigned char val;
void main(void) {           //Thread mode
    TRISB = 0x01;
    TRISC = 0x00;
    OPTION_REG &= 0x7F;
    PORTB = 0x00;
    PORTC = 0x00;
    TRISD = 0x00;
    PORTD = 0x00;
    INTCON = 0x90;          //Configuring INTCON register
}
void __interrupt() ISR_EXT_INT() {
    if(INTCON&0x02) {
        val = PORTB;
        if(PORTB&0x01) {    //handler mode
            PORTC = 0xFF;
            __delay_ms(200);
            PORTC = 0x00;
        }
        INTCON &= 0xFD;     //Clear flag
    }
}

