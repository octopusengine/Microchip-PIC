//***********************************************************************
//  PIC12 code example - PWM(P1A/TMR2), ADC, I2C Slave, WDT
//                    PIC12LF1822/40
//                    --------------
//                    |     U      |
//             + [VDD]|1          8|[VSS] -
//                    |            |
//  PWM <--- P1A [RA5]|2          7|[RA0/ICSPDAT]-->LED1
//                    |            |
//      ---> AN3 [RA4]|3          6|[RA1/ICSPCLK] <--- SCL
//                    |            |
//      [MCLR/VPP/RA3]|4          5|[RA2] <--> SDA
//                    |            |
//                    --------------
//***********************************************************************

#include "PIC12_ADC.h"
#include "PIC12_I2C_Slave.h"
#include "PIC12_ECCP.h"
#include "PIC12_TMR2.h"
#include "PIC12_WDT.h"
#include "oeLib.h"
#define _XTAL_FREQ 8000000

//__EEPROM_DATA(0b000000001, 0b000000010, 0b000000011, 0b000001000, 0b000010000, 0b000100000, 0b001000000, 0b010000000);
__EEPROM_DATA(16, 03, 0, 3, 123, 55, 0, 0);
#define LED1 0
int STAV;


void interrupt ONE_ISR_FITS_ALL(void) {
    //PIC12_TMR0_handler();
    PIC12_I2C_Slave_handler();
}

void SYS_IO_init() {
    TRISA = 0<<LED1; //y

    nWPUEN = 0; // Enable all weak pull-up resistors
    ANSELA = 0; // Disable all analog functions first
    ANSA4 = 1; // Enable analog on RA4
    // Configure RA5 pin as a digital output for CCP1/P1A
    TRISA5 = 0;
    // Alternate Pin Config: Route CCP1/P1A function to RA5 (PIC12(L)F1822 only)
    CCP1SEL = 1;
    // Configure RA1 as digital in (SCL), disable analog, enable weak pull-up
    TRISA1 = 1;
    ANSA1 = 0;
    WPUA1 = 1;
    // Configure RA2 as digital in (SDA), disable analog, enable weak pull-up
    TRISA2 = 1;
    ANSA2 = 0;
    WPUA2 = 1;
}

void SYS_CLK_init(void) {
    OSCCONbits.IRCF = 14; // Internal Oscillator Frequency Select = 8 MHz
    OSCCONbits.SCS = 0; // Clock determined by FOSC<2:0> in Configuration Word 1
}

// logic for processing the data in the I2C transmit and receive buffers
void I2C_processBuffers(void) {
    if (SSPBuffer[SSPBUFIDX_SLEEP] == FALSE) {
        CCP1CONbits.CCP1M = PWM_P1AP1C_AH; // PWM mode: P1A,P1C active high
    } else {
        CCP1CONbits.CCP1M = CCP1_OFF; // Capture/Compare/PWM = OFF
        LATA5 = 0; // Force PWM output low
    }
    PR2 = SSPBuffer[SSPBUFIDX_PR2]; //1
    CCPR1L = SSPBuffer[SSPBUFIDX_CCPR1L]; //2
    STAV = SSPBuffer[3];
    //var =  SSPBuffer[4];
    //analog SSPBuffer[5]; 
}

int pat16;
void goPat16(int byte8) //zobrazí Byte 1/strobe0 2/data1 3/clock2
{
    for(int i = 0; i<16; i++)
    {
      if (testbit(byte8,i)) {setbit(PORTA,LED1);}
      else {clrbit(PORTA,LED1);}
      //PORTA = sGPIO;
      __delay_ms(150);
    }
}

int main(void) {
    SYS_IO_init(); // Initialize I/O functions
    SYS_CLK_init(); // Initialize System Clock
    PIC12_ADC_init(); // Initialize ADC
    PIC12_TMR2_init(); // Initialize Timer2
    PIC12_ECCP_init(); // Initialize ECCP module
    PIC12_I2C_Slave_init(); // Initialize I2C Slave
    PIC12_WDT_init(); // Initialize WDT
    GIE = 1; // Enable Global interrupts
    PEIE = 1; // Enable Peripheral interrupts

    SSPBuffer[SSPBUFIDX_SLEEP] = FALSE; //0
    SSPBuffer[SSPBUFIDX_PR2] = PR2; //1
    SSPBuffer[SSPBUFIDX_CCPR1L] = CCPR1L; //2

    SSPBuffer[10] = EEPROM_READ(0);//y
    SSPBuffer[11] = EEPROM_READ(1);//y
    SSPBuffer[12] = EEPROM_READ(2);//y
    SSPBuffer[13] = EEPROM_READ(3);//y
    SSPBuffer[14] = EEPROM_READ(4);//y
    SSPBuffer[15] = EEPROM_READ(5);//y
    
    //test blik
    for (int blik=0;blik<5;blik++)
    {
      setbit(PORTA,LED1);
      __delay_ms(200);
      clrbit(PORTA,LED1);
      __delay_ms(200);
    }

    while (1) {       

        I2C_processBuffers(); // Process the data in the I2C Tx and Rx buffers
       

        ///if (SSPBuffer[3] >128){ setbit(PORTA,LED1); }
        ///else { clrbit(PORTA,LED1); }

         if(STAV==1) { pat16 = 0b1111111111111110; } //Ready
         else {        pat16 = 0b0110110111111111; } //Boot

         if(STAV==2) { pat16 = 0b1111111111100000; } //Print         

         if(STAV==3) { pat16 = 0b1010101010101010; } //Err

         if(STAV==5){ //a/d + write eeprom
             pat16 = 0b1111111111111111;
             clrbit(PORTA,LED1);
             EEPROM_WRITE(0, SSPBuffer[10]);
             __delay_ms(500);
             setbit(PORTA,LED1);
             EEPROM_WRITE(1, SSPBuffer[11]);
             __delay_ms(500);
             clrbit(PORTA,LED1);
             EEPROM_WRITE(2, SSPBuffer[12]);
             __delay_ms(500);
             setbit(PORTA,LED1);
             EEPROM_WRITE(3, SSPBuffer[13]);
             __delay_ms(500);
             clrbit(PORTA,LED1);
             EEPROM_WRITE(4, SSPBuffer[14]);
             __delay_ms(500);
             setbit(PORTA,LED1);
             EEPROM_WRITE(5, SSPBuffer[15]);
             __delay_ms(500);             
         }

        if(STAV==6){ //a/d + write eeprom
             pat16 = 0b0000000000000001;
             PIC12_ADC_channelData[3] = PIC12_ADC_read(3); // Read AN3 input voltage
             SSPBuffer[5] = PIC12_ADC_channelData[3]; //y = moje vsuvka
             //PIC12_WDT_kick(); // Clear WDT
             }

        goPat16(pat16);
         ///if (SSPBuffer[SSPBUFIDX_SLEEP] != FALSE) {SLEEP();}
    }
}
