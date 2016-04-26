/************************************************************************                                                                 *
*   Filename:      MC_L3-Flash_LED-50p-int-1Hz-HTC.c                    *
*   Date:          8/6/12                                               *
*   File Version:  1.5                                                  *

*   Processor:     12F629                                               *
*   Compiler:      MPLAB XC8 v1.00 (Free mode)                          *
*************************************************************************
*                                                                       *
*   Description:    Lesson 3, example 1c                                *
*   Demonstrates use of Timer0 interrupt to perform a background task   *
*   Flash LED at exactly 1 Hz (50% duty cycle)                          *
************************************************************************/

//03.ok - posílá - ukládá min max 1 2, výpis eeprom
//02.ok - cca ka?dou minutu zapisuje min,max do eeprom
//01.ok - chodí timer

#include <xc.h>
#include <stdint.h>

#include <pic12f1840.h>

/***** CONFIGURATION *****/
#include "config1840.h"
#define _XTAL_FREQ 4000000
#include "mojeFce1840-01.h"

// Pin assignments

// --------------------------------------------------------------------
//  PIC schema
//---------------------------------------------------------------------
//       VDD + -| P |- VSS
//   -> T1 GP5 -| I |- GP0 LED1 / AN0 <-
//   <- TX GP4 -| C |- GP1 LED2 / AN1 <-
//         GP3 -|   |- GP2 LED3 / AN2 / PIEZZO ->
//---------------------------------------------------------------------
#define LED1 0
#define LED2 1
#define LED3 2
#define PIEZO 2
#define T1 5
#define T2 4
#define T3 3
#define AN 0

int valADc;
int valADc16;
int min1 = 255;
int max1 = 0;
int min2 = 255;
int max2 = 0;
int adr = 0; //zacínám od 1 __EEPROM_DATA
int sec; //po?et vterin

void init(void);
int adcSample(void);
//void zobrazLed(int val);
int buttonpressed(int poz);
void posliData(int co, int value);
void ulozData(int value);
void kazdouVterinu(void);
void kazdouMinutu(void);
void vypisEEprom(void);
//==============================================================================
void init()
{
    //*** Initialisation
    set4mhz();

    // configure port
    PORTA = 0;
    //TRISA  = 0b011100; // out = 0 (in =1)
    //TRISA = ~(1<<2);               // configure GP2 (only) as an output
    TRISA = 1<<AN | 1<<T1;

    ADCON0 = 0;                     //adc disabled
    //;clrf   ANSEL         ; Make all ports as digital I/O
    ANSELA = 0;                     //all digital
    CM1CON0 = 0;                    //comparator disabled
    CPSCON0 = 0;                      //cap sense disabled
    //CMCON0 = 1<<CM2 | 1<<CM1 | 1<<CM0;

    beep(PIEZO);
    __delay_ms(500);
    beep2(PIEZO);
    __delay_ms(500);

    // configure Timer0
    OPTION_REGbits.T0CS = 0;        // select timer mode
    OPTION_REGbits.PSA = 1;         // no prescaler (assigned to WDT)
                                    // -> increment every 1 us
    // enable interrupts
    INTCONbits.T0IE = 1;            // enable Timer0 interrupt
    ei();                           // enable global interrupts
}


/***** MAIN PROGRAM *****/
void main()
{
    init();
    uart_init600();

    //*** Main loop
    for (;;)
    {
        // continually copy shadow GPIO to port
         if(buttonpressed(T1)==1)
      {
            beep2(PIEZO);
            vypisEEprom();

      }

    }   // repeat forever
}


/***** INTERRUPT SERVICE ROUTINE *****/
void interrupt isr(void)
{
    static uint16_t  cnt_t0 = 0;    // counts timer0 overflows
    //*** Service Timer0 interrupt
    //  TMR0 overflows every 250 clocks = 250 us
    //  Flashes LED at 1 Hz by toggling on every 2000th interrupt (every 500 ms)
    //   (only Timer0 interrupts are enabled)
    TMR0 += 256-250+3;     // add value to Timer0 for overflow after 250 counts
    INTCONbits.T0IF = 0;            // clear interrupt flag

    // toggle LED every 500 ms
    ++cnt_t0;                       // increment interrupt count (every 250 us)
    if (cnt_t0 == 500000/250*2) {     // if count overflow (every 500*2 ms),
        cnt_t0 = 0;                 //   reset count
        //sF_LED = ~sF_LED;           //   toggle LED (via shadow register)
        kazdouVterinu();
    }
}

