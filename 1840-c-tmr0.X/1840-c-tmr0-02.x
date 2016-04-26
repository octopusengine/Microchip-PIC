/* PROJECT: ad
 * File:    1840-c-tmr0 test
 * Author:  yenda
 * Modif:   17.05.2014
 *

 */
#include <xc.h>
#include <pic.h>
#include <htc.h>

#include <pic12f1840.h>
#include "config1840.h"

#define _XTAL_FREQ 4000000
//#include <basic2014.h>
#include "mojeFce1840-01.h"

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

//4094 (PIN)
#define STR 0 //1
#define DAT 1 //2
#define CLK 2 //3

//---------------------------------------------------------------------
__EEPROM_DATA(0b00000001,  0b00000010, 0b00000100, 0b00001000,  0b00010000, 0b00100000, 0b01000000,0b10000000);
__EEPROM_DATA(0b11111111,  0b00000010, 0b00000100, 0b00001000,  0b00010000, 0b00100000, 0b01000000,0b10000000);
//__EEPROM_DATA(0b000000001, 0b000000010, 0b000000011, 0b00000100, 0b00000101, 0b000000110, 0b000000111, 0b000000111);

int sGPIO;
unsigned int valADc, valADcSave;
int valADc16;
int adr = 16; //za?ínám od 16, ní?e je tabulka __EEPROM_DATA

void init(void);
void initTest(void);
int adcSample(void);
//void zobrazLed(int val);
void zobrazBar(int val);
int buttonpressed(int poz);
void posliData(int co, int value);
void ulozData(int value);
//==============================================================================
//hlavní =======================================================================
void main (void)
{
    init();
    initTest();
    //uart_init_fast();
    uart_init600();
    //uart_init2400();
    //uart_init9600();
    //uart_init19200();
    //uart_init57600();
    //setUref2V();

    // enable interrupts
    INTCONbits.T0IE = 1;            // enable Timer0 interrupt
    ei();

    while(1)    // LOOP FOREVER
    {

        initAnalog(0);
        adcSample();
        posliData('1',valADc);
        valADcSave = valADc;

        initAnalog(1);
        adcSample();
        posliData('2',valADc);

        //initAnalog(2);
        //adcSample();
        //blik(LED2);

        if(buttonpressed(T1)==1)
      {
        // if (testbit(PORTA,T1))
        //setbit(sGPIO,LED1);
            beep(PIEZO);
            posliData('5',valADcSave);
            ulozData(valADcSave);

        }

    }
}
//==============================================================================
/***** INTERRUPT SERVICE ROUTINE *****/
void interrupt isr(void)
{
    beep(PIEZO);
    
    //static uint16_t  cnt_t0 = 0;    // counts timer0 overflows
    static int  cnt_t0 = 0;    // counts timer0 overflows
    
    //*** Service Timer0 interrupt  
    //  TMR0 overflows every 250 clocks = 250 us   
    //  Flashes LED at 1 Hz by toggling on every 2000th interrupt (every 500 ms)
    //   (only Timer0 interrupts are enabled)

    TMR0 += 256-250+3;              // add value to Timer0
                                    //   for overflow after 250 counts
    INTCONbits.T0IF = 0;            // clear interrupt flag
    
    // toggle LED every 500 ms
    ++cnt_t0;                       // increment interrupt count (every 250 us)
    if (cnt_t0 == 500000/250) {     // if count overflow (every 500 ms),
        cnt_t0 = 0;                 //   reset count
        //sF_LED = ~sF_LED;           //   toggle LED (via shadow register)
        
        
    }
}
//=========================================================================


void ulozData(int value)
{
            EEPROM_WRITE(adr, value);
            __delay_ms(10);
            //beep2(PIEZO);
            //__delay_ms(300);
            adr++;
            if (adr>255) adr = 16;
}

void posliData(int ch, int value)
{
        uart_char('#');
        uart_char(ch);
        uart_char('>');
        uart_3numb(value);
        uart_char('*');

        __delay_ms(50); //20ne! 50 jo
}



/////////////////////////////////////////////////////////////////////////////////////////
//this routine is to check button pressings.
//input: none
//output: 0 if switch is not pressed. 1 if switch is pressed.
/////////////////////////////////////////////////////////////////////////////////////////
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

void init()
{
    set4mhz();
    //set8mhz();
    //set32mhz();

    ADCON0 = 0;                     //adc disabled
    //;clrf   ANSEL         ; Make all ports as digital I/O
    ANSELA = 0;                     //all digital
    CM1CON0 = 0;                    //comparator disabled
    CPSCON0 = 0;                      //cap sense disabled
    //CMCON0 = 1<<CM2 | 1<<CM1 | 1<<CM0;
    PORTA = 0;
    //TRISA  = 0b011100; // out = 0 (in =1)
    TRISA = 1<<AN | 1<<T1;

    beep(PIEZO);
    __delay_ms(500);
    beep2(PIEZO);
    __delay_ms(500);
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

    sGPIO = 0;
    PORTA = sGPIO;
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




