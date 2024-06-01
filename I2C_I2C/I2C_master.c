#include <xc.h>
#define _XTAL_FREQ      16000000
#define Read_Add        0x71
#define Write_Add       0x70      // any general purpose register address
void i2c_master_init(unsigned long);
void i2c_start(void);
void i2c_stop(void);
void i2c_ACK(void);
void i2c_NACK(void);
void i2c_repeated_start(void);
unsigned char master_byte_write(unsigned char);
unsigned char master_byte_read();
void master_wait();
unsigned char arr[10] = {0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE0, 0xFE, 0xF6}, i=-1;
void main(void) {
    i2c_master_init(100000);
    TRISB = 0xFF;
    TRISC = 0x18;
    TRISD = 0x00;
    PORTB = 0xFF;
    PORTD = 0x00;
    OPTION_REG &= 0x7F;
    i2c_start();                  // Start
    master_byte_write(Write_Add); // Slave address
    master_byte_write(0x00);      // Data
    i2c_stop();                   // Stop
    while(1) {
        if(PORTB == 0xFE) {
            i2c_start();                    // Start
            master_byte_write(Write_Add);   // Slave address
            master_byte_write(arr[++i]);    // Incremented data
            i2c_stop();                     // Stop
            RD0 = 1;
            RD1 = 0;
            if (i >= 9)
                while(PORTB != 0xFD);
            while(PORTB == 0xFE);
        }
        else if(PORTB == 0xFD) {
            if(i<=0)
                i=1;
            i2c_start();                    // Start
            master_byte_write(Write_Add);   // Slave address
            master_byte_write(arr[--i]);    // Decremented data
            i2c_stop();                     // Stop
            RD0 = 0;
            RD1 = 1;
            while(PORTB == 0xFD);
        }
    }
}
void master_wait() {
    while(SSPSTAT&0x04 || SSPCON2&0x1F);    //loops until the bus is idle
}
void i2c_master_init(unsigned long baud) {
    SSPCON = 0x28;  // master mode
    SSPCON2 = 0;
    SSPADD = (_XTAL_FREQ/(4*baud))-1;
    SSPSTAT=0;      // only SSPSTAT<7,6> can be modified
    TRISC = 0x18;   // SDA, SCL
}
void i2c_start() {
    master_wait();
    SSPCON2|=0x01;  //SEN = 1
}
void i2c_repeated_start() {
    master_wait();
    SSPCON2|=0x02;  //RSEN = 1
}
void i2c_stop() {
    master_wait();
    SSPCON2|=0x04;  //PEN = 1
}
void i2c_ACK() {
    SSPCON2 &= 0xDF;    //ACKDT = 0
    master_wait();
    SSPCON2 |= 0x10;    //ACKEN = 1
}
void i2c_NACK() {
    SSPCON2 |= 0x20;    //ACKDT = 1
    master_wait();
    SSPCON2 |= 0x10;    //ACKEN = 1;
}
unsigned char master_byte_write(unsigned char write) {
    master_wait();
    SSPBUF = write;
    while(!(PIR1&0x08));    //SSPIF
    PIR1&=~0x08;
    return (SSPCON2&0x40);  //ACKSTAT
}
unsigned char master_byte_read() {
    master_wait();
    SSPCON2|=0x08;          //RCEN
    while(!(PIR1&0x08));    //SSPIF
    PIR1&=~0x08;
    master_wait();
    return (SSPBUF);
}