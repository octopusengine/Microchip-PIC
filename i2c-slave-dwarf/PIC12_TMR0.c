#include <xc.h>
#include "PIC12_TMR0.h"

unsigned int TMR0_counter = 0;

void PIC12_TMR0_handler(void) {
    if (TMR0IF) {
        TMR0_counter++; // Increment Timer0 software counter
        TMR0IF = 0; // Reset Timer0 interrupt flag
    }
}

void PIC12_TMR0_init() {
    OPTION_REGbits.PSA = 0; // Use Prescaler
    OPTION_REGbits.PS = 4; // Timer0 Prescaler Rate = 1:32
    T0CS = 0; // Select internal clock
    T0IE = 1; // Enable timer0 interrupt
}
