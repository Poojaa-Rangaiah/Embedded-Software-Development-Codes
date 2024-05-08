#include <xc.h>
#define _XTAL_FREQ 16000000
#define EN RC0
#define RS RC1
//int *SSPCON_2 = 0x91, *SSPCON_1 = 0x14, *SSP_BUF = 0x13, *SSP_STAT = 0x94, *SSP_ADD = 0x93, *PIF1 = 0x0C;
//int *DRC = 0x87, *DRD = 0x88, *dataC = 0x07, *dataD = 0x08;
unsigned char *SSPCON_2, *SSPCON_1, *SSP_BUF, *SSP_STAT, *SSP_ADD, *PIF1, *DRC, *DRD, *dataC, *dataD;
void i2c_init(const unsigned long);
void i2c_start();
void i2c_rep_start();
unsigned char i2c_read();
unsigned char i2c_write(unsigned char);
void i2c_stop();
void i2c_wait();
void lcd_cmd(unsigned char);
void lcd_data(unsigned char);
void init();
void setup();
void update();
int dec2bcd(int);
int bcd2dec(int);
void days(int);
int sec=58, min=59, hour=11, date=8, month=04, day=1, year=24, i, hour1;
unsigned char msg1[]={"TIM:"}, mode1[]={" AM"}, mode2[]={" PM"};
unsigned char msg2[]={"DAT:"};
void main(void) {
    init();
    i2c_init(10000);
    setup();                            //Writes only once 
    while(1) {
        update();                       //reads the updated time
        hour1=(dec2bcd(hour)&0x1F)-6;   //converts to conventional mode
        lcd_cmd(0x80);
        for(i=0; i<4; i++)
            lcd_data(msg1[i]);
        lcd_data(hour1/10+'0');
        lcd_data(hour1%10+'0');
        lcd_data('-');
        lcd_data(min/10+'0');
        lcd_data(min%10+'0');
        lcd_data('-');
        lcd_data(sec/10+'0');
        lcd_data(sec%10+'0');
        if(!(dec2bcd(hour)&0x20))       //checks if B5 is ~AM/PM
            for(i=0; i<3; i++)
                lcd_data(mode1[i]);     //AM
        else
            for(i=0; i<3; i++)
                lcd_data(mode2[i]);     //PM
        
        lcd_cmd(0xC0);
        for(i=0; i<4; i++)
            lcd_data(msg2[i]);
        lcd_data(date/10+'0');
        lcd_data(date%10+'0');
        lcd_data('-');
        lcd_data(month/10+'0');
        lcd_data(month%10+'0');
        lcd_data('-');
        lcd_data(year/10+'0');
        lcd_data(year%10+'0');
        if(dec2bcd(hour)&0x52) {    //Update the day when it is 12AM
            day++;
            if(day>7)
                day=1;
        }
        days(day);
        __delay_ms(100);
    }
}
void i2c_init(unsigned long baud) {
    SSPCON_1 = (unsigned char*)0x14;
    SSPCON_2 = (unsigned char*)0x91;
    SSP_STAT = (unsigned char*)0x94;
    SSP_ADD = (unsigned char*)0x93;
    SSP_BUF = (unsigned char*)0x13;
    PIF1 = (unsigned char*)0x0C;
    *SSPCON_1 = 0x28;
    *SSP_STAT = 0x00;
    *SSPCON_2 = 0x00;
    *SSP_ADD = (_XTAL_FREQ/(4*baud))-1;
}
void i2c_wait() {
    while((*SSP_STAT)&0X04 || (*SSPCON_2&0X1F));    //wait until the bus is idle
}
void i2c_start() {
    i2c_wait();
    (*SSPCON_2)|=0x01;                      //SEN
}
void i2c_rep_start() {
    i2c_wait();
    (*SSPCON_2)|=0X02;                      //RSEN
}
void i2c_stop() {
    i2c_wait();
    (*SSPCON_2)|=0x04;                      //PEN
}
unsigned char i2c_read() {
    int val;
    i2c_wait();
    (*SSPCON_2)|=0X08;                      //RCEN
    i2c_wait();
    if((*PIF1)&0x08)
        (*PIF1)&=~0x08;
    val = *SSP_BUF;
    i2c_wait();
    (*SSPCON_2)|=(((*SSPCON_2)&0x40)?0:1)&0x20; //ACKDT
    (*SSPCON_2)|=0x10;                      //ACKEN
    return val;
}
unsigned char i2c_write(unsigned char data) {
    i2c_wait();
    *SSP_BUF = data;
    if((*PIF1)&0x08)                       //SSPIF
        (*PIF1)&=~0x08;
    return (*SSPCON_2)&0x40;                //ACKSTAT
}
void lcd_cmd(unsigned char i) {
    RS = 0;
    *dataD = i;
    EN = 1;
    __delay_ms(5);
    EN = 0;
}
void lcd_data(unsigned char i) {
    RS = 1;
    *dataD = i;
    EN = 1;
    __delay_ms(5);
    EN = 0;
}
void init() {
    DRC = (unsigned char*)0X87;
    DRD = (unsigned char*)0X88;
    dataC = (unsigned char*)0x07;
    dataD = (unsigned char*)0x08;
    *DRC = (unsigned char*)0X18;
    *dataC = (unsigned char*)0x00;
    *DRD = (unsigned char*)0x00;
    lcd_cmd(0x38);
    __delay_ms(4);
    lcd_cmd(0x38);
    __delay_ms(4);
    lcd_cmd(0x38);
    __delay_ms(4);
    lcd_cmd(0x38);
    __delay_ms(4);
    lcd_cmd(0x0C);
    __delay_ms(4);
    lcd_cmd(0x06);
    __delay_ms(4);
    lcd_cmd(0x01);
    __delay_ms(4);
}
void days(int day) {
    switch(day) {       //To diplay day on LCD
            case 1:
                lcd_data(' '); lcd_data('S');
                lcd_data('U'); lcd_data('N');
                break;
            case 2:
                lcd_data(' '); lcd_data('M');
                lcd_data('O'); lcd_data('N');
                break;
            case 3:
                lcd_data(' '); lcd_data('T');
                lcd_data('U'); lcd_data('E');
                break;
            case 4:
                lcd_data(' '); lcd_data('W');
                lcd_data('E'); lcd_data('D');
                break;
            case 5:
                lcd_data(' '); lcd_data('T');
                lcd_data('H'); lcd_data('U');
                break;
            case 6:
                lcd_data(' '); lcd_data('F');
                lcd_data('R'); lcd_data('I');
                break;
            case 7:
                lcd_data(' '); lcd_data('S');
                lcd_data('A'); lcd_data('T');
                break;
        }
}
int dec2bcd(int temp) {
    return ((temp/10)<<4)+((temp%10));
}
int bcd2dec(int temp) {
    return ((temp>>4)*10)+((temp&0x0F));
}
void setup() {
    i2c_start();                    //S
    i2c_write(0xD0);                //Slave address with WR
    i2c_write(0x00);                //00h
    i2c_write(dec2bcd(sec));        
    i2c_write(dec2bcd(min));
    i2c_write(dec2bcd(hour)|0x60);  //12hour + PM //0x40 for AM //0x60 for PM
    i2c_write(dec2bcd(day));        //day=(flag)?1:day
    i2c_write(dec2bcd(date));
    i2c_write(dec2bcd(month));
    i2c_write(dec2bcd(year));
    i2c_stop();                     //P
}
void update() {
    i2c_start();                    //s
    i2c_write(0xD0);                //Slave address with WR
    i2c_write(0x00);                //location 00h
    i2c_stop();                     //P
//    i2c_rep_start();
    i2c_start();                    //s
    i2c_write(0xD1);                //Slave address with RD
    sec = bcd2dec(i2c_read());      //reads at 00h
    min = bcd2dec(i2c_read());      //01h
    hour = bcd2dec(i2c_read());     //02h
    day = bcd2dec(i2c_read());
    date = bcd2dec(i2c_read());
    month = bcd2dec(i2c_read());
    year = bcd2dec(i2c_read());
    i2c_stop();
    i2c_start();                    //To secure data from garbage data
    i2c_write(0xD1);
    i2c_read();
    i2c_stop();
}