void kazdouVterinu()
{
    //beep2(PIEZO);

    initAnalog(0);
    adcSample();
    posliData('1',valADc);
    __delay_ms(10);

    if (valADc<min1) min1 = valADc;
    if (valADc>max1) max1 = valADc;

    //posliData('n',min1);
    //__delay_ms(10);
    //posliData('x',max1);
    //__delay_ms(10);
     
    initAnalog(1);
    adcSample();
    posliData('2',valADc);
    __delay_ms(10);

    if (valADc<min2) min2 = valADc;
    if (valADc>max2) max2 = valADc;

    sec++;
    if (sec==840) //42 ok? 1 minuta //420 - 10 // 840 - 20
    {
        sec=0;
        kazdouMinutu(); //20 minut
    }
}

void kazdouMinutu()
{
    beep(PIEZO);
    //ulozData(valADc);
    ulozData(min1);
    ulozData(max1);
    min1 =255;
    max1= 0;

    ulozData(min2);
    ulozData(max2);
    min2 =255;
    max2= 0;
}

int buttonpressed(int poz)
{
    //if (GPIObits.GP3==1)        // is button pressed?
    if (testbit(PORTA,poz))
    {                            //yes
       __delay_ms(5);            // wait 5mS for debounce.
        if (testbit(PORTA,poz))    // is button still has pressed status after 10mS delay?
        {                        // yes, we have key press
            return 1;
        }
    }
    return 0;// if key is not pressed, return 0
}

void vypisEEprom()
{
    INTCONbits.T0IE = 0;            // zakázat interrupt

    uart_char(' ');
    uart_char(' ');
    uart_char(' ');
    uart_char(' ');
    uart_char(' ');

    uart_char('<');
    uart_char('T');
    uart_char('A');
    uart_char('B');
    uart_char('L');
    uart_char('E');
    uart_char('>');

    for (int i=0; i<256/4; i++)
    {

        uart_char('<');
        uart_char('T');
        uart_char('R');
        uart_char('>');

        uart_char('<');
        uart_char('T');
        uart_char('D');
        uart_char('>');
        uart_3numb(EEPROM_READ(i*4));

        uart_char('<');
        uart_char('T');
        uart_char('D');
        uart_char('>');
        uart_3numb(EEPROM_READ(i*4+1));

        uart_char('<');
        uart_char('T');
        uart_char('D');
        uart_char('>');
        uart_3numb(EEPROM_READ(i*4+2));

        uart_char('<');
        uart_char('T');
        uart_char('D');
        uart_char('>');
        uart_3numb(EEPROM_READ(i*4+3));
        //uart_char(',');

        uart_char('<');
        uart_char('/');
        uart_char('T');
        uart_char('R');
        uart_char('>');
      
    }

    uart_char('<');
    uart_char('/');
    uart_char('T');
    uart_char('A');
    uart_char('B');
    uart_char('L');
    uart_char('E');
    uart_char('>');

    INTCONbits.T0IE = 1;            // enable Timer0 interrupt
    ei();
}

void ulozData(int value)
{
            EEPROM_WRITE(adr, value);
            __delay_ms(10);
            //beep2(PIEZO);
            //__delay_ms(300);
            adr++;
            if (adr>255) adr = 0;
}

void posliData(int ch, int value)
{
        uart_char(' ');
        uart_char(' ');
        uart_char(' ');
        __delay_ms(30);

        uart_char('#');
        uart_char(ch);
        uart_char('>');
        uart_3numb(value);
        uart_char('*');
        __delay_ms(30);
        
        uart_char(' ');
        uart_char(' ');
        uart_char(' ');
        uart_char(' ');
        uart_char(' ');

        __delay_ms(50); //20ne! 50 jo
}

void initTest()
{
    beep(PIEZO);
    PORTA = 0b111111;
    __delay_ms(500);
    PORTA = 0;
    __delay_ms(500);

    blik(0);
    blik(1);
    blik(2);

    //sGPIO = 0;
    //PORTA = sGPIO;
    //beep();

    //beepUni(PIEZO,100);
    //beepUni(PIEZO,30);
}


int adcSample()
{
     valADc = 0;
//      ;ADRESH  :ADRESL
//	;98765432:10 (10 bit result)
    ADCON0bits.GO = 1;          //start conversion
    while (ADCON0bits.GO_nDONE)    //wait until done
        //sGPIO = ADRESH >>4; //copy high nybble of result to leds

        //valADc = (ADRESH *256 + ADRESL)/256; //pro 1840, asi jinak bit?)

        //valADc =(ADRESH<<4);
        //valADc | = ADRESL;
        valADc = ADRESH;


    //valADc = ADRESH << 8;
    //valADc |= ADRESL;

}