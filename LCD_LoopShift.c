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
# define _XTAL_FREQ 6000000
void init();
void LCD_Command(unsigned char i);      //function for LCD Command
void LCD_Data (unsigned char i);        //function for LCD data
unsigned char arr[] = {"POOJAA_R"};  //The string of data
unsigned char i, m, n; 
//LCD DISP USING LEFT AND RIGHT SHIFT WITH LOOPS
void main() {
    init();     //initializing the LCD
    m = 0x80;   //1st line address
    for (i=0; i<=7; i++) {
        LCD_Command(m);
        LCD_Data(arr[i]);   //passing the data
        m+=0x01;    //incrementing the address
    }
    n = 0xC7;   //2nd line address
    for (i=0; i<=7; i++) {
        LCD_Command(n);
        LCD_Data(arr[i]);   //passing the data
        n-=0x01;    //decrementing the address
    }
    while(1);
}

void init() {
    TRISC = 0x00;
    TRISD = 0x00;
    LCD_Command (0x30);     //function set command must be executed around 4ms
    __delay_ms(100);
    LCD_Command (0x30);     //function set command must be executed around 4ms
    __delay_ms(100);
    LCD_Command (0x30);     //function set command must be executed around 4ms
    __delay_ms(100);
    LCD_Command (0x30);     //function set command must be executed around 4ms
    __delay_ms(100);
    LCD_Command (0x38);     //2 line and 5x7 matrix
    __delay_ms(100);
    LCD_Command (0x0E);     //display on with cursor on and doesn't blinks the char that the cursor points to, 3rd bit is always 1
    __delay_ms(100);
    LCD_Command (0x01);     //clear display
    __delay_ms(100);
}

void LCD_Command(unsigned char n) {
    RC3 = 0;        //RS = 0 for commands
    PORTD = n;      //Data Bus
    RC0 = 1;        //Enable
    __delay_ms(100);
    RC0 = 0;
}

void LCD_Data(unsigned char n) {
    RC3 = 1;        //RS = 1 for data
    PORTD = n;      //Data Bus
    RC0 = 1;        //Enable
    __delay_ms(100);
    RC0 = 0;
}