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
void LCD_Comm(unsigned char);
void LCD_Data(unsigned char);
void LCDOutput(unsigned int);
unsigned int num1, num2, sub, m;
unsigned char x, k[30], n;

void main() {
    init();             //initializing the LCD
    num1=100;
    num2=200;
    LCD_Comm(0x80);     //1st row 1st column address of LCD
    LCDOutput(num1);    //passing integer as parameter
    LCD_Data('-');      //passing character as parameter
    LCDOutput(num2);
    LCD_Data('=');
    sub = num1-num2;    //subtracting
    if (sub>32767) {    //Checking if sub stores 2's complemented value
        LCD_Comm(0x88);
        LCD_Data('-');
        sub = num2-num1;
        LCDOutput(sub); 
    }
    else {
        LCDOutput(sub);
    }
    while(1);
}

void init(void) {
    TRISC=0x00;
    TRISD=0x00;
    LCD_Comm(0x38);     //2line display with 5x7 matrix
    __delay_ms(100);
    LCD_Comm(0x38);     //2line display with 5x7 matrix
    __delay_ms(100);
    LCD_Comm(0x38);     //2line display with 5x7 matrix
    __delay_ms(100);
    LCD_Comm(0x38);     //2line display with 5x7 matrix
    __delay_ms(100);
    LCD_Comm(0x0E);     //display ON with cursor
    __delay_ms(100);
    LCD_Comm(0x06);     //entry mode set for right shift
    __delay_ms(100);
    LCD_Comm(0x01);     //clear display
    __delay_ms(100);
}

void LCDOutput (unsigned int i) {   //converting integer to char
    unsigned char s, j=1;
    m=i;
    while(m) {
        s = m-((m/10)*10);
        k[j]=s; j++; m/=10;
    } 
    k[j]='\0'; --j;
    while(j) {
        n=0x30+k[j];
        LCD_Data(n); j--;
    }
}

void LCD_Comm (unsigned char i) {
    PORTC&=0xF7;    //RC3=0 for RS=0
    PORTD = i;      
    PORTC|=0x01;    //RC0=1 for EN=1
    __delay_ms(100);
    PORTC&=~0x01;   //RC0=0 for EN=0
}

void LCD_Data (unsigned char i) {
    PORTC|=0x08;    //RC3=1 for RS=1
    PORTD = i;
    PORTC|=0x01;    //RC0=1 for EN=1
    __delay_ms(100);
    PORTC&=~0x01;   //RC0=0 for EN=0
}