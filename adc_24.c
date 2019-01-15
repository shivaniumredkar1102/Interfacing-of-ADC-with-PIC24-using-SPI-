#define FCY 8000000UL
// PIC24FJ128GA106 Configuration Bit Settings

// 'C' source line config statements

// CONFIG3
#pragma config POSCMOD = XT             // Primary Oscillator Select (XT Oscillator mode selected)
#pragma config OSCIOFNC = OFF           // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = PRI              // Oscillator Select (Primary Oscillator (XT, HS, EC))
#pragma config IESO = ON                // Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) enabled)

// CONFIG1
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config WINDIS = OFF             // Watchdog Timer Window (Windowed Watchdog Timer enabled; FWDTEN must be 1)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config ICS = PGx2               // Comm Channel Select (Emulator/debugger uses EMUC2/EMUD2)
#pragma config GWRP = OFF               // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG port is disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
            // JTAG Port Enable (JTAG port is disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdio.h>
#include <p24FJ128GA010.h>
#include <libpic30.h>
#include <string.h>

// define configurable LCD pins 
#define RS_PIN LATBbits.LATB15 // RB15 --> RS
#define E_PIN LATDbits.LATD4  // RD4 --> E 
#define RW_PIN LATDbits.LATD5 //RD5 --> RW
#define D0_PIN LATEbits.LATE0 
#define D1_PIN LATEbits.LATE1
#define D2_PIN LATEbits.LATE2
#define D3_PIN LATEbits.LATE3
#define D4_PIN LATEbits.LATE4 // RE4 --> D4
#define D5_PIN LATEbits.LATE5 // RE5 --> D5
#define D6_PIN LATEbits.LATE6 // RE6 --> D6
#define D7_PIN LATEbits.LATE7 // RE7 --> D7

// Give direction to bits
#define RS_TRIS TRISBbits.TRISB15
#define E_TRIS  TRISDbits.TRISD4
#define RW_TRIS TRISDbits.TRISD5
#define D0_TRIS TRISEbits.TRISE0
#define D1_TRIS TRISEbits.TRISE1
#define D2_TRIS TRISEbits.TRISE2
#define D3_TRIS TRISEbits.TRISE3
#define D4_TRIS TRISEbits.TRISE4
#define D5_TRIS TRISEbits.TRISE5
#define D6_TRIS TRISEbits.TRISE6
#define D7_TRIS TRISEbits.TRISE7

#include "lcd.h"   // import lcd library
#define _XTAL_FREQ 16000000UL
#define CS   _RG9  // define chip select 


void spi_init(void)
{
    SPI2CON1bits.DISSCK = 0;              //Internal Serial Clock is Enabled
    SPI2CON1bits.DISSDO = 0;              //SDOx pin is controlled by the module
    SPI2CON1bits.MODE16 = 1;              //Communication is word-wide (16 bits)
    SPI2CON1bits.SMP = 0;                 //Input Data is sampled at the middle of data output time
    SPI2CON1bits.CKE = 0;                 //Serial output data changes on transition from
    SPI2CON1bits.CKP = 0;                 //Idle state for clock is a low level;   
    SPI2CON1bits.SPRE = 0b111;            // secondary prescale as 1:1
    SPI2CON1bits.PPRE = 0b00; 			  // Primary prescale as 64:1
    SPI2CON1bits.MSTEN = 1;               //Master Mode Enabled
    SPI2STATbits.SPIEN = 1;               //Enable SPI Module*/
    CS = 1;                               // Chipselect inactive
}
/* function to read SPI command */
int readADC(void)
{
    unsigned int result; 
    SPI2BUF = 0x01;
    CS = 0;
    while(!SPI2STATbits.SPIRBF);
    result = SPI2BUF;
    return result;
    
}

/*function for write SPI command*/
int writeSPI(void)
{
    SPI2BUF = 0x01;
    CS = 0;
    while(!SPI2STATbits.SPIRBF);
    return SPI2BUF;   
}

void main()
{
    unsigned char buffer[20];
    unsigned int result;
    float result1;
    TRISB = 0;
    TRISG = 0;
    RS_TRIS = 0;
    E_TRIS=0;
    RW_TRIS = 0;
    D0_TRIS = 0;
    D1_TRIS = 0;
    D2_TRIS = 0;
    D3_TRIS = 0;
    D4_TRIS=0;
    D5_TRIS=0;
    D6_TRIS=0;
    D7_TRIS=0;
    
    E_PIN = 0;
    RS_PIN = 0;
    RW_PIN = 0;
    
    __delay_ms(100);
    lcd_init();
    __delay_ms(100);
    lcd_cmd();
    __delay_ms(10);
    lcd_print("Hello World :)");
    __delay_ms(100); 
    spi_init();
    __delay_ms(100);
    lcd_clr();
    __delay_ms(100);
    lcd_print("spi init.....");
    __delay_ms(1000);
    
    while(1)
    {
        CS = 1;
        writeSPI();
        result = readADC();
        result1 = ((result*3.3)/1024);
        lcd_clr();
        __delay_ms(100);
        sprintf(buffer,"%f",result1);
        lcd_print(buffer);
        __delay_ms(1000);   
    }
            
    
}