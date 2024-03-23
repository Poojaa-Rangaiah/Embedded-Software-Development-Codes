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
void LCDOutput(unsigned int i, unsigned int k);
void keyscan(void);
void Bt_Com(unsigned int);
unsigned char arr[30]={"BAT VOLT:    V"}, val, x;
unsigned char low[15]={"BAT LOW   "}; 
unsigned char norm[15]={"BAT NORMAL"}; 
unsigned char high[15]={"BAT HIGH  "}; 
unsigned int d2,d1,d0,j,n;
void main(void) {
    init();     //initializing
    j=15;    
    n=5;
    while(1) {
        keyscan();  //scanning for key press
    }
}

void keyscan(void) {
    val = PORTB&0xF0;
    switch(val) {
        case 0xE0:              //if RC4 is pressed
            LCD_Comm(0x80);
            for(x=0;x<9;x++) {
                LCD_Data(arr[x]);
            }
            LCD_Comm(0x8E);
            LCD_Data(arr[13]);
            LCD_Comm(0x8A);
            LCDOutput(j,n);
            Bt_Com(j);          //Battery comment
            break;
        case 0xD0:              //if RC5 is pressed
            n++;
            if(n>9) {
                n=0; j++;
            }
            if(j==22 && n>5) {
                j=22; n=5;
            }
            LCD_Comm(0x8A);
            LCDOutput(j,n);
            Bt_Com(j);          //Battery comment
            break;
        case 0xB0:              //if RC6 is pressed
            n--;
            if(n>32767) {
                n=9; j--;
            }
            if(j==15 && n<5) {
                j=15; n=5;
            }
            LCD_Comm(0x8A);
            LCDOutput(j,n);
            Bt_Com(j);          //Battery comment
            break;
        case 0x70:              //if RC7 is pressed
            j=15; n=5;
            LCD_Comm(0x8A);
            LCDOutput(j,n);
            Bt_Com(j);          //Battery comment
            break;
    }
}

void init(void) {
    TRISC=0x00;
    TRISD=0x00;
    TRISB=0xF0;
    OPTION_REG&=0x7F;
    LCD_Comm(0x38);     //2 line display with 5x7 matrix
    __delay_ms(100);
    LCD_Comm(0x38);     //2 line display with 5x7 matrix
    __delay_ms(100);
    LCD_Comm(0x38);     //2 line display with 5x7 matrix
    __delay_ms(100);
    LCD_Comm(0x38);     //2 line display with 5x7 matrix
    __delay_ms(100);
    LCD_Comm(0x0C);     //display ON
    __delay_ms(100);
    LCD_Comm(0x06);     //entry mode set for right shift
    __delay_ms(100);
    LCD_Comm(0x01);     //clear display
    __delay_ms(100);
}

void Bt_Com(unsigned int j) {
    LCD_Comm(0xC0);
    if(j>=15 && n>=5) {     //Battery low comment
        if(j<17 || (j==17 && n<=5)) {
            for(x=0;x<10;x++) {
                LCD_Data(low[x]);
            }
        }
    }
    if(j>=17 && n>5) {      //Battery normal comment
        if(j<20 || (j==20 && n<=5)) {
            for(x=0;x<10;x++) {
                LCD_Data(norm[x]);
            }
        }
    }
    if(j>=20 && n>5) {      //Battery high comment
        if(j<22 || (j==22 && n<=5)) {
            for(x=0;x<10;x++) {
                LCD_Data(high[x]);
            }
        }
    }
}

void LCDOutput (unsigned int i, unsigned int k) {
    d2 = (unsigned char)(i/10);                 //tenth place
    d1 = (unsigned char)(i-(d2*10));            //unit place 
    d0 = (unsigned char)k;                      //decimal place
    LCD_Comm(0x8A);
    LCD_Data(0x30+d2);
    LCD_Data(0x30+d1);
    LCD_Data('.');
    LCD_Data(0x30+d0);
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