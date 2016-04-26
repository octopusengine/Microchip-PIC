#include <htc.h>
#include "PIC12_TMR1.h"

void PIC12_TMR1_handler(void)
{
    if (TMR1IF)
    {
        TMR1IF = 0;         // Clear TMR1 Interrupt Flag
    }
}

void PIC12_TMR1_init()
{
    // The below code set up TIMER1 input clock = 1 MHz
    T1CONbits.TMR1CS = 0;   // Clock source = instruction clock (Fosc/4)
    T1CONbits.T1CKPS = 3;   // Prescale = 1:8
    TMR1ON = 1;             // TMR1 Module ON/OFF
    TMR1IE = 0;             // TMR1 Interrupt Enable/Disable
}
