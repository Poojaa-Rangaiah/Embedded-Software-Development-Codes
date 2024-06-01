#include <xc.h>
#define _XTAL_FREQ      16000000
#define Slave_Add       0x70    // any general purpose register address
void i2c_Slave_init(unsigned char);
unsigned char read=0x00;
void main(void) {
    TRISD = 0x00;
    i2c_Slave_init(Slave_Add);    // 100 kbps
    while(1);
}
void i2c_Slave_init(unsigned char add) {
    SSPCON = 0x36;     // slave mode
    SSPCON2 = 0x01;
    SSPADD = add;
    SSPSTAT = 0x80;    // only SSPSTAT<7,6> can be modified
    TRISC = 0x018;     // SDA, SCL
    INTCON |= 0xC0;    // Enable Interrupts GIE, PEIE
    PIR1&=~0x08;       // SSPIF = 0
    PIE1|=0x08;        // SSPIE = 1
}
void __interrupt() I2C_Slave_Read() {
    if(PIR1&0x08) {                 // SSPIF
        if(!(SSPSTAT&0x04)) {       // R_nW (when it is write from master to slave)
            read = SSPBUF;          // dummy read
            SSPCON|=0x10;           // CKP=1, release the clock
            while(!(SSPSTAT&0x01)); // BF=1, SSPBUF is full else empty
            read = SSPBUF;          // read the data from buffer
            PORTD = read;
        }
        SSPCON|=0x10;               // CKP=1, release the clock
        PIR1 &= ~0x08;              // clear SSPIF
    }
}