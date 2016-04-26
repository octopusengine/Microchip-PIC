 /* PROJECT: ad
 * File:    1840-c-ad
 * Author:  yenda
 * Modif:   12.04.2014
 * Pozn.    adc na 3 Led
 */
#include <xc.h>
#include <pic.h>
#include <htc.h>

#include <pic12f1840.h>
#include "config1840.h"

#define _XTAL_FREQ 4000000
//#include <basic2014.h>
#include "mojeFce1840-02.h"

// --------------------------------------------------------------------
//  PIC schema
//---------------------------------------------------------------------
//       VDD + -| P |- VSS
//PIEZO T1 GP5 -| I |- GP0 LED1 / AN0
//  AN3 T2 GP4 -| C |- GP1 LED2 / AN1
//      T3 GP3 -|   |- GP2 / PIEZO
//---------------------------------------------------------------------
#define LED1 5
#define LED2 4
#define LED3 2
#define LED5 2 //zatez
//#define PIEZO 5
#define T1 5
#define T2 4
#define T3 3
#define AN1 0
#define AN2 1

//4094 (PIN)
#define STR 0 //1
#define DAT 1 //2
#define CLK 2 //3

//---------------------------------------------------------------------
//__EEPROM_DATA(0b00000001,  0b00000010, 0b00000100, 0b00001000,  0b00010000, 0b00100000, 0b01000000,0b10000000);
//__EEPROM_DATA(0b11111111,  0b00000010, 0b00000100, 0b00001000,  0b00010000, 0b00100000, 0b01000000,0b10000000);
//__EEPROM_DATA(0b000000001, 0b000000010, 0b000000011, 0b00000100, 0b00000101, 0b000000110, 0b000000111, 0b000000111);

int sGPIO;
unsigned int valADc;
int valADc16;
int adr = 1; //
int vypnuto = 0; //po?et vypnutí p?i vybití
int val1;
int val2;

void init(void);
void initTest(void);
int adcSample(void);
//void zobrazLed(int val);
void zobrazBar(int val);
int buttonpressed(int poz);
void zobrazLed(int pom);
void ulozData(int value);
//==============================================================================
//hlavní =======================================================================
void main (void)
{
    init();
    initAnalog(0);
    adcSample();
    __delay_ms(10);
    val1 = valADc;

    initAnalog(1);
    adcSample();
    __delay_ms(10);
    val2 = valADc;

    ulozData(val1);
    ulozData(val2);

    clrbit(PORTA,LED2);
    zobrazLed(0);
    setbit(PORTA,LED2);
    __delay_ms(5000);
    zobrazLed(0);

    //initTest();
    
    while(1)    // LOOP FOREVER
    {
        initAnalog(0);
        adcSample();
        __delay_ms(10);
        val1 = valADc;

        initAnalog(1);
        adcSample();
        __delay_ms(10);
        val2 = valADc;

        
        //if (val1>0x81){ setbit(PORTA,LED1);} //090 bylo cca 3.5V 080 je 3
        //else { clrbit(PORTA,LED1);}

        //if (val2>0x81){ setbit(PORTA,LED2);} //090 bylo cca 3.5V 080 je 3
        //else { clrbit(PORTA,LED2);}

        if ((val2>0xA0)&&(val1>0x9A))   {
            setbit(PORTA,LED2); //090 bylo cca 3.5V 080 je 3
            clrbit(PORTA,LED1);
           
        }
        else {
            clrbit(PORTA,LED2);
            zobrazLed(val1);
            vypnuto++;
        }



        //ulozData(val1);
        //ulozData(val2);
        __delay_ms(9000);

        if (vypnuto>2)
        {
            ulozData(val1);
            ulozData(val2);
            clrbit(PORTA,LED2);
            while(1)
            {
             zobrazLed(0xFF);
             __delay_ms(3000);
            }
        }
        
    }
}
//==============================================================================

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
    adr=0;
    set4mhz();
    //set32mhz();

    ADCON0 = 0;                     //adc disabled
    //;clrf   ANSEL         ; Make all ports as digital I/O
    ANSELA = 0;                     //all digital
    CM1CON0 = 0;                    //comparator disabled
    CPSCON0 = 0;                      //cap sense disabled
    //CMCON0 = 1<<CM2 | 1<<CM1 | 1<<CM0;
    PORTA = 0;
    //TRISA  = 0b011100; // out = 0 (in =1)
    //TRISA = 1<<AN | 1<<T1; //tla?ítko pro zápis
    TRISA = 1<<AN1 | 1<<AN2; // vstupy
}

void initTest()
{
    //beep(PIEZO);
    sGPIO = 0;
    PORTA = sGPIO;   
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


void goPat(int byte8) //zobrazí Byte 1/strobe0 2/data1 3/clock2
{
    for(int i = 0; i<16; i++)
    {
      if (testbit(byte8,i)) {setbit(PORTA,LED1);}
      else {clrbit(PORTA,LED1);}
      //PORTA = sGPIO;
      __delay_ms(150);
    }
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


void zobrazLed(int pom)
{
    int pat16;
    //90-A1-A0-A8
    //90-A1-A0-A3-A5-A8

    if (pom>0x80){ setbit(PORTA,LED1);} //090 bylo cca 3.5V 080 je 3
    else { clrbit(PORTA,LED1);}
                   pat16 = 0b1010101010101010;
    if (pom>0x90)  pat16 = 0b1100000110000000;
    if (pom>0x98)  pat16 = 0b1100110011010000;
    if (pom>0xA0)  pat16 = 0b1110000000000000;
    if (pom>0xA8)  pat16 = 0b1111111100000000;
    if (pom>0xB0)  pat16 = 0b1111111111100000;
    if (pom>0xB8)  pat16 = 0b1111111111111110;
    if (pom>0xDD)  pat16 = 0b1000000000000000;
    goPat(pat16);
   
}